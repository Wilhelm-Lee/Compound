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

/** @file memory.h */

#ifndef COMPOUND_MEMORY_H
# define COMPOUND_MEMORY_H

# include <stdlib.h>

# include "language.h"
# include "types.h"
# include "location.h"

typedef struct {
  /* The location where the address got allocated. */
  Location allocation;
  /* The latest location where the address got reallocated. */
  Location reallocation;
  /* The location where the address got freed. */
  Location freeing;
  void *data;
  size_t size;
  boolean has_reallocated;
  boolean has_freed;
} Memory;

# define _memory_get_freeing(addr)\
  ((void *)(addr - sizeof(Location)))

# define _memory_get_reallocation(addr)\
  ((void *)_memory_get_freeing(addr) - sizeof(Location))

# define _memory_get_allocation(addr)\
  ((void *)_memory_get_reallocation(addr) - sizeof(Location))

# define _memory_get_size(addr)\
  ((void *)(addr + sizeof(void *)))

# define _memory_get_has_reallocated(addr)\
  ((void *)_memory_get_size(addr) + sizeof(size_t))

# define _memory_get_has_freed(addr)\
  ((void *)_memory_get_has_reallocated(addr) + sizeof(boolean))

typedef struct {
  Memory *data;
  llong capacity;  //< The total capacity of the instance.
  llong height;  //< The current indexer of the instance.
} MemoryStack;

void MemoryStack_Create(MemoryStack *const inst, const llong capacity);
void MemoryStack_Delete(MemoryStack *const inst);
void MemoryStack_Push(MemoryStack *const inst, const Memory push);
void MemoryStack_Pop(MemoryStack *const inst);
void MemoryStack_PopAll(MemoryStack *const inst);
Memory *MemoryStack_Top(MemoryStack *const inst);
Memory *MemoryStack_At(MemoryStack *const inst, const llong index);

# define MEMORY_STACK_HEIGHT_MAXIMUM  256LL

extern MemoryStack *MEMORY_STACK;
/* Keeps track of every Memory instances in @MEMORY_STACK. */
extern llong *MEMORY_STACK_INDEXERS;

# define M  ((MemoryStack_Top(MEMORY_STACK)))

# define mpush(memory)\
  MemoryStack_Push(MEMORY_STACK, (memory))

# define mpop\
  MemoryStack_Pop(MEMORY_STACK)

# define mpopall\
  MemoryStack_Pop(MEMORY_STACK)

void *Allocate(const size_t nmemb, const size_t size);

void *Reallocate(void *inst, const size_t size);

void Deallocate(void *const inst);


// void *_Allocate(const size_t nmemb, const size_t size,
//                 const char *restrict const file,
//                 const char *restrict const func,
//                 const int line);

// # define Allocate(nmemb, size)\
//   _Allocate(nmemb, size, __FILE__, __func__, __LINE__)

// void *_Reallocate(Memory *const inst, const size_t size,
//                   const char *restrict const file,
//                   const char *restrict const func,
//                   const int line);

// # define Reallocate(inst_ptr, size)\
//   _Reallocate(inst_ptr, size, __FILE__, __func__, __LINE__)

// void Deallocate(void *const *inst);

#endif  /* COMPOUND_MEMORY_H */
