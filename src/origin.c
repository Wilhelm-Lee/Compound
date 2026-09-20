/*
 * This file is part of Compound library.
 * Copyright (C) 2024-2026  William Lee
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

/** @file origin.c */

#include "../inc/origin.h"

extern MemoryStack *MEMORY_STACK;

struct Origin {
  struct {
    /* allocation_metadata_slots. */
    Memory data[__COMPOUND_ORIGIN_MEMORY_METADATA_SLOT_MAXIMUM__];

    /* Even population_distribution_descriptors. */
    /*    1    2    3    4    5    6    7    8    9    0    1    2    3    ... */
    /*    0                                            1                   ... */
    /* 0b 1111 0011 1111 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 ... */
    /* Each bit represents one Header. */
    /* 0 = AVAILABLE
     * 1 = OCCUPIED */
    uint32_t occupations[__COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__];
  } meta;

  struct {
    /* heap_rawdata. */
    byte data[__COMPOUND_ORIGIN_HEAP_SIZE_MAXIMUM__];

    /* Odd population_distribution_descriptors.                                */
    /*    1    2    3    4    5    6    7    8    9    0    1    2    3    ... */
    /*    0                                            1                   ... */
    /*                                                                         */
    /* 0b 1111 1111 1111 1111 1111 1111 0000 0001 1111 1111 1111 1000 0000 ... */
    /*    ^~~~ ~~~~ ~~~^ ~~~~ ^~~~              ^ ~~~~ ~~~~ ~~~~ ^             */
    /*    begin        begin  length == 4       length == 13                   */
    /* Each bit represents an occupation of an allocation. */
    uint32_t occupations[__COMPOUND_ORIGIN_HEAP_OCCUPATION_COUNT_MAXIMUM__];
  } heap;
};

Origin *origin = nll;
Origin _origin = Empty;

static void print_long_long_binary(
  register const long long num,
  char *restrict const user_buff
) {
  register llong written = 0;
  char buff[32 + ((32 / 4) - 1) /* Spacing */ + 1 /* Null-Terminator */] = Empty;

  loop (i, 32) {
    register const llong bit = (num << i) & 0x80000000U;

    written += snprintf(buff + written, (sizeof(buff) - 1), "%c", '0' + !!bit);

    if (!((i + 1) % 4)) {
      written += snprintf(buff + written, (sizeof(buff) - 1), " ");
    }
  }

  if (user_buff) {
    memcpy(user_buff, buff, sizeof(buff) * sizeof(buff[0]));
    return;
  }

  printf(" %s"NL, buff);
}

inline llong Align(const llong size)
{
  if (!size) {
    return -1;
  }

  return ((size + 7) & ~7) + sizeof(llong);
}

boolean IsAligned(Origin *const ptr)
{
  return !((uintptr_t)ptr & 7);
}

boolean IsOnHeap(Origin *const inst, void *const ptr)
{
  return inst && (ptr >= (void *)inst->heap.data && ptr <= (void *)&inst->heap.data[__COMPOUND_ORIGIN_HEAP_SIZE_MAXIMUM__ - 1]);
}

static inline boolean IsValidPointerOnHeap(void *const ptr)
{
  return /* IsAligned(ptr) && */IsOnHeap(origin, ptr);
}

boolean IsValidUserPointer(void *const ptr)
{
  /* By verifying the @actual pointer's validity,
   * it is possible to verify @ptr is a valid @user pointer. */
  /* @actual is the next struct memeber of @user. */
  return IsValidPointerOnHeap(ptr) && IsValidPointerOnHeap(((Memory *)ptr)->header.actual);
}

int CountTrailingZeros(register uint32_t value)
{
  if (!value) {
    return 32;
  }

#if defined(__COMPOUND_BY_GCC__) || defined(__COMPOUND_BY_CLANG__)
  return __builtin_ctz(value);
#elif defined(__COMPOUND_BY_MSVC__)
  return __tzcnt(value);
#endif

  register int count = 0;
  while (!(value & 1)) {
    value >>= 1;
    count ++;
  }

  return count;
}

int CountLeadingZeros(register uint32_t value)
{
  if (!value) {
    return 32;
  }

#if defined(__COMPOUND_BY_GCC__) || defined(__COMPOUND_BY_CLANG__)
  return __builtin_clz(value);
#elif defined(__COMPOUND_BY_MSVC__)
  return __lzcnt(value);
#endif

  register int count = 0;
  while (!(value & 0x80000000U)) {
    value <<= 1;
    count ++;
  }

  return count;
}

static inline boolean IsRequirementValid(
  const llong occupations_count,
  const llong requirement
) {
  return requirement >= 0 && requirement <= occupations_count;
}

boolean GetOccupation(
  register uint32_t *const occupations,
  const llong occupations_array_length,
  const llong index
) {
  if (!occupations || index < 0 || index >= occupations_array_length) {
    return false;
  }

  return !!(occupations[index / 32] & (0x80000000U >> (index % 32)));
}

