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

/** @file memory_stack.h */

#ifndef COMPOUND_MEMORY_STACK_H
# define COMPOUND_MEMORY_STACK_H

# include <stdio.h>

# include "common.h"
# include "language.h"
# include "location.h"
# include "platform.h"

typedef struct Memory Memory;

typedef struct MemoryStack {
  void **data;
  llong capacity;  // The total capacity of the instance.
  llong height;  // The current indexer of the instance.
} MemoryStack;

void *Allocate(const size_t nmemb, const size_t size);
void _Deallocate(void *const inst);

# ifdef __COMPOUND_FEATURE_RECYCLER__
#  define Deallocate(inst)
# else
#  define Deallocate(inst)                                                     \
  _Deallocate(inst)
# endif

/**
 * @return The registered indexer in @MEMORY_STACK if succeeded.
 *         -1 when failed.
 */
llong MemoryStack_Push(MemoryStack *const inst, void *const addr);
void MemoryStack_Pop(MemoryStack *const inst);
void *MemoryStack_Top(MemoryStack *const inst);

/* Returns -1 either when @inst is null
 * or when @inst the stack is empty. */
llong MemoryStack_GetHeight(MemoryStack *const inst);

boolean MemoryStack_IsEmpty(MemoryStack *const inst);
boolean MemoryStack_IsFull(MemoryStack *const inst);

#endif /* COMPOUND_MEMORY_STACK_H */
