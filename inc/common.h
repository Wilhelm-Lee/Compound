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

/* Evaluate given statement while the ptr to s is not NULL. */
# define svoid(s)  { if (s)  return; }

/* Handling expression with IF statement for @cond as "condition",
   followed by @b as "block". */
# define when(expr, cond, block)                           \
{                                                          \
  const int _ = (expr);                                    \
  if (cond)                                                \
    block                                                  \
}

/* Handling expression returned NON-ZERO. */
# define nzero(expr, block)                                \
{                                                          \
  const int _ = (expr);                                    \
  if (_)                                                   \
    block                                                  \
}

/* Handling expression returned ZERO. */
# define zero(expr, block)                                 \
{                                                          \
  const int _ = (expr);                                    \
  if (!_)                                                  \
    block                                                  \
}

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

/* Return @stat when passing a failing @stat commented with @str. */
# define fails(stat, str)                                  \
    notok(stat, return(annot(_, str));)

/* Return @val when passing a failing @stat. */
# define vfail(stat, val)                                  \
    notok(stat, return(val);)

/* Clone a new variable "v2" with "v1". */
# define clone(v1, v2)                                     \
    __typeof__(v1) v2 = v1;

/* Cast Memory into builtin type in C specified with @type. */
# define cast(var, type)                                   \
    (*(type *)(var).data)

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

# define spush(stk, addr)                                  \
  call(Stack, Pointer, Push) with ((stk), (void *)(addr))

# define spop(stk)                                         \
  call(Stack, Pointer, Pop) with ((stk))

# define push(addr)                                        \
  call(Stack, Pointer, Push) with (GlobalPointerStack, (void *)(addr))

# define pop()                                             \
  call(Stack, Pointer, Pop) with (GlobalPointerStack)

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

/* Replace string with "" once it's NULL. */
# define stremp(s)  (!(s) ? ("") : (s))

/* Converts char string into char pointer. */
# define strptr(s)  ((char *)&s[0])

/* Converts char string into const char pointer. */
# define strcptr(s)  ((const char *)&s[0])

/* Calculate array length.  Doesn't work when @arr is merely a pointer.*/
# define arrlen(arr)  (sizeof(arr) / sizeof(__typeof__(*(arr))))

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

/* Example: wide(NEWLINE) */
# define wide(cstr)                                        \
  L""cstr

/*************************/
/* Status manipulations. */
/*************************/

/* Overwrite @stat.value with @val. */
# define value(stat, val)  ((Status){                      \
  .identity = (stat).identity;                             \
  .value = val;                                            \
  .description = (stat).description;                       \
  .characteristic = (stat).characteristic;                 \
  .location = (stat).location;                             \
  .prev = (stat).prev;                                     \
})

/* Overwrite @stat.location with @__HERE__. */
# define apply(stat)  ((Status){                           \
  .identity = (stat).identity,                             \
  .value = (stat).value,                                   \
  .description = (stat).description,                       \
  .characteristic = (stat).characteristic,                 \
  .location = __HERE__,                                    \
  .prev = (stat).prev,                                     \
})

/* Overwrite @stat.description with @desc. */
# define annot(stat, desc)  ((Status){                     \
  .identity = (stat).identity,                             \
  .value = (stat).value,                                   \
  .description = desc,                                     \
  .characteristic = (stat).characteristic,                 \
  .location = (stat).location,                             \
  .prev = (stat).prev,                                     \
})

/* Create a Report on the fly. */
# define stamp(e, ini)  ((Report) {\
  .content = e,\
  .initiator = ini,\
  .time = time(NULL),\
  .level = REPORT_SENDING_PRIORITY_NORMAL,\
  .status = REPORT_SENDING_TASK_STATUS_PENDING,\
  .dst = stdout\
})

/* Proceed following @block with variadic context described with @fmt. */
# define variadic(fmt, block)                              \
{                                                          \
  va_list ap;                                              \
  va_start(ap, fmt);                                       \
  parse(fmt, block);                                       \
  va_end(ap);                                              \
}

/* Proceed with @cstr. */
# define parse(cstr, block)                                \
{                                                          \
  const char *fmt = (cstr);  ignore fmt;                   \
  boolean _within = false;  ignore _within;                   \
  boolean _once = false;  ignore _once;                       \
  char *_check = NULL;  ignore _check;                     \
  char *_begin = NULL;  ignore _begin;                     \
  char *_end = NULL;  ignore _end;                         \
  const char *app = fmt;  /*"app" stands for "ap pointer"*/\
  for (; *app; app++)  block                               \
}

/* Current character during parsing. */
# define CURRENT  (app ? *app : 0)

# define STEP  ((app - fmt) / (sizeof(char)))

/* Setup a checkpoint/stub for pointer traveling. */
# define stub  _check = (char *)app

/* Travel back to an existing checkpoint and tear it down. */
# define back  if (_check) { app = _check;  unstub; }  

