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

# include <stdio.h>
# include <stdlib.h>

// TODO(william): change with "hashmap" after importing the lib.
// # include <Compound/hashmap.h>

# include "common.h"
# include "language.h"
# include "types.h"
# include "memory_common.h"

typedef struct Memory Memory;

Memory *Memory_Create(const void *const addr, const size_t size);
Memory *Memory_CopyOf(const Memory *const other);
void Memory_Delete(Memory *const inst);

void *Memory_GetAddress(const Memory *const inst);
size_t Memory_GetSize(const Memory *const inst);

#endif  /* COMPOUND_MEMORY_H */
