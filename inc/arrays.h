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

/** @file arrays.h */

#ifndef COMPOUND_ARRAYS_H
# define COMPOUND_ARRAYS_H

# include "array.h"

ARRAY_BASICTYPE_TYPE(ptr, void *)
ARRAY_BASICTYPE_TYPE(byte, int)
ARRAY_BASICTYPE_TYPE(boolean, int)
ARRAY_BASICTYPE_TYPE(float, double)
ARRAY_BASICTYPE(double)
ARRAY_BASICTYPE_TYPE(char, int)
ARRAY_BASICTYPE_TYPE(short, int)
ARRAY_BASICTYPE(int)
ARRAY_BASICTYPE(long)
ARRAY_BASICTYPE(llong)
ARRAY_BASICTYPE_TYPE(int8_t, int)
ARRAY_BASICTYPE_TYPE(int16_t, int)
ARRAY_BASICTYPE(int32_t)
ARRAY_BASICTYPE(int64_t)
ARRAY_BASICTYPE_TYPE(int_fast8_t, int)
ARRAY_BASICTYPE(int_fast16_t)
ARRAY_BASICTYPE(int_fast32_t)
ARRAY_BASICTYPE(int_fast64_t)
ARRAY_BASICTYPE(intptr_t)
ARRAY_BASICTYPE(intmax_t)
ARRAY_BASICTYPE_TYPE(uchar, int)
ARRAY_BASICTYPE_TYPE(ushort, int)
ARRAY_BASICTYPE(uint)
ARRAY_BASICTYPE(ulong)
ARRAY_BASICTYPE(ullong)
ARRAY_BASICTYPE_TYPE(uint8_t, int)
ARRAY_BASICTYPE_TYPE(uint16_t, int)
ARRAY_BASICTYPE(uint32_t)
ARRAY_BASICTYPE(uint64_t)
ARRAY_BASICTYPE_TYPE(uint_fast8_t, int)
ARRAY_BASICTYPE(uint_fast16_t)
ARRAY_BASICTYPE(uint_fast32_t)
ARRAY_BASICTYPE(uint_fast64_t)
ARRAY_BASICTYPE(uintptr_t)
ARRAY_BASICTYPE(uintmax_t)
ARRAY_BASICTYPE(size_t)

/* For inline-ment of @compose:
   e.g.
     Array(ptr) *arrays = compose(Array, array(...), array(...), ...)
 */
typedef Array(ptr) Array;

#endif  /* COMPOUND_ARRAYS_H */