static inline void SetOccupation(
  register uint32_t *const occupations,
  const llong occupations_array_length,
  register const llong index,
  register const boolean truth
) {
  if (!occupations || index < 0 || index >= occupations_array_length) {
    return;
  }

  if (truth) {
    occupations[index / 32] |= (0x80000000U >> (index % 32));
    return;
  }

  occupations[index / 32] &= ~(0x80000000U >> (index % 32));
}

llong CountZerosFromOffsetInOccupations(
  const uint32_t *const occupations,
  const llong occupations_array_length,
  const llong bit_offset,
  const llong requirement
) {
  if (!occupations
   || occupations_array_length <= 0
   || bit_offset < 0
   || bit_offset >= (occupations_array_length * 32)
   || !IsRequirementValid(occupations_array_length, requirement)
  ) {
    return -1;
  }

  const llong index_start = bit_offset / 32;
  const llong index_bit_start = bit_offset % 32;

  llong accum_count = 0;

  uint32_t first_val = occupations[index_start] << index_bit_start;

  if (first_val) {
    accum_count = CountLeadingZeros(first_val);
    char buff[40] = Empty;
    print_long_long_binary(first_val, buff);
    return (requirement > 0 && accum_count >= requirement) ? requirement : accum_count;
  }

  // If first_val is 0, all remaining bits in this element are zeros
  accum_count += (32 - index_bit_start);

  if (requirement > 0 && accum_count >= requirement) {
    return requirement;
  }

  /* Count the subsequential elements' zeros. */
  for (register llong i = index_start + 1; i < occupations_array_length; i++) {
    uint32_t current_val = occupations[i];

    if (current_val) {
      accum_count += CountLeadingZeros(current_val);
      break; // Hit a 1, stop counting
    }

    accum_count += 32;
    if (requirement > 0 && accum_count >= requirement) {
      return requirement;
    }
  }

  return accum_count;
}

static inline llong GetFirstFitOffset(
  uint32_t *const occupations,
  const llong occupations_array_length,
  const llong requirement
) {
  if (!occupations || !occupations_array_length || !IsRequirementValid(occupations_array_length, requirement)) {
    return -1;
  }

  const llong total_bits = occupations_array_length * 32;
  for (register llong i = 0; i < occupations_array_length; i++) {
    if (i < 8) {
      char buff[40] = Empty;
      print_long_long_binary(occupations[i], buff);
    }

    register llong bit_offset = i * 32;
    if (bit_offset + requirement > total_bits) {
      return -1;
    }

    repeat (32) {
      if (bit_offset + requirement > total_bits) {
        return -1;
      }

      /* Skip occupied bits.  GetOccupation returns true if a bit is 1. */
      if (GetOccupation(occupations, occupations_array_length, bit_offset)) {
        bit_offset++;
        continue;
      }

      const llong available = CountZerosFromOffsetInOccupations(occupations, occupations_array_length, bit_offset, requirement);
      // printf("available : requirement = %lld : %lld"NL, available, requirement);
      if (available >= requirement) {
        // printf("CHOSEN bit_offset = %lld"NL, bit_offset);
        return bit_offset;
      }

      bit_offset += available;
    }
  }

  puts("");

  return -1;
}

void *Origin_Allocate(const llong requirement)
{
  if (!requirement) {
    return nll;
  }

  llong heap_offset = GetFirstFitOffset(origin->heap.occupations, __COMPOUND_ORIGIN_HEAP_OCCUPATION_COUNT_MAXIMUM__, requirement);
  if (heap_offset < 0) {
    return nll;
  }

  if (heap_offset < 0 || heap_offset >= __COMPOUND_ORIGIN_HEAP_CHUNK_COUNT__) {
    return nll;
  }

  const llong meta_offset = GetFirstFitOffset(origin->meta.occupations, __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__, 1);
  if (meta_offset < 0) {
    return nll;
  }

  // const llong heap_offset = ((byte *)address_on_heap - origin->heap.data) >> 8;

  /* Origin.meta.occupations. */
  SetOccupation(origin->meta.occupations, __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__, meta_offset, true);

  /* Origin.heap.occupations. */
  loop (i, requirement) {
    SetOccupation(origin->heap.occupations, __COMPOUND_ORIGIN_HEAP_OCCUPATION_COUNT_MAXIMUM__, heap_offset + i, true);
  }

  Memory *const ref = &origin->meta.data[meta_offset];
  ref->header.actual = &origin->heap.data[heap_offset];
  ref->header.user = ref->header.actual;  // @user is initially the same as @actual.
  ref->header.size = requirement;

  ref->index_on_meta = meta_offset;
# ifdef __COMPOUND_FEATURE_RECOLLECTOR__
  ref->pinned = false;
  ref->frame_id = MEMORY_FRAME_ID_NULL;
# endif

  void *address_on_heap = ref->header.user;
  if (!address_on_heap) {
    return nll;
  }

  /* This erasure is necessary because there is no bound check
   * for .data section as in heap -- the garbage data can be manipulated. */
  memset(address_on_heap, 0, requirement);

  return address_on_heap;
}

