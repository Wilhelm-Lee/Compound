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

/*
  (so you should pass 'int' not 'byte' {aka 'unsigned char'} to 'va_arg')
  'float' -> 'double'
  'char' -> 'int'
  'short' -> 'int'
  'boolean' -> 'int'
  'int8_t' -> 'int'
  'int16_t' -> 'int'
  'int_fast8_t' -> 'int'
  'uchar' -> 'int'
  'ushort' -> 'int'
  'uint8_t' -> 'int'
  'uint16_t' -> 'int'
  'uint_fast8_t' -> 'int'
*/

ARRAY_BASICTYPE(ptr);
ARRAY_BASICTYPE(byte);
ARRAY_BASICTYPE(boolean);
ARRAY_BASICTYPE(float);
ARRAY_BASICTYPE(double);
ARRAY_BASICTYPE(char);
ARRAY_BASICTYPE(short);
ARRAY_BASICTYPE(int);
ARRAY_BASICTYPE(long);
ARRAY_BASICTYPE(llong);
ARRAY_BASICTYPE(int8_t);
ARRAY_BASICTYPE(int16_t);
ARRAY_BASICTYPE(int32_t);
ARRAY_BASICTYPE(int64_t);
ARRAY_BASICTYPE(int_fast8_t);
ARRAY_BASICTYPE(int_fast16_t);
ARRAY_BASICTYPE(int_fast32_t);
ARRAY_BASICTYPE(int_fast64_t);
ARRAY_BASICTYPE(intptr_t);
ARRAY_BASICTYPE(intmax_t);
ARRAY_BASICTYPE(uchar);
ARRAY_BASICTYPE(ushort);
ARRAY_BASICTYPE(uint);
ARRAY_BASICTYPE(ulong);
ARRAY_BASICTYPE(ullong);
ARRAY_BASICTYPE(uint8_t);
ARRAY_BASICTYPE(uint16_t);
ARRAY_BASICTYPE(uint32_t);
ARRAY_BASICTYPE(uint64_t);
ARRAY_BASICTYPE(uint_fast8_t);
ARRAY_BASICTYPE(uint_fast16_t);
ARRAY_BASICTYPE(uint_fast32_t);
ARRAY_BASICTYPE(uint_fast64_t);
ARRAY_BASICTYPE(uintptr_t);
ARRAY_BASICTYPE(uintmax_t);
ARRAY_BASICTYPE(size_t);

/* For inline-ment of @compose:
   e.g.
     Array(ptr) *arrays = compose(Array, array(...), array(...), ...)
 */
typedef ptrArray Array;

#endif  /* COMPOUND_ARRAYS_H */
