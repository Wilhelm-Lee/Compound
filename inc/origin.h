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

/** @file origin.h */

#ifndef COMPOUND_ORIGIN_H
# define COMPOUND_ORIGIN_H

# ifdef __COMPOUND_BY_MSVC__
#  include <intrin.h>  // For __tzcnt, __lzcnt.
# endif

# include <ctype.h>

# include "memory.h"
# include "memory_internal.h"
# include "memory_stack.h"
# include "types.h"

# define __COMPOUND_ORIGIN_PROFILE_ESTABLISHED__

# if defined (__COMPOUND_ORIGIN_PROFILE_DELICATE__)
#  define __COMPOUND_ORIGIN_HEAP_CHUNK_COUNT__              128        //  4 KiB
#  define __COMPOUND_ORIGIN_META_SLOT_MAXIMUM__    4
# elif defined (__COMPOUND_ORIGIN_PROFILE_ESTABLISHED__)
#  define __COMPOUND_ORIGIN_HEAP_CHUNK_COUNT__              ( 16 KiB)  //512 KiB
#  define __COMPOUND_ORIGIN_META_SLOT_MAXIMUM__             (  8 KiB)
# elif defined (__COMPOUND_ORIGIN_PROFILE_CLIMB__)
#  define __COMPOUND_ORIGIN_HEAP_CHUNK_COUNT__              (  1 MiB)  // 32 MiB
#  define __COMPOUND_ORIGIN_META_SLOT_MAXIMUM__             ( 64 KiB)
# elif defined (__COMPOUND_ORIGIN_PROFILE_COMPETENT__)
#  define __COMPOUND_ORIGIN_HEAP_CHUNK_COUNT__              ( 32 MiB)  //  1 GiB
#  define __COMPOUND_ORIGIN_META_SLOT_MAXIMUM__             (512 KiB)
# else
#  define __COMPOUND_ORIGIN_HEAP_CHUNK_COUNT__              (  1 KiB)  // 32 KiB
#  define __COMPOUND_ORIGIN_META_SLOT_MAXIMUM__             (  1 KiB)
# endif


/* Since __COMPOUND_ORIGIN_HEAP_SIZE_MAXIMUM__ must be a multiple of 32,
 * this provides the base multiplier for it.
 */
# define __COMPOUND_ORIGIN_HEAP_PER_CHUNK_SIZE__  32

/* Must be a multiple of 32; uint32_t is used for representing bytes on heap. */
# define __COMPOUND_ORIGIN_HEAP_SIZE_MAXIMUM__\
  (__COMPOUND_ORIGIN_HEAP_PER_CHUNK_SIZE__*__COMPOUND_ORIGIN_HEAP_CHUNK_COUNT__)

# define __COMPOUND_ORIGIN_META_OCCUPATION_COUNT_MAXIMUM__\
  ((__COMPOUND_ORIGIN_META_SLOT_MAXIMUM__ + 31) / 32)

# define __COMPOUND_ORIGIN_HEAP_OCCUPATION_COUNT_MAXIMUM__\
  ((__COMPOUND_ORIGIN_HEAP_SIZE_MAXIMUM__ + 31) / 32)

typedef struct Origin Origin;

extern Origin *origin;
extern void Recollector_Recollect(void);

void InitialiseOrigin(void);
void DeinitialiseOrigin(void);

Origin *Origin_Create(void);
Origin *Origin_CopyOf(Origin *const other);
void Origin_Delete(void);

void *Origin_Allocate(const llong requirement);
void Origin_Deallocate(void *address_on_heap);

llong Origin_GetUsedMetaCount(void);
llong Origin_GetUsedHeapSize(void);
llong Origin_GetAvailableMetaCount(void);
llong Origin_GetAvailableHeapSize(void);

// /* Returns the total available size in bytes on heap.
//  *         -1 to indicate errors.
//  */
// llong Origin_GetAvailableHeapSize(Origin *const inst);
// /* Returns the total available count of slots of Memory struct in @meta.
//  *         -1 to indicate errors.
//  */
// llong Origin_GetAvailableMetaCount(Origin *const inst);

void VisualiseMemoryByMemoryOccupation(const char *const subject);
void VisualiseMemoryByHeaderOccupation(const char *const subject);
void DumpHeap(const char *const title);
void DumpHeapOccupations(void);
void DumpMetaOccupations(void);

#endif  /* COMPOUND_ORIGIN_H */
