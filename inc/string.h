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

#ifndef COMPOUND_STRING_H
# define COMPOUND_STRING_H

# include "array.h"
# include "arrays.h"
# include "common.h"
# include "status.h"

# define STRING_LENGTH_MAXIMUM  INT32_MAX

typedef struct {
  Array(byte) data;
  size_t width;  // Char width for read & write.
  Array(size_t) breaks;  // The beginning indexer of tokens.
} String;

ARRAY(String);

Status String_Create(String *inst, const size_t length, const size_t width);
Status String_CopyOf(String *inst, const String other);
Status String_Delete(String *inst);

Status String_Update(String *inst, const char *content);
Status String_Compare(int *store, const String string1, const String string2);
Status String_Concat(String *inst, const String string);
Status String_Substr(String *store, const String source,
                     const size_t off, const size_t len);
Status String_Tokens(String *source, const String delim, size_t *count);
Status String_Breaks(String *store, const String source, const size_t idx);
Status String_Format(String *inst, const String format, ...);
size_t String_FirstOf(const String source, const byte target,
                      const size_t offset);
size_t String_LastOf(const String source, const byte target,
                     const size_t offset);

# define string(cstr)                                      \
({                                                         \
  String _string = EMPTY;                                  \
  fail(call(String,, Create) with (&_string, strlen(cstr), sizeof((cstr)[0])));\
  fail(call(String,, Update) with (&_string, cstr));       \
  _string;                                                 \
})

# define length(string)                                    \
  ((string).data.length ? ((string).data.length - 1) : 0)

# define getbyte(string, idx)                              \
  get((string).data, byte, idx)

# define setbyte(string, idx, elem)                        \
  set(&((string)->data), byte, idx, elem)

# define fallback(string)                                  \
  ((char *)(string).data.data)

# define compare(string1, string2)                         \
  ({                                                       \
    int result = 0;                                        \
    ig call(String,, Compare) with (&result, (string1), (string2));\
    result;                                                \
  })

# define concat(string1, string2)                          \
  ({                                                       \
    fail(call(String,, Concat) with ((string1), (string2)));\
    string1;                                               \
  })

# define substr(string, off, len)                          \
  ({                                                       \
    String _substr_store = EMPTY;                          \
    fail(call(String,, Substr) with (&_substr_store, (string), (off), (len)));\
    _substr_store;                                         \
  })

# define firstof(string, byte, offset)                     \
  (call(String,, FirstOf) with (string, byte, offset))

# define lastof(string, byte, offset)                      \
  (call(String,, LastOf) with (string, byte, offset))

# define stringing(it, string, block)                      \
  foreach (byte, it, (string).data, block)

/* Example: wide(NEWLINE) */
# define wide(cstr)                                        \
  L""cstr

# define tokens(string, delim)                             \
  ({                                                       \
    size_t _tokens_count = 0;                              \
    fail(call(String,, Tokens) with (string, delim, &_tokens_count));\
    _tokens_count;                                         \
  })

# define breaks(string, tokenth)                           \
  ({                                                       \
    String _breaks_substr = EMPTY;                         \
    fail(call(String,, Breaks) with (&_breaks_substr, string, tokenth));\
    _breaks_substr;                                        \
  })