/* Tear down an existing checkpoint. */
# define unstub  _check = NULL

/* Within every variadic context, execute @block for once on initial. */
# define init(block)                                       \
{                                                          \
  if (!_once) {                                            \
    block                                                  \
    _once = true;                                          \
  }                                                        \
}

/* Within every variadic context, propose solutions for @opt, typed with @type. */
# define option(opt, type, block)                          \
{                                                          \
  if (*(app) == (opt)) {                                   \
    const type _ = va_arg(ap, type);  block                \
  }                                                        \
}

/* Within every variadic context, propose solutions for @opt. */
# define at(opt, block)                                    \
{                                                          \
  if (*(app) == (opt))  block                              \
}

# define again                                             \
{                                                          \
  app = ((app > fmt) ? (app - 1) : (fmt));                 \
  continue;                                                \
}

/* Within every variadic context, select given prefix @opt
   before proposing options. */
# define prefix(opt, block)                                \
{                                                          \
  if (_within)  block                                      \
  if (*(app) == (opt))  { app+=1;  block }                 \
}

// Redundant with @from, @region.
# define within(opt1, opt2, before, block, after)          \
{                                                          \
  if (*(app) == (opt1)) {                                  \
    before                                                 \
    _within = true;                                        \
    continue;                                              \
  }                                                        \
                                                           \
  if (_within && *(app) != (opt2))  block                  \
                                                           \
  if (*(app) == (opt2)) {                                  \
    _within = false;                                       \
    after                                                  \
  }                                                        \
}

/* Within every variadic context, ignore everything until
   @opt is met. */
# define until(opt)                                        \
    if (*(app) != (opt))  continue;

/* Within every variadic context, perform @block till @opt
   is met. */
# define till(opt, block)                                  \
    if (*(app) != (opt))  block

// Untested.
/* Within every variadic context, perform @block till @opt
   is met.  And return back in place. */
# define presight(opt, block)                              \
{                                                          \
  stub;                                                    \
  till(opt, block)                                         \
  back;                                                    \
}

/* Within every variadic context, go to region and operate. */
# define region(opt1, opt2, block)                         \
{                                                          \
  at(opt1, till(opt2, block););                            \
}

// Untested.
/* Within every variadic context, operate in distance ranged
   in a nearest region.  And return back after operation. */
# define remote(opt1, opt2, block)                         \
{                                                          \
  stub;                                                    \
  region(opt1, opt2, block);                               \
  back;                                                    \
}

// Malfunctioning.
# define distance_of(opt1, opt2)                           \
({                                                         \
  within(opt1, opt2, {                                     \
    stub;                                                  \
    _begin = (char *)app;                                  \
  }, {}, {                                                 \
    _end = (char *)app;                                    \
    back;                                                  \
  })                                                       \
  (_end - _begin) - 1;                                     \
})

typedef struct {
  int level;
  char opt_enter;
  char opt_leave;
} Module;

# define module(id, opt_enter, opt_leave)                  \
  Module id = {                                            \
    0, opt_enter, opt_leave                                \
  }

# define enter(module, block)                              \
  at((module).opt_enter, {                                 \
    (module).level++;                                      \
    block                                                  \
  })

# define leave(module, block)                              \
  at((module).opt_leave, {                                 \
    (module).level--;                                      \
    block                                                  \
  })

# define update(module)                                    \
  enter(module, app++;);                                   \
  leave(module, app++;);

# define inside(module)                                    \
  ((module).level)

# define outside(module)                                   \
  (!(module).level)

/* Within every variadic context, skip given character @opt without processing. */
# define skip(opt)  if (*(app) == (opt)) continue;

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

# define ref(inst, type, index)                            \
  ({                                                       \
    type *_ref = NULL;                                     \
    ig call(Array, type, RefIdx) with (inst, (index), &_ref);\
    _ref;                                                  \
  })

# define get(inst, type, index)                            \
  ({                                                       \
    type _get = EMPTY;                                     \
    ig call(Array, type, GetIdx) with (inst, (index), &_get);\
    _get;                                                  \
  })

# define set(inst, type, index, elem)                      \
  call(Array, type, SetIdx) with ((inst), (index), (elem)) \

# define iterate(idx, collection)                          \
  for (register __typeof__((collection).length) idx = 0;   \
       idx < (collection).length; idx++)

# define foreach(type, it, collection, block)              \
  {                                                        \
    type it = EMPTY;                                       \
    iterate (_foreach_idx, (collection)) {                 \
      it = get((collection), type, _foreach_idx);          \
      block                                                \
    }                                                      \
  }

# define stringing(it, string, block)                      \
  foreach (byte, it, (string).data, block)

# define last(collection)                                  \
  ((collection).length)

# define max(a, b)                                         \
  (a <= b ? a : b)

#endif /* COMPOUND_COMMON_H */
