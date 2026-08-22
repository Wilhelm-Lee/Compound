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
# include "quote.h"

# define EMPTY  {0}

# define ignore  (void)
# define ig  ignore

# define null  NULL

# define nop  ((void)0)

# define elif  else if

# define loop(it, times)                                                       \
  for (register llong it = 0; it < (times); it++)

# define rloop(it, times)                                                      \
  for (register llong it = times - 1; it >= 0; it--)

# define repeat(times)                                                         \
  loop(CONCAT(_repeat_, __COUNTER__), times)

/* Types incompleted objects with their @Operation for calling. */
# define call(type, Operation, ...)                                            \
  (CONCAT(CONCAT(type, _), Operation)(__VA_ARGS__))

# define Create(type, ...)                                                     \
  (call(type, Create, __VA_ARGS__))

# define CopyOf(type, ...)                                                     \
  (call(type, CopyOf, __VA_ARGS__))

# define Delete(type, ...)                                                     \
  (call(type, Delete, __VA_ARGS__))

# define Equals(type, ...)                                                     \
  (call(type, Equals, __VA_ARGS__))

# define Recreate(type, ...)                                                    \
  (call(type, Recreate, __VA_ARGS__))

# define Getter(type, Member, inst)                                            \
  (call(type, Get##Member, inst))

# define Setter(type, Member, inst, value)                                     \
  (call(type, Set##Member, inst, value))

# define _Getter(type, Member, inst)                                           \
  (Getter(CONCAT(_, type), Member, inst))

# define _Setter(type, Member, inst)                                           \
  (Setter(CONCAT(_, type), Member, inst))

# define Compose(type, ...)                                                    \
  (call(type, Compose, arglen(__VA_ARGS__), __VA_ARGS__))

# define ComposeFromCstr(type, ...)                                            \
  (call(type, ComposeFromCstr, arglen(__VA_ARGS__), __VA_ARGS__))

# define Length(type, inst)                                                    \
  (call(type, Length, inst))

#endif /* COMPOUND_COMMON_H */
