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

/** @file char.h */

#ifndef COMPOUND_CHAR_H
# define COMPOUND_CHAR_H

# include "arrays.h"
# include "endian.h"
# include "types.h"

typedef struct Char Char;

/* Construct a Char instance with @bytes, a sequence of bytes. */
# define chars(byte_ptr)\
  (Create(Char, byte_ptr))

ARRAY(Char)

Char *Char_Create(const byte *restrict const data);
Char *Char_CopyOf(const Char *const other);
void Char_Delete(Char **const inst);
boolean Char_Equals(const Char *const inst, const Char *const other);

Array(byte) *Char_GetData(const Char *const inst);
llong Char_GetByteCount(const Char *const inst);

#endif  /* COMPOUND_CHAR_H */