# define tokenise(string, delim, idx)                      \
  ({                                                       \
    ig tokens(string, delim);                              \
    breaks(string, idx);                                   \
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
  boolean _parse_within = false;  ig _parse_within;        \
  boolean _parse_init = false;  ig _parse_init;            \
  char *_parse_check = NULL;  ig _parse_check;             \
  char *_parse_begin = NULL;  ig _parse_begin;             \
  char *_parse_end = NULL;  ig _parse_end;                 \
  const char *app = fmt;  /*"app" stands for "ap pointer"*/\
  for (; *app; app++)  block                               \
}

/* Current character during parsing. */
# define CURRENT  (app ? *app : 0)

# define STEP  ((app - fmt) / (sizeof(char)))

/* Setup a checkpoint/stub for pointer traveling. */
# define stub  _parse_check = (char *)app

/* Travel back to an existing checkpoint and tear it down. */
# define back  if (_parse_check) { app = _parse_check;  unstub; }  

/* Tear down an existing checkpoint. */
# define unstub  _parse_check = NULL

/* Within every variadic context, execute @block for once on initial. */
# define init(block)                                       \
  if (!_parse_init) {                                      \
    _parse_init = true;                                    \
    block                                                  \
  }

/* Within every variadic context, propose solutions for @opt, typed with @type. */
# define option(opt, type, block)                          \
  if (*(app) == (opt)) {                                   \
    const type _ = va_arg(ap, type);  block                \
  }

/* Within every variadic context, propose solutions for @opt. */
# define at(opt, block)                                    \
  if (*(app) == (opt))  { block; continue; }

# define again                                             \
  app = ((app > fmt) ? (app - 1) : (fmt));                 \
  continue;

/* Within every variadic context, select given prefix @opt
   before proposing options. */
# define prefix(opt, block)                                \
  at(opt, app += 1;  block)

// Redundant with @from, @region.
# define within(opt1, opt2, before, block, after)          \
  if (*(app) == (opt1)) {                                  \
    before                                                 \
    _parse_within = true;                                  \
    continue;                                              \
  }                                                        \
  if (_parse_within && *(app) != (opt2))  block            \
  if (*(app) == (opt2)) {                                  \
    _parse_within = false;                                 \
    after                                                  \
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
  stub;                                                    \
  till(opt, block)                                         \
  back;

/* Within every variadic context, go to region and operate. */
# define region(opt1, opt2, block)                         \
  at(opt1, till(opt2, block););

// Untested.
/* Within every variadic context, operate in distance ranged
   in a nearest region.  And return back after operation. */
# define remote(opt1, opt2, block)                         \
  stub;                                                    \
  region(opt1, opt2, block);                               \
  back;

// Malfunctioning.
# define distance_of(opt1, opt2)                           \
({                                                         \
  within(opt1, opt2, {                                     \
    stub;                                                  \
    _parse_begin = (char *)app;                            \
  }, {}, {                                                 \
    _parse_end = (char *)app;                              \
    back;                                                  \
  })                                                       \
  (_parse_end - _parse_begin) - 1;                         \
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
    (module).level += 1;                                   \
    block                                                  \
  })

# define leave(module, block)                              \
  at((module).opt_leave, {                                 \
    (module).level -= 1;                                   \
    block                                                  \
  })

# define update(module)                                    \
  enter(module, continue;);                                \
  leave(module, continue;);

# define inside(module)                                    \
  ((module).level)

# define outside(module)                                   \
  (!(module).level)

/* Within every variadic context, skip given character @opt without processing. */
# define skip(opt)  if (*(app) == (opt)) continue;

STATUS(UnavailableString, 1,
       "Given string is null.",
       STATUS_ERROR, &UnavailableObject);

STATUS(UnavailableCharString, 1,
       "Given char string is null.",
       STATUS_ERROR, &UnavailableObject);

STATUS(StringIndexOutOfBound, 1,
       "Accessing elements out of bound.",
       STATUS_ERROR, &IndexOutOfBound);

STATUS(InvalidStringWidth, 1,
       "The width for character in given string is zero.",
       STATUS_ERROR, &InvalidObject);

STATUS(StringTooLong, 1,
       "String is too long, nevertheless.",
       STATUS_ERROR, &InvalidObject);

STATUS(EmptyString, 1,
       "Given string is empty.",
       STATUS_ERROR, &InvalidObject);

STATUS(NoMoreTokens, 1,
       "No more available tokens.",
       STATUS_ERROR, &RuntimeError);

STATUS(InvalidMaximumTokenCount, 1,
       "The maximum token count to apply cannot be zero or negative.",
       STATUS_ERROR, &InvalidObject);

#endif  /* COMPOUND_STRING_H */
