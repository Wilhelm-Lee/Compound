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

# include "../inc/array.h"
# include "../inc/arrays.h"
# include "../inc/common.h"
# include "../inc/status.h"

# define STRING_LENGTH_MAXIMUM  INT32_MAX

typedef struct {
  Array(byte) data;
  size_t width;  // Char width for read & write.
} String;

ARRAY(String);

Status String_Create(String *inst, const size_t length, const size_t width);
Status String_CopyOf(String *inst, const String other);
Status String_Delete(String *inst);

Status String_Update(String *inst, const char *content);
Status String_Concat(String *inst, const String string);
Status String_Compare(int *result, const String str1, const String str2);
Status String_Format(String *inst, const String format, ...);
Status String_Substring(const String source, String *store,
                        const size_t off, const size_t len);
// int String_First(const String source, const String target);
// int String_Last(const String source, const String target);

# define string(cstr)                                      \
({                                                         \
  String string = EMPTY;                                   \
  fail(call(String,, Create) with (&string, strlen(cstr), sizeof((cstr)[0])));\
  fail(call(String,, Update) with (&string, cstr));        \
  string;                                                  \
})

# define length(string)                                    \
  ((string).data.length ? ((string).data.length - 2) : 0)

# define getbyte(string, idx)                              \
  get(string.data, byte, idx)

# define fallback(string)                                  \
  ({                                                       \
    byte *_fallback_bptr = NULL;                           \
    fail(Allocate(&_fallback_bptr, length(string) + 2, sizeof(byte)));\
    iterate (i, string.data) {                             \
      _fallback_bptr[i] = getbyte(string, i);              \
    }                                                      \
    _fallback_bptr[length(string)] = '\0';                 \
    _fallback_bptr;                                        \
  })

STATUS(UnavailableString, 1,
       "Given string is null.",
       STATUS_ERROR, &UnavailableObject);

STATUS(UnavailableCharString, 1,
       "Given char string is null.",
       STATUS_ERROR, &UnavailableObject);

STATUS(StringNotNullTerminated, 1,
       "The given string is not terminated with \\0.",
       STATUS_ERROR, &InvalidObject);

STATUS(InvalidStringWidth, 1,
       "The width for character in given string is zero.",
       STATUS_ERROR, &InvalidObject);

STATUS(FormatError, 1,
       "An error occurred when examining the given format.",
       STATUS_ERROR, &RuntimeError);

STATUS(StringFormatObjectDescriptorMismatch, 1,
       "Described object count does not equal to actual given object count.",
       STATUS_ERROR, &FormatError);

STATUS(StringTooLong, 1,
       "String is too long, nevertheless.",
       STATUS_ERROR, &InvalidObject);

#endif  /* COMPOUND_STRING_H */