void Origin_Deallocate(void *address_on_heap)
{
  if (!IsValidPointerOnHeap(address_on_heap)) {
    return;
  }

  Memory *accommodation = nll;
  for (register llong i = 0; i < __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__; i ++) {
    if (origin->meta.data[i].header.actual == address_on_heap) {
      accommodation = &origin->meta.data[i];
      break;
    }
  }

  if (!accommodation) {
    return;
  }

  const llong heap_offset = ((byte *)address_on_heap) - origin->heap.data;
  const llong meta_offset = accommodation->index_on_meta;
  const llong length = accommodation->header.size;

  memset(address_on_heap, 0, length);

  *accommodation = (Memory)Empty;

  /* Origin.heap.occupations. */
  rloop (i, length) {
    SetOccupation(origin->heap.occupations, __COMPOUND_ORIGIN_HEAP_OCCUPATION_COUNT_MAXIMUM__, heap_offset + i, false);
  }

  /* Origin.meta.occupations. */
  SetOccupation(origin->meta.occupations, __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__, meta_offset, false);
}

llong Origin_GetAvailableMetaCount(void)
{
  Memory *it = origin->meta.data;
  register llong count = 0;
  repeat (__COMPOUND_ORIGIN_MEMORY_METADATA_SLOT_MAXIMUM__) {
    count += !!it;
    it ++;
  }

  return count;
}

inline llong Origin_GetAvailableHeapSize(void)
{
  return Origin_GetAvailableMetaCount() * sizeof(origin->meta.data[0]);
}

void InitialiseOrigin(void)
{
  origin = &_origin;
}

void DeinitialiseOrigin(Origin *const inst)
{
  if (!inst) {
    return;
  }

  *origin = (Origin)Empty;
}

void VisualiseMemoryByHeaderOccupation(const char *const title)
{
  const llong meta_capacity = __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__;
  if (meta_capacity <= 0) {
    return;
  }

  printf("=== Header Occupation (%s) ===\n", title ? title : "Global");

  for (register llong i = 0; i < meta_capacity; i++) {
    /* Query the bitfield registry instead of physical memory */
    if (origin->meta.data[i].header.actual) {
      printf("@ ");
    } else {
      printf(". ");
    }

    /* Wrap the grid every 32 slots for readability */
    if ((i + 1) % 32 == 0) printf("\n");
  }
  printf("\n");
}

void VisualiseMemoryByMemoryOccupation(const char *const title)
{
  const llong meta_capacity = __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__;
  if (meta_capacity <= 0) {
    return;
  }

  printf("=== Memory Occupation Summary (%s) ===\n", title ? title : "");
  printf("Capacity: 0x%llX"NL, meta_capacity);

  llong total_used = 0;
  for (register llong i = 0; i < meta_capacity; i++) {
    if (i % (64 * 8) == 0) {
      printf(NL);
    }

    if (i % 64 == 0) {
      printf(NL);
      /* Print index. */
      printf("0x%04llX  ", i);
    }

    if (i % 16 == 0) {
      printf(" ");
    }

    if (i % 8 == 0) {
      printf(" ");
    }

    register Memory *const ref = &origin->meta.data[i];
    if (!ref) {
      printf("E");
      continue;
    }

    if (ref->header.actual) {
      printf("@");
    } else {
      printf(".");
    }
  }

  printf(NL);

  printf("Total Memory Used in Occupied Slots: %lld bytes"NL NL, total_used);
}

void DumpHeap(const char *const title)
{
  register const llong cap = 3 * (2 * 8 * 4 * 8);
  register llong usage = 0;

  printf("=== Heap Occupation Summary (%s) ===\n", title ? title : "");
  printf("Capacity: 0x%llX"NL, __COMPOUND_ORIGIN_HEAP_SIZE_MAXIMUM__);

  for (register llong i = 0; i < cap; i++) {
    if (i % (64 * 8) == 0) {
      printf(NL);
    }

    if (i % 64 == 0) {
      printf(NL);
      /* Print index. */
      printf("0x%08llX  ", (ullong)origin->heap.data + i);
    }

    if (i % 16 == 0) {
      printf(" ");
    }

    if (i % 8 == 0) {
      printf(" ");
    }


    register byte *const ref = &origin->heap.data[i];
    if (!ref) {
      printf("E");
      continue;
    }

    if (isalpha(*ref) || isdigit(*ref) || ispunct(*ref)) {
      printf("%c", *ref);
      usage ++;
      continue;
    }

    printf(".");
  }

  printf(NL);

  printf("Total usage: %lld"NL, usage);
  printf("Available remaining: %lld"NL, (cap - usage));

  printf(NL);
}

void DumpHeapOccupations(void)
{
  loop (i, __COMPOUND_ORIGIN_HEAP_OCCUPATION_COUNT_MAXIMUM__) {
    if (!origin->heap.occupations[i]) {
      continue;
    }

    print_long_long_binary(origin->heap.occupations[i], nll);
  }

  puts("");
}

void DumpMetaOccupations(void)
{
  loop (i, __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__) {
    if (!origin->meta.occupations[i]) {
      continue;
    }

    print_long_long_binary(origin->meta.occupations[i], nll);
  }

  puts("");
}
