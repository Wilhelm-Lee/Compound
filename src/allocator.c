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

/** @file allocator.c */

#include "../inc/allocator.h"
#include "../inc/memory_stack.h"
// #include "../inc/recollector.h"

extern MemoryStack MEMORY_STACK;

/* Returns the address stored in @MEMORY_STACK. */
inline void *_Allocate(const size_t nmemb, const size_t size)
{
  void *const inst = calloc(nmemb, size);
  if (!inst && (nmemb && size)) {
    return null;
  }

# ifdef __COMPOUND_FEATURE_RECYCLER__
  MemoryStack_Push(&MEMORY_STACK, inst);
# endif

  return inst;
}

void _Deallocate(void *const ptr)
{
  if (ptr) {
    free(ptr);
  }
}
