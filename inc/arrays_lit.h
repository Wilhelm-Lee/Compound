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

/** @file arrays_lit.h */

#ifndef COMPOUND_ARRAYS_LIT_H
# define COMPOUND_ARRAYS_LIT_H

# include <inttypes.h>

# include "string.h"
# include "literalise.h"

FUNC_ARRAY_BASICTYPE_LITERALISE(boolean)
FUNC_ARRAY_BASICTYPE_LITERALISE(byte)
FUNC_ARRAY_BASICTYPE_LITERALISE(char)
FUNC_ARRAY_BASICTYPE_LITERALISE(double)
FUNC_ARRAY_BASICTYPE_LITERALISE(float)
FUNC_ARRAY_BASICTYPE_LITERALISE(int)
FUNC_ARRAY_BASICTYPE_LITERALISE(int16_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(int32_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(int64_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(int8_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(int_fast16_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(int_fast32_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(int_fast64_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(int_fast8_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(intmax_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(intptr_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(llong)
FUNC_ARRAY_BASICTYPE_LITERALISE(long)
FUNC_ARRAY_BASICTYPE_LITERALISE(ptr)
FUNC_ARRAY_BASICTYPE_LITERALISE(short)
FUNC_ARRAY_BASICTYPE_LITERALISE(size_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uchar)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint16_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint32_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint64_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint8_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint_fast16_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint_fast32_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint_fast64_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uint_fast8_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uintmax_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(uintptr_t)
FUNC_ARRAY_BASICTYPE_LITERALISE(ullong)
FUNC_ARRAY_BASICTYPE_LITERALISE(ulong)
FUNC_ARRAY_BASICTYPE_LITERALISE(ushort)

#endif  /* COMPOUND_ARRAYS_LIT_H */
