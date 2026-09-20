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
# include "memory_internal.h"
# include "platform.h"
# include "types.h"

# define MEMORY_STACK_HEIGHT_MAXIMUM  (1024LL)

typedef struct MemoryStack MemoryStack;

void InitialiseMemoryStack(void);
void DeinitialiseMemoryStack(void);

/* Returns the registered indexer in @MEMORY_STACK; -1 for failures. */
llong MemoryStack_Push(MemoryStack *const inst, void *const addr);
void MemoryStack_Pop(MemoryStack *const inst);

/* Acts as a assertive termination to clean up
 * the data stored previously to start a new session. */
void MemoryStack_PopAll(MemoryStack *const inst);
Memory *MemoryStack_Top(MemoryStack *const inst);
boolean MemoryStack_IsEmpty(MemoryStack *const inst);
boolean MemoryStack_IsFull(MemoryStack *const inst);

/* Returns -1 either when @inst is null
 * or when @inst the stack is empty. */
llong MemoryStack_GetHeight(MemoryStack *const inst);
/* Returns null when out-of-bound or beyond current stack height. */
void *MemoryStack_GetAddress(MemoryStack *const inst, const llong idx);

#endif /* COMPOUND_MEMORY_STACK_H */
