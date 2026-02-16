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

/** @file string.h */

#ifndef COMPOUND_STRING_H
# define COMPOUND_STRING_H

# include <stdarg.h>
# include <stdlib.h>
# include <stdio.h>

# include "arrays.h"
# include "common.h"
# include "const.h"

# define STRING_LENGTH_MAXIMUM  INT32_MAX
# define STRING_FORMAT_BUFFER_INITIAL_LENGTH  256LL

typedef struct
{
  Array(byte) *data;
  llong width; ///< Byte width.
  Array(llong) *breaks; ///< Token descriptors.  See doc/STRING.md.
} String;

ARRAY(String);

# define string(char_ptr)\
  (String_Update(NULL, char_ptr))

# define length(string_ptr)\
  (String_Length(string_ptr))

# define refbyte(string_ptr, idx)\
  (ref(byte, (string_ptr)->data, idx))

# define getbyte(string_ptr, idx)\
  (get(byte, (string_ptr)->data, idx))

# define setbyte(string_ptr, idx, elem)\
  set(byte, (string_ptr)->data, idx, elem)

# define fallback(string_ptr)\
  ((string_ptr)->data->data)

# define compare(string_ptr1, string_ptr2)\
  (String_Compare((string_ptr1), (string_ptr2)))

# define concat(string_ptr1, string_ptr2)\
  (String_Concat((string_ptr1), (string_ptr2)))

# define substr(string_ptr, offset, length)\
  (String_Substr((string_ptr), (offset), (length)))

# define firstat(string_ptr, target_byte, offset)\
  (String_FirstAt(string_ptr, target_byte, offset))

# define lastat(string_ptr, target_byte, offset)\
  (String_LastAt(string_ptr, target_byte, offset))

# define firstof(string_ptr)\
  getbyte(string_ptr, 0)

# define lastof(string_ptr)\
  getbyte(string_ptr, length(string_ptr) - 1)

# define stringing(it, string_ptr, block)\
  foreach (byte, it, &(string_ptr)->data, block)

# define tokens(string_ptr, delim_cstr)\
  (String_Tokens(string_ptr, delim_cstr))

# define breaks(string_ptr, tokenth)\
  (String_Breaks(string_ptr, tokenth))

/* WARNING: Could generate considerable amount of data in a sudden. */
# define gather(string_ptr)\
  (String_Gather(string_ptr))

# define whence(string_ptr, target_string_ptr, offset)\
  (String_Whence(string_ptr, target_string_ptr, offset))

# define blank(string_ptr)\
  (String_Blank(string_ptr))

# define tokenise(string_ptr, delim_string_ptr, idx)\
  tokens(string_ptr, delim_string_ptr);\
  breaks(string_ptr, idx)

# define trim(string_ptr_ptr)\
  (String_Trim(string_ptr_ptr))

/* Returns an array of two parts from @string. */
# define strcut(string_ptr_ptr, index)\
  (String_StrCut(string_ptr_ptr, index))

# define format(format_cstr, ...)\
  (String_Format(format_cstr, __VA_ARGS__))

# define insert(string_ptr_ptr, string_source_ptr, index)\
  (String_Insert(string_ptr_ptr, string_source_ptr, index))

/* The size of given @type must be larger than or equals to
 * @string_ptr->width to prevent irrational-cross-byte access. */
# define flatten(type, string_ptr)\
  ((type *)String_Flatten(string_ptr, sizeof(type)))

# define contains(string_ptr, string_target_ptr)\
  (String_Contains(string_ptr, string_target_ptr))

# define iteratebyte(it, string_ptr, block)\
  do {\
    const llong CONCAT(it, len) = length(string_ptr);\
    for (register llong it = 0; i < CONCAT(it, len); i++) {\
      block\
    }\
  } while (0);

# define foreachbyte(it, string_ptr, block)\
  do {\
    const llong CONCAT(it, len) = length(string_ptr);\
    for (register llong i = 0; i < CONCAT(it, len); i++) {\
      const byte it = getbyte(string_ptr, i);\
      block\
    }\
  } while (0);

String *String_Create(const llong length, const llong width);

String *String_CopyOf(const String *const other);

void String_Delete(String **const inst);

boolean String_Equals(const String *const string1, const String *const string2);

/**
 * @brief Construct the instance by sampling from @content.
 *
 * @param inst The previous instance of String to be modified.
 *             If this parameter is provided valid, this function will recollect
 *             the resource from it before re-creating it with @cstr as content.
 * @param cstr The char-string literal content.
 *
 * @return A new string composed with @content.
 */
String *String_Update(String **const inst, const char *restrict const cstr);

/* @return The value under a given indexer from @string1
           minus to the one from @string2.

           If the bytes from both side under indexer N
           are the same as in values, the result for
           comparing should be 0. */
int String_Compare(const String *const string1, const String *const string2);

/* @return The new string concatenates the two of given strings. */
String *String_Concat(String *const string1, const String *const string2);

/**
 *
 * @param source The source of sample String instance.
 * @param offset The beginning indexer of the sub-string.
 * @param length The duration length of the sub-string.
 * @return A reference to newly allocated string processed.
 *         NULL when either:
 *             @source is NULL
 *             @offset + @length is greater than the length of @source.
 */
String *String_Substr(
  const String *const source,
  const llong offset,
  const llong length
);

/* @return The total count for tokens by @delim. */
inline llong String_CountTokens(const String *const inst);

/* @return The total count for tokens by @delim. */
llong String_Tokens(String *const inst, const char *restrict const delim_cstr);

/* @return The piece of substring broken from @source. */
String *String_Breaks(const String *const source, const llong tokenth);

/* @return An array that holds the collection of
 * the references of all pieces by @breaks. */
Array(ptr) *String_Gather(const String *const inst);

/* @return The indexer of the first occurrence of @target in @source
           by @offset. */
llong String_FirstAt(
  const String *const source,
  const byte target,
  const llong offset
);

/* @return The indexer of the last occurrence of @target in @source
           by @offset. */
llong String_LastAt(
  const String *const source,
  const byte target,
  const llong offset
);

/* @return The starting indexer of found @target in @source by @offset. */
llong String_Whence(
  const String *const source,
  const String *const target,
  const llong offset
);

/* @return The new string formatted by @format with @.... */
String *String_Format(const char *restrict const format, ...);

boolean String_Empty(const String *const source);

boolean String_Blank(const String *const source);

/* @return An new string after trimming. */
String *String_Trim(String **const inst);

/* @return An new string after trimming. */
String *String_RemoveLeadingWhitespace(String **const inst);

/* @return An new string after trimming. */
String *String_RemoveTrailingWhitespace(String **const inst);

/* @return If @ch is contained in any element from @targets. */
boolean String_MatchesAny(const byte target, const char *const group);


/** Cut an instance of String to make it shortened but also
 *  preversed the cut-off part returned.
 *
 * @param source The source of an String instance to be cut.
 * @param index The cutting position from @source.
 * @return An instance of String fell off after cutting.
 */
String *String_StrCut(
  const String **const source,
  const llong index
);

/* @return The calculated length for the given instance of String @inst. */
llong String_Length(const String *const string);

/* Insert @source at @index in @inst. */
String *String_Insert(
  String **const inst,
  const String *const source,
  const llong index
);

/* Flatten to a contiguous chunk of memory about the byte data. */
void *String_Flatten(const String *const inst, const llong width);

boolean String_Contains(const String *const inst, const String *const target);

#endif  /* COMPOUND_STRING_H */
