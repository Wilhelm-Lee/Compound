/*
 * This file is part of Compound library.
 * Copyright (C) 2024-TODAY  William Lee
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

#ifndef COMPOUND_COMMON_H
# define COMPOUND_COMMON_H

# include <stdlib.h>

# include "types.h"

/********************************/
/* Status & Returning Handling. */
/********************************/

/* Get the literal. */
# define nameof(obj)                                       \
  #obj

/* Return @n as the return value, once @o is NULL. */
# define nonull(o, n)                                      \
  if (!(o))                                                \
    return (n);

/* Shortcut for returning UnavailableObject. */
# define avail(o)                                          \
  nonull((o), apply(annot(UnavailableObject,               \
                          "\""nameof(o)"\""" was unavailable.")))

/* Evaluate given statement while the ptr to s is not NULL. */
# define state(expr, stat)  { if (expr)  RETURN(stat); }

/* Execute @block whenever finds @stat is "NOT okay". */
# define notok(stat, block)                                \
{                                                          \
  const Status _ = stat;                                   \
  if (!Status_IsOkay(_))                              \
    block                                                  \
}

/* Return @stat when passing a failing @stat. */
# define fail(stat)                                        \
    notok(stat, RETURN(_);)

/* Useful on handling Status errors in a function returns int. */
# define quit_on_fail(s)                                   \
{                                                          \
  notok(s, {                                               \
    PrintStatusDump(_);                                    \
    return _.value;                                        \
  })                                                       \
}

/* For handling Status at one place. */
# define handle(status, block)                             \
{                                                          \
  Status _ = (status);                                     \
  block                                                    \
}

/* For differentiate handling of possible Status. */
# define match(status, block)                              \
{                                                          \
  if (Status_Match((_), (status)))                         \
    block                                                  \
}

/* Detect current Status is based on @status from root. */
# define belong(status, block)                             \
{                                                          \
  if (Status_Belong((_), (status)))                        \
    block                                                  \
}

/* The inverting of "belong". */
#define nbelong(status, block)                             \
{                                                          \
  if (!Status_Belong((_), (status)))                       \
    block                                                  \
}

// /* Push current Status into Stack and return with it. */
// # define RETURN(status)                                    \
// {                                                          \
//   handle((apply(status)), {                                \
//     call(Stack, Status, Push) with (GlobalStatusStack, _); \
//     return (_);                                            \
//   })                                                       \
// }

# define RETURN(status)                                    \
  return ((!strcmp((status).location.func, "(GLOBAL)")) ? (apply(status)) : (status));

/* Replace string with "(null)" once it's NULL. */
# define strnil(s)  (!(s) ? ("(null)") : (s))

/* Converts char string into char pointer. */
# define strptr(s)  ((char *)&s[0])

/* Jump to destructor when:
     A. Reached the end of this scope.
     B. Hit "break" keyword. */
# define local(construct, destruct, block)                 \
  {                                                        \
    construct;                                             \
    do block while (0);                                    \
    destruct;  /* Direct destruction. */                   \
  }

/* Calculate file content size. */
# define fsize(fp)                                         \
  ({fseek((fp), 0, SEEK_END);                              \
   size_t _fsize_sz = ftell(fp); rewind(fp); (_fsize_sz);})

/* Converts calling function with compatible Incompleted objects. */
# define call(incompleted, type, operation)                \
  type##incompleted##_##operation

# define with(...)                                         \
  (__VA_ARGS__)

/******************/
/* Miscellaneous. */
/******************/

# define EMPTY  {0}

# define INRANGE(lf, inclf, rt, incrt, v)                  \
  ((inclf ? (v >= lf) : (v > lf)) && (incrt ? (v <= rt) : (v < rt)))

# define ATRANGE(lf, rt, v)                                \
  (INRANGE(lf, true, rt, true, v) ? 0 : (v < lf ? (v - lf) : (v - rt)))

# define IDENTITY_LENGTH_MAXIMUM  64

# define ELLIPSIS  const char *fmt, ...

# define ignore  (void)

# define ig  ignore

# define neg(number)  ((number) * (-1))

# define pos(number)  ((number) < 0 ? neg(number) : (number))

# define max(a, b)                                         \
  (a <= b ? a : b)

#endif /* COMPOUND_COMMON_H */
