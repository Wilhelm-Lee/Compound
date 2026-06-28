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

#include "../inc/memory.h"

struct Memory
{
  void *addr;

  /* Submitted size for allocation in bytes. */
  size_t size;

#ifdef __COMPOUND_ALLOW_BACKTRACING__
  Location *allocation;
  Location *deallocation;
#endif

#ifdef __COMPOUND_ALLOW_RECOLLECTOR__
  boolean is_released_automatically;
  boolean is_garbage_collection_marked;

  /* If given with a custom Destructor,
     an execution of it will take place before calling for @Deallocate.

     Tip: resetting @addr to NULL within @Destructor to fully charge
          the handling without releasing after it since @Deallocate
          skips NULL. */
  void (*Destructor)(void);
#endif
};

extern inline Memory *Memory_Create(void *const addr, const size_t size)
{
  Memory *inst = calloc(1, sizeof(Memory));
  if (!inst) {
    return NULL;
  }

  inst->addr = addr;
  inst->size = size;

#ifdef __COMPOUND_ALLOW_BACKTRACING__
  inst->allocation = __builtin_frame_address(1);
  inst->deallocation = NULL;
#endif

  return inst;
}

extern inline Memory *Memory_CopyOf(const Memory *const other)
{
  if (!other) {
    return NULL;
  }

  Memory *inst = calloc(1, sizeof(Memory));
  if (!inst) {
    return NULL;
  }

  inst->addr = other->addr;
  inst->size = other->size;

#ifdef __COMPOUND_ALLOW_BACKTRACING__
  inst->allocation = __builtin_frame_address(1);
#endif

  return inst;
}

extern inline void Memory_Delete(Memory **const inst)
{
  if (!inst) {
    return;
  }

  if (*inst) {
    free((*inst)->addr);
    (*inst)->addr = NULL;
    (*inst)->size = 0;
    free(*inst);
    *inst = NULL;
  }
}

extern inline void *Memory_GetAddress(const Memory *const inst)
{
  if (!inst) {
    return NULL;
  }

  return inst->addr;
}

extern inline size_t Memory_GetSize(const Memory *const inst)
{
  if (!inst) {
    return 0;
  }

  return inst->size;
}
