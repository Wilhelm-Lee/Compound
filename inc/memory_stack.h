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

# include "common.h"
# include "language.h"
# include "memory.h"
# include "platform.h"

# define __COMPOUND_MEMORY_STACK_HEIGHT_MAXIMUM__  32768LL

typedef struct MemoryStack MemoryStack;

extern MemoryStack *MEMORY_STACK;

/**
 * @return The registered indexer in @MEMORY_STACK if succeeded.
 *         -1 when failed.
 */
llong MemoryStack_Push(MemoryStack *const inst, Memory *const memory);
void MemoryStack_Pop(MemoryStack *const inst);
Memory *MemoryStack_Top(MemoryStack *const inst);

/* Returns -1 either when @inst is NULL
 * or when @inst the stack is empty. */
llong MemoryStack_GetHeight(MemoryStack *const inst);

boolean MemoryStack_IsEmpty(MemoryStack *const inst);
boolean MemoryStack_IsFull(MemoryStack *const inst);

void *Allocate(const size_t nmemb, const size_t size);
void Deallocate(void *const inst);

#endif /* COMPOUND_MEMORY_STACK_H */
