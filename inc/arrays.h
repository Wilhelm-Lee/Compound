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
# include "types.h"

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

ARRAY(ptr);
ARRAY(byte);
ARRAY(boolean);
ARRAY(float);
ARRAY(double);
ARRAY(char);
ARRAY(short);
ARRAY(int);
ARRAY(long);
ARRAY(llong);
ARRAY(int8_t);
ARRAY(int16_t);
ARRAY(int32_t);
ARRAY(int64_t);
ARRAY(int_fast8_t);
ARRAY(int_fast16_t);
ARRAY(int_fast32_t);
ARRAY(int_fast64_t);
ARRAY(intptr_t);
ARRAY(intmax_t);
ARRAY(uchar);
ARRAY(ushort);
ARRAY(uint);
ARRAY(ulong);
ARRAY(ullong);
ARRAY(uint8_t);
ARRAY(uint16_t);
ARRAY(uint32_t);
ARRAY(uint64_t);
ARRAY(uint_fast8_t);
ARRAY(uint_fast16_t);
ARRAY(uint_fast32_t);
ARRAY(uint_fast64_t);
ARRAY(uintptr_t);
ARRAY(uintmax_t);
ARRAY(size_t);

typedef ptrArray Array;

#endif  /* COMPOUND_ARRAYS_H */
