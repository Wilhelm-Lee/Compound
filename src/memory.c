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

#include "../inc/memory_stack.h"

struct Memory {
  void *addr;

  /* Garbage Collector marking. */
  boolean marked;
};

Memory *Memory_Create(const void *restrict const addr)
{
  if (!addr) {
    return NULL;
  }

  Memory *const inst =  malloc(sizeof(Memory));
  if (!inst) {
    perror("Failed to allocate memory for Memory registeration.");
    exit(EXIT_FAILURE);
  }

  inst->addr = addr;
  inst->marked = false;

  return inst;
}

Memory *Memory_CopyOf(const Memory *const other)
{
  if (!other) {
    return NULL;
  }

  Memory *const inst = malloc(sizeof(Memory));
  if (!inst) {
    perror("Failed to allocate memory for Memory registeration.");
    exit(EXIT_FAILURE);
  }

  inst->addr = other->addr;
  inst->marked = false;  /* This is intentional. */

  return inst;
}

void Memory_Delete(Memory **const inst)
{
  if (inst && *inst) {
    free(*inst);
    *inst = NULL;
  }
}

inline void *Memory_GetAddr(Memory *inst)
{
  if (!inst) {
    return NULL;
  }

  return inst->addr;
}

inline void *Allocate(const size_t nmemb, const size_t size)
{
  void *const inst = calloc(nmemb, size);
  if (!inst) {
    return NULL;
  }

  /* Register the memory onto the heap stack. */
  ig MemoryStack_Push(MEMORY_STACK, Create(Memory, inst));

  return inst;
}

inline void *Reallocate(void *inst, const size_t size)
{
  if (!inst) {
    return inst;
  };

  inst = realloc(inst, size);

  if (!inst) {
    return inst;
  }

  return inst;
}

inline void _Deallocate(void *const inst)
{
  if (inst) {
    free(inst);
  }
}
