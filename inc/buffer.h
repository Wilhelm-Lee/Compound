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

/** @file buffer.h */

#ifndef COMPOUND_BUFFER_H
# define COMPOUND_BUFFER_H

# include "arrays.h"
# include "string.h"

typedef struct Buffer Buffer;

ARRAY(Buffer)
LITERALISE(Buffer)

Buffer *Buffer_Create(const llong capacity);
Buffer *Buffer_CopyOf(const Buffer *const other);
void Buffer_Delete(Buffer *const inst);
boolean Buffer_Equals(const Buffer *const obj1, const Buffer *const obj2);

boolean Buffer_Sample(Buffer *const inst, const String *const string);
void Buffer_Rewind(Buffer *const inst);
void Buffer_Clear(Buffer *const inst);

/* Appending dynamically scales using the grow() macro when full. */
boolean Buffer_Append(Buffer *const inst, const byte value);
boolean Buffer_Write(Buffer *const inst, const String *const value);

Array(byte) *Buffer_GetData(const Buffer *const inst);
llong Buffer_GetCursor(const Buffer *const inst);

#endif /* COMPOUND_BUFFER_H */
