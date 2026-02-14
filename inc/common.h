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

/** @file common.h */

#ifndef COMPOUND_COMMON_H
# define COMPOUND_COMMON_H

# include <limits.h>

# include "types.h"

# define _CONCAT(a, b)\
  a##b

# define CONCAT(a, b)\
  _CONCAT(a, b)

# define loop(it, times)\
  for (register llong it = 0; it < (times); it++)

# define rloop(it, times)\
  for (register llong it = times - 1; it >= 0; it--)

# define repeat(times)\
  loop(CONCAT(_repeat_, __COUNTER__), times)

/* Provides the "quoted" version for literal value of @obj. */
# define nameof(obj)\
  #obj

/* Types incompleted objects with their operation for calling. */
# define call(incompleted, type, operation)\
  type##incompleted##_##operation

# define with

# define EMPTY  {0}

# define ignore  (void)
# define ig  ignore

# define Create(type, ...)\
  call(type,, Create) with (__VA_ARGS__)

# define CopyOf(type, ...)\
  call(type,, CopyOf) with (__VA_ARGS__)

# define Delete(type, ...)\
  call(type,, Delete) with (__VA_ARGS__)

# define Equals(type, ...)\
  call(type,, Equals) with (__VA_ARGS__)

#endif /* COMPOUND_COMMON_H */
