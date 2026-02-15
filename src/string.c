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

#include "../inc/string.h"

String *String_Create(const llong length, const llong width)
{
  if (width <= 0) {
    return NULL;
  }

  if (length > INT32_MAX) {
    return NULL;
  }

  String *string = Allocate(1, sizeof(String));

  string->data = array(byte, length + 1);
  string->width = width;
  string->breaks = array(llong, 0);

  return string;
}

String *String_CopyOf(const String *const other)
{
  if (!other) {
    return NULL;
  }

  if (other->width <= 0) {
    return NULL;
  }

  const llong length = length(other);

  if (length > INT32_MAX) {
    return NULL;
  }

  String *string = Allocate(1, sizeof(String));

  string->data = array(byte, length + 1);
  memmove(fallback(string), fallback(other), length + 1);
  string->width = other->width;
  string->breaks = CopyOf(Array(llong), other->breaks);

  return string;
}

void String_Delete(String **const inst)
{
  if (!inst || !*inst) {
    return;
  }

  Delete(Array(byte), &(*inst)->data);
  (*inst)->width = 0;
  Delete(Array(llong), &(*inst)->breaks);
  Deallocate(*inst);
  *inst = NULL;
}

inline boolean String_Equals(
  const String *const string1,
  const String *const string2
)
{
  return (!compare(string1, string2))
         && Equals(Array(llong), string1->breaks, string2->breaks, NULL);
}

inline llong _String_CountCStrLength(const char *restrict const cstr)
{
  if (!cstr) {
    return 0;
  }

  register llong count = 0;
  while (cstr[count++]) {}

  return count;
}

String *String_Update(String **const inst, const char *restrict const cstr)
{
  /* For getting an empty string, provide "" instead. */
  if (!cstr) {
    return NULL;
  }

  const llong length = _String_CountCStrLength(cstr);

  String *const string = String_Create(length, sizeof(cstr[0]));
  loop(i, length) {
    set(byte, string->data, i, cstr[i]);
  }

  /* Clean resource before re-creating. */
  if (inst && *inst) {
    Delete(String, inst);
  }

  return string;
}

int String_Compare(const String *const string1, const String *const string2)
{
  if (!string1 || !string2) {
    return false;
  }

  const llong string1_len = length(string1);
  const llong string2_len = length(string2);
  const llong minlen = string1_len < string2_len
                         ? string1_len
                         : string2_len;
  for (register llong i = 0; i < minlen; i++) {
    const int diff = getbyte(string1, i) - getbyte(string2, i);

    /* Has different. */
    if (diff) {
      return diff;
    }
  }

  return 0;
}

String *String_Concat(String *const string1, const String *const string2)
{
  if (!string1 && !string2) {
    return NULL;
  }

  if (!string1) {
    return CopyOf(String, string2);
  }

  if (!string2) {
    return CopyOf(String, string1);
  }

  const llong string1_len = length(string1);
  const llong string2_len = length(string2);
  const llong width = (string1_len > string2_len
                         ? string1_len
                         : string2_len);

  String *const concat = String_Create(string1_len + string2_len, width);

  iterate(i, concat->data) {
    if (i < string1_len) {
      setbyte(concat, i, getbyte(string1, i));
    } else {
      setbyte(concat, i, getbyte(string2, i - string1_len));
    }
  }

  return concat;
}

String *String_Format(const char *restrict const format, ...)
{
  if (!format) {
    return NULL;
  }

  const llong formatlen = _String_CountCStrLength(format);
  if (!formatlen) {
    return string("");
  }

  String *buffer = Create(
    String,
    STRING_FORMAT_BUFFER_INITIAL_LENGTH,
    sizeof(byte)
  );

  va_list ap;
  va_start(ap, format);
  const size_t written = vsnprintf(
    (char *)fallback(buffer),
    STRING_FORMAT_BUFFER_INITIAL_LENGTH,
    format,
    ap
  );
  va_end(ap);

  String *accurate = String_Create(written, sizeof(byte));
  memmove(fallback(accurate), fallback(buffer), written + 1);

  Delete(String, &buffer);

  return accurate;
}

