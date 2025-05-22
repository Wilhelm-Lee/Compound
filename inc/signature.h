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

#ifndef COMPOUND_SIGNATURE_H
# define COMPOUND_SIGNATURE_H

# include <stdlib.h>

# include "Compound/string.h"

# define SIGNATURE_LENGTH_MAXIMUM  1024

/* "char**" -> "cpp". */
Status Signature_Encode(String *store, const String raw);
/* "cpp" -> "char**". */
Status Signature_Decode(String *store, const String signature);
/* Returns total bytes written. */
size_t Signature_Print(FILE *stream, const String signature);

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

// # define terminate(condition)                              \
// {                                                          \
//   if (condition) {                                         \
//     a                                                      \
//   }                                                        \
// }

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

// /* Within every variadic context, quit processing when selected with @opt.
//    @block is executed before quitting. */
// # define escape(opt, block)                                \
//   if (*(app) == (opt))  { block  va_end(ap); break; }

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
  stub;                                                   \
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
  stub;                                                   \
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

// /* Record @CURRENT to @buff. */ 
// # define mark(buff)  *buff = CURRENT

// // Malfunctioning.
// # define from(opt1, id, opt2, block)                       \
// {                                                          \
//   const size_t len = distance_of(opt1, opt2);              \
//   char *id = calloc(len, sizeof(char));                    \
//   size_t idx = 0;                                          \
//   region(opt1, opt2, {                                     \
//     id[idx++] = *app;                                      \
//   })                                                       \
//   block                                                    \
//   free(id);                                                \
// }

// /* Only perform when on focus. */
// # define onfocus(block)  if (_focus)  block

typedef struct {
  int level;
  char opt_enter;
  char opt_leave;
} Module;

# define module(id, opt_enter, opt_leave)                  \
  Module id = {                                            \
    0, opt_enter, opt_leave                                \
  }

// /* Ensuring given @module has exact @opt_enter and @opt_leave. */
// # define ensure(module, wanted_enter, wanted_leave)        \
//   (((module).opt_enter == wanted_enter)                    \
//    && ((module).opt_leave == wanted_leave))

// # define either(cstr, block)                               \
// {                                                          \
//   const unsigned long _either_cstr_len =                   \
//     strlen(stremp(cstr));                                  \
//   for (register unsigned long _either_idx = 0;             \
//        _either_idx < _either_cstr_len; _either_idx++) {    \
//     if (cstr[_either_idx] == *app) {                       \
//       block                                                \
//       break;                                               \
//     }                                                      \
//   }                                                        \
// }

// # define next(block)                                       \
// {                                                          \
//   /* Ensure the next char is valid. */                     \
//   if (*app && *(app + 1)) {                                \
//     stub;                                                 \
//     app++;                                                 \
//     block                                                  \
//   }                                                        \
//   back;                                                    \
// }

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

/*# define context(type, obj, block)                         \
  {                                                        \
    type _INST = (obj);                                    \
    block                                                  \
  }

# define _LINE_RECLAIMED  (_LINE_RECLAIMED_INST != NULL)

# define with(...)                                         \
  (_LINE_RECLAIMED ? (_LINE_RECLAIMED_INST, __VA_ARGS__) : (_LINE_INST, __VA_ARGS__))

# define end                                               \
  (_LINE_INST)

# define by(obj)                                           \
  {                                                        \
    *((__typeof__(obj) *)_LINE_RECLAIMED_INST) = (obj);    \
    _LINE_RECLAIMED = true;                                \
  }

# define line(block)                                       \
  {                                                        \
    __typeof__(_INST) _LINE_INST = _INST;                  \
    void *_LINE_RECLAIMED_INST = NULL;                     \
    block;                                                 \
  }*/


STATUS(ParseError, 1,
       "An error occurred during parsing.",
       STATUS_ERROR, &RuntimeError);

STATUS(ParseModuleMismatch, 1,
       "Failed to match up entering part with leaving part for one module.",
       STATUS_ERROR, &ParseError);

STATUS(UnavailableParsingFormat, 1,
       "Given format for parsing is null.",
       STATUS_ERROR, &UnavailableObject);

#endif  /* COMPOUND_SIGNATURE_H */