String *String_Substr(
  const String *const source,
  const llong offset,
  const llong length
)
{
  if (!source) {
    return NULL;
  }

  const llong sourcelen = length(source);
  if (!sourcelen) {
    return NULL;
  }

  llong final_length = length;

  /* Not giving effective length means the maximum length after offset. */
  if (length < 0) {
    final_length = sourcelen - offset;
  }

  const llong source_length = length(source);
  if (offset + final_length > source_length) {
    return NULL;
  }

  String *substring = Create(String, final_length, source->width);
  memmove(fallback(substring), refbyte(source, offset), source->width * final_length);

  return substring;
}

boolean String_Empty(const String *const source)
{
  if (!source) {
    return false;
  }

  return !length(source);
}

boolean String_Blank(const String *const source)
{
  if (!source) {
    return false;
  }

  const llong sourcelen = length(source);
  if (!sourcelen) {
    return true;
  }

  for (register llong i = 0; i < sourcelen; i++) {
    if (!String_MatchesAny(getbyte(source, i), WHITESPACE)) {
      return false;
    }
  }

  return true;
}

String *String_Trim(String **const inst)
{
  if (!inst || !*inst) {
    return NULL;
  }

  *inst = String_RemoveLeadingWhitespace(inst);

  *inst = String_RemoveTrailingWhitespace(inst);

  return *inst;
}

inline llong String_CountTokens(const String *const inst)
{
  return (inst->breaks->capacity % 2) + (inst->breaks->capacity / 2);
}

llong String_Tokens(String *const inst, const char *restrict const delim_cstr)
{
  if (!inst) {
    return -1;
  }

  const llong instlen = length(inst);
  if (!instlen) {
    return 0;
  }

  const llong delim_cstrlen = _String_CountCStrLength(delim_cstr);
  if (!delim_cstrlen) {
    return -1;
  }

  llong tokenth = 0;
  llong begin = 0;
  llong end = 0;
  boolean refreshed = false;
  for (register llong i = 0; i < length(inst); i++) {
    const boolean delimed = String_MatchesAny(getbyte(inst, i), delim_cstr);

    /* First byte of a token. */
    if (!refreshed && !delimed) {
      refreshed = true;
      tokenth++;
      begin = i;
      inst->breaks = llongArray_Insert(
        &inst->breaks, last(inst->breaks), begin
      );
    }

    /* The byte should be skipped or a byte within a token. */
    if ((!refreshed && delimed) || (refreshed && !delimed)) {
      continue;
    }

    /* One byte right after a token. */
    if (refreshed && delimed) {
      refreshed = false;
      end = i - 1;
      inst->breaks = llongArray_Insert(
        &inst->breaks, last(inst->breaks), end - begin + 1
      );
    }
  }

  return tokenth;
}

String *String_Breaks(const String *const source, const llong tokenth)
{
  if (!source || tokenth < 0) {
    return NULL;
  }

  const llong sourcelen = length(source);
  if (!sourcelen) {
    return NULL;
  }

  const llong count = String_CountTokens(source);

  if (!count) {
    return NULL;
  }

  const llong offset = get(llong, source->breaks, tokenth * 2);
  llong length = get(llong, source->breaks, tokenth * 2 + 1);

  /* Set @length as the remaining length of string if no value is provided. */
  if (!length) {
    length = sourcelen - offset;
  }

  return substr(source, offset, length);
}

Array(ptr) *String_Gather(const String *const inst)
{
  if (!inst) {
    return NULL;
  }

  const llong count = String_CountTokens(inst);
  if (!count) {
    return NULL;
  }

  Array(ptr) *tokens = array(ptr, count);
  iterate (i, tokens) {
    set(ptr, tokens, i, breaks(inst, i));
  }

  return tokens;
}

llong String_Whence(
  const String *const source,
  const String *const target,
  const llong offset
) {
  if (!source || !target || offset < 0) {
    return -1;
  }

  const llong sourcelen = length(source);
  if (!sourcelen) {
    return -1;
  }

  const llong targetlen = length(target);
  if (!targetlen) {
    return -1;
  }

  if (offset + targetlen > sourcelen) {
    return -1;
  }

  return (byte *)strstr(&fallback(source)[offset],
                        fallback(target)) - fallback(source);
}

String *String_RemoveLeadingWhitespace(String **const inst)
{
  if (!inst || !*inst) {
    return NULL;
  }

  llong first_non_whitespace_byte = -1;
  iteratebyte (i, *inst, {
    if (!String_MatchesAny(getbyte(*inst, i), WHITESPACE)) {
      first_non_whitespace_byte = i;
      break;
    }
  })

  /* Not found.  The whole string was whitespace. */
  if (first_non_whitespace_byte < 0) {
    Delete(String, inst);
    return string("");
  }

  String *substring = substr(*inst, first_non_whitespace_byte, -1);
  Delete(String, inst);

  return substring;
}

String *String_RemoveTrailingWhitespace(String **const inst)
{
  if (!inst || !*inst) {
    return NULL;
  }

  llong last_non_whitespace_byte = -1;
  iteratebyte (i, *inst, {
    if (!String_MatchesAny(getbyte(*inst, i), WHITESPACE)) {
      last_non_whitespace_byte = i;
    }
  })

  /* Not found. The whole string was whitespace. */
  if (last_non_whitespace_byte < 0) {
    Delete(String, inst);
    return string("");
  }

  /* Create substring from index 0 with length (last_index + 1) */
  String *substring = substr(*inst, 0, last_non_whitespace_byte + 1);
  Delete(String, inst);

  return substring;
}

boolean String_MatchesAny(const byte target, const char *const group)
{
  if (!target) {
    return false;
  }

  const llong grouplen = _String_CountCStrLength(group);
  if (!grouplen) {
    return false;
  }

  for (register llong i = 0; i < grouplen; i++) {
    const byte current = group[i];

    if (target == current) {
      return true;
    }
  }

  return false;
}

llong String_FirstAt(
  const String *const source,
  const byte target,
  const llong offset
)
{
  if (!source) {
    return -1;
  }

  const llong sourcelen = length(source);
  if (!sourcelen) {
    return -1;
  }

  if (offset < 0 || offset >= sourcelen) {
    return -1;
  }

  for (register llong i = offset; i < sourcelen; i++) {
    if (target == getbyte(source, i)) {
      return i;
    }
  }

  return -1;
}

llong String_LastAt(
  const String *const source,
  const byte target,
  const llong offset
)
{
  if (!source) {
    return -1;
  }

  const llong sourcelen = length(source);
  if (!sourcelen) {
    return -1;
  }

  if (offset < 0 || offset >= sourcelen) {
    return -1;
  }

  for (register llong i = sourcelen - offset - 1; i >= 0; i--) {
    if (target == getbyte(source, i)) {
      return i;
    }
  }

  return -1;
}

String *String_StrCut(
  const String **const source,
  const llong index
)
{
  if (!source || !*source) {
    return NULL;
  }

  const llong sourcelen = length(*source);
  if (!sourcelen) {
    return *source;
  }

  if (index <= 0 || index >= sourcelen) {
    return NULL;
  }

  String *remain = substr(*source, 0, index);
  String *cutoff = substr(*source, index, -1);

  Delete(String, source);
  *source = remain;

  return cutoff;
}

inline llong String_Length(const String *const string)
{
  if (!string) {
    return 0;
  }

  register llong length = 0;
  while (getbyte(string, length)) {
    length += 1;
  }

  return length;
}

IMPL_ARRAY(String, !compare(A, B))
