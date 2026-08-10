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

extern size_t strnlen(const char *, size_t);

struct String {
  Array(byte) *data;
  llong width;  // Byte width.
  Array(llong) *breaks;  // Token descriptors.  See doc/STRING.md.
};

String *String_Create(const llong length, const llong width)
{
  if (width <= 0) {
    return NULL;
  }

  if (length > INT32_MAX) {
    return NULL;
  }

  String *inst = Allocate(1, sizeof(String));
  if (!inst) {
    return NULL;
  }

  inst->data = array(byte, length + 1);
  if (!inst->data) {
    Deallocate(inst);
    return NULL;
  }

  inst->width = width;
  inst->breaks = array(llong, 0);
  if (!inst->breaks) {
    Deallocate(inst->data);
    Deallocate(inst);
    return NULL;
  }

  return inst;
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

  String *inst = Allocate(1, sizeof(String));
  if (!inst) {
    return NULL;
  }

  inst->data = array(byte, length + 1);
  if (!inst->data) {
    Deallocate(inst);
    return NULL;
  }

  memmove(refbyte(inst, 0), refbyte(other, 0), length + 1);
  inst->width = other->width;
  inst->breaks = CopyOf(Array(llong), other->breaks);
  if (!inst->breaks) {
    Deallocate(inst->data);
    Deallocate(inst);
    return NULL;
  }

  return inst;
}

void String_Delete(String *const inst)
{
  if (!inst) {
    return;
  }

  Delete(Array(byte), inst->data);
  inst->width = 0;
  Delete(Array(llong), inst->breaks);
  Deallocate(inst);
}

inline boolean String_Equals(
  const String *const string1,
  const String *const string2
) {
  return (!compare(string1, string2))
         && Equals(Array(llong), string1->breaks, string2->breaks, NULL);
}

inline String *String_Transfer(
  String **const receiver,
  String *const provider
) {
  if (!receiver || !*receiver) {
    return NULL;
  }

  Delete(String, *receiver);

  *receiver = provider;

  return *receiver;
}

String *String_Update(String **const inst, const char *restrict const cstr)
{
  /* For getting an empty string, provide "" instead. */
  if (!cstr) {
    return NULL;
  }

  const llong length = strnlen(cstr, STRING_LENGTH_MAXIMUM);

  String *const string = String_Create(length, sizeof(cstr[0]));

  /* Clean resource before re-creating. */
  if (inst && *inst) {
    Delete(String, *inst);
  }

  memmove(Getter(Array(byte), Data, string->data), cstr, sizeof(char) * length);

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

  String *const concat = Create(String, string1_len + string2_len, width);

  iterate (Array(byte), i, concat->data, {
    if (i < string1_len) {
      setbyte(string1, i, refbyte(concat, i));
      continue;
    }

    setbyte(concat, i, refbyte(string2, i - string1_len));
  })

  return concat;
}

String *String_Format(const char *restrict const format, ...)
{
  if (!format) {
    return NULL;
  }

  const llong formatlen = strnlen(format, STRING_LENGTH_MAXIMUM);
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
    (char *)refbyte(buffer, 0),
    STRING_FORMAT_BUFFER_INITIAL_LENGTH,
    format,
    ap
  );
  va_end(ap);

  String *accurate = String_Create(written, sizeof(byte));
  memmove(refbyte(accurate, 0), refbyte(buffer, 0), written + 1);

  Delete(String, buffer);

  return accurate;
}

String *String_Substr(
  const String *const source,
  const llong offset,
  const llong length
) {
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
  memmove(
    refbyte(substring, 0), refbyte(source, offset), source->width * final_length
  );

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
    if (!String_MatchesAny(*refbyte(source, i), WHITESPACE)) {
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
  return (call(Array(llong), GetCapacity, inst->breaks) % 2) +
    (call(Array(llong), GetCapacity, inst->breaks) / 2);
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

  const llong delim_cstrlen = strnlen(delim_cstr, STRING_LENGTH_MAXIMUM);
  if (!delim_cstrlen) {
    return -1;
  }

  llong tokenth = 0;
  llong begin = 0;
  llong end = 0;
  boolean refreshed = false;
  for (register llong i = 0; i < length(inst); i++) {
    const boolean delimed = String_MatchesAny(*refbyte(inst, i), delim_cstr);

    /* First byte of a token. */
    if (!refreshed && !delimed) {
      refreshed = true;
      tokenth++;
      begin = i;
      inst->breaks = call(
        Array(llong),
        Insert,
        inst->breaks,
        last(Array(llong), inst->breaks),
        &begin
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
      llong calc = end - begin + 1;
      inst->breaks = call(
        Array(llong),
        Insert,
        inst->breaks,
        last(Array(llong), inst->breaks),
        &calc
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

  const llong offset = *ref(Array(llong), source->breaks, tokenth * 2);
  llong length = *ref(Array(llong), source->breaks, tokenth * 2 + 1);

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
  iterate (Array(ptr), i, tokens, {
    *ref(Array(ptr), tokens, i) = breaks(inst, i);
  })

  return tokens;
}

inline llong String_Whence(
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

  return strstr((char *)refbyte(source, offset),
                (char *)refbyte(target, 0)) - (char *)refbyte(source, 0);
}

String *String_RemoveLeadingWhitespace(String **const inst)
{
  if (!inst || !*inst) {
    return NULL;
  }

  llong first_non_whitespace_byte = -1;
  iteratebyte (i, *inst, {
    if (!String_MatchesAny(*refbyte(*inst, i), WHITESPACE)) {
      first_non_whitespace_byte = i;
      break;
    }
  })

  /* Not found.  The whole string was whitespace. */
  if (first_non_whitespace_byte < 0) {
    Delete(String, *inst);
    return string("");
  }

  String *substring = substr(*inst, first_non_whitespace_byte, -1);
  Delete(String, *inst);

  return substring;
}

String *String_RemoveTrailingWhitespace(String **const inst)
{
  if (!inst || !*inst) {
    return NULL;
  }

  llong last_non_whitespace_byte = -1;
  iteratebyte (i, *inst, {
    if (!String_MatchesAny(*refbyte(*inst, i), WHITESPACE)) {
      last_non_whitespace_byte = i;
    }
  })

  /* Not found. The whole string was whitespace. */
  if (last_non_whitespace_byte < 0) {
    Delete(String, *inst);
    return string("");
  }

  /* Create substring from index 0 with length (last_index + 1) */
  String *substring = substr(*inst, 0, last_non_whitespace_byte + 1);
  Delete(String, *inst);

  return substring;
}

boolean String_MatchesAny(const byte target, const char *const group)
{
  if (!target) {
    return false;
  }

  const llong grouplen = strnlen(group, STRING_LENGTH_MAXIMUM);
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
) {
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
    if (target == *refbyte(source, i)) {
      return i;
    }
  }

  return -1;
}

llong String_LastAt(
  String *const source,
  const byte target,
  const llong offset
) {
  if (!source) {
    return -1;
  }

  reverse(Array(byte), Getter(String, Data, source));

  const llong index = String_FirstAt(source, target, offset);

  reverse(Array(byte), Getter(String, Data, source));

  return index;
}

String *String_Strcut(
  String **const source,
  const llong index
) {
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

  Delete(String, *source);
  *source = remain;

  return cutoff;
}

inline llong String_Length(const String *const string)
{
  if (!string) {
    return 0;
  }

  // register llong length = 0;
  // while (getbyte(string, length)) {
  //   length += 1;
  // }

  byte *const ref_index0 = refbyte(string, 0);
  if (!ref_index0) {
    return 0;
  }

  /* Temporary optimisation applied before Char exist. */
  return strnlen((const char *)ref_index0, STRING_LENGTH_MAXIMUM);
}

String *String_Insert(
  String **const inst,
  const String *const source,
  const llong index
) {
  if (!inst || !*inst) {
    return NULL;
  }

  if (!source) {
    return *inst;
  }

  const llong instlen = length(*inst);
  if (index < 0 || index > instlen) {
    return *inst;
  }

  const llong sourcelen = length(source);
  if (!sourcelen) {
    return *inst;
  }

  const llong final_width = ((*inst)->width > source->width
                               ? (*inst)->width
                               : source->width);

  const llong length = instlen + sourcelen;

  String *insert = String_Create(length, final_width);
  memmove(refbyte(insert, 0), refbyte((*inst), 0), index);
  memmove(refbyte(insert, index), refbyte(source, 0), sourcelen);
  memmove(
    refbyte(insert, index + sourcelen), refbyte((*inst), index),
    instlen - index
  );
  *refbyte(insert, length) = 0;

  Delete(String, *inst);

  return insert;
}

String *String_Remove(
  String **const inst,
  const llong offset,
  const llong length
) {
  if (!inst || !*inst) {
    return NULL;
  }

  const llong instlen = length(*inst);
  if (offset < 0 || offset >= instlen) {
    return *inst;
  }

  llong final_length = length;

  /* All the way downtown. *//* Corp the length if too long. */
  if (final_length < 0 || offset + final_length > instlen) {
    final_length = instlen - offset;
  }

  if (!final_length) {
    return *inst;
  }

  String *result = Create(String, instlen - final_length, (*inst)->width);

  /* Copy the part before the offset. */
  if (offset > 0) {
    memmove(refbyte(result, 0), refbyte((*inst), 0), offset);
  }

  /* Copy the part after the removed section. */
  if (offset + final_length < instlen) {
    memmove(
      refbyte(result, offset),
      refbyte((*inst), offset + final_length),
      instlen - (offset + final_length)
    );
  }

  Delete(String, *inst);

  return result;
}

llong String_CountOccurrences(
  const String *const content,
  const String *const target,
  const llong offset
) {
  if (!content || !target || offset < 0) {
    return 0;
  }

  const llong contentlen = length(content);
  const llong targetlen = length(target);
  if (offset > contentlen || offset + targetlen > contentlen) {
    return 0;
  }

  llong occurrence_accum = 0;
  llong progress = offset;
  llong whence = -1;
  while ((whence = whence(content, target, progress)) >= 0) {
    occurrence_accum++;
    progress = whence + 1;
  }

  return occurrence_accum;
}

Array(llong) *String_Occurrences(
  const String *const content,
  const String *const target,
  const llong offset
) {
  if (!content || !target || offset < 0) {
    return NULL;
  }

  const llong contentlen = length(content);
  const llong targetlen = length(target);
  if (offset > contentlen || offset + targetlen > contentlen) {
    return NULL;
  }

  Array(llong) *occurrences = array(llong, contentlen);
  if (!occurrences) {
    return NULL;
  }

  llong occurrence_accum = 0;
  llong progress = offset;
  llong whence = -1;
  while ((whence = whence(content, target, progress)) >= 0) {
    set(Array(llong), occurrences, occurrence_accum, &whence);
    occurrence_accum++;
    progress = whence + 1;
  }

  occurrences = resize(Array(llong), occurrences, occurrence_accum);

  return occurrences;
}

String *String_ReplaceFirst(
  String **const inst,
  const String *target,
  const String *replacement,
  const llong offset
) {
  if (!inst || !*inst) {
    return NULL;
  }

  const llong instlen = length(*inst);
  const llong targetlen = length(target);
  if (!instlen || !target || !replacement || !targetlen || targetlen > instlen
      || (offset + targetlen) > instlen) {
    return *inst;
  }

  const llong occurrence = String_Whence(*inst, target, offset);
  if (occurrence < 0) {
    return *inst;
  }

  const llong final_width = ((*inst)->width >= replacement->width
                               ? (*inst)->width
                               : replacement->width);

  const llong replacementlen = length(replacement);
  String *replace = String_Create(
    instlen + (replacementlen - targetlen), final_width
  );
  if (!replace) {
    return NULL;
  }

  memmove(refbyte(replace, 0), refbyte((*inst), 0), occurrence);
  memmove(
    refbyte(replace, occurrence),
    refbyte(replacement, 0),
    replacementlen
  );
  memmove(
    refbyte(replace, occurrence + replacementlen),
    refbyte((*inst), occurrence + targetlen),
    instlen - (occurrence + targetlen)
  );

  Delete(String, *inst);

  return replace;
}

String *String_ReplaceAll(
  String **const inst,
  const String *target,
  const String *replacement,
  const llong offset
) {
  if (!inst || !*inst) {
    return NULL;
  }

  const llong instlen = length(*inst);
  const llong targetlen = length(target);
  if (!instlen || !target || !replacement || !targetlen || targetlen > instlen
      || (offset + targetlen) > instlen) {
    return *inst;
  }

  Array(llong) *occurrences = String_Occurrences(*inst, target, offset);
  if (!occurrences) {
    return *inst;
  }

  const llong replacementlen = length(replacement);
  const llong diff = replacementlen - targetlen;
  const llong final_width = ((*inst)->width >= replacement->width
                               ? (*inst)->width
                               : replacement->width);

  String *replace = String_Create(
    instlen + (call(Array(llong), GetCapacity, occurrences) * diff), final_width
  );
  if (!replace) {
    Delete(Array(llong), occurrences);
    return NULL;
  }

  llong dst_idx = 0;
  llong src_idx = 0;

  foreach (llong, occur, occurrences, {
    /* Calculate the distance between the last match and this match */
    llong seg_length = occur - src_idx;

    /* Copy original text before the occurrence */
    if (seg_length > 0) {
      memcpy(
        refbyte(replace, dst_idx),
        refbyte((*inst), src_idx),
        seg_length
      );
      dst_idx += seg_length;
    }

    /* Copy the replacement text */
    if (replacementlen > 0) {
      memcpy(
        refbyte(replace, dst_idx),
        refbyte(replacement, 0),
        replacementlen
      );
      dst_idx += replacementlen;
    }

    /* Advance source index past the target we just replaced */
    src_idx = occur + targetlen;
  })

  /* Copy the remaining tail of the original string */
  llong tail_length = instlen - src_idx;
  if (tail_length > 0) {
    memcpy(
      refbyte(replace, dst_idx),
      refbyte((*inst), src_idx),
      tail_length
    );
  }

  Delete(Array(llong), occurrences);
  Delete(String, *inst);

  return replace;
}

void *String_Flatten(const String *const inst, const llong width)
{
  if (!inst) {
    return NULL;
  }

  byte *const index0 = refbyte(inst, 0);

  if (!index0) {
    /* Default width is the width of a byte.
       There is no need to initialise the terminating byte since @Allocate
       zeros everything out whatsoever. */
    void *const empty_buffer = Allocate(1, sizeof(byte));
    if (!empty_buffer) {
      return NULL;
    }

    /* Returning an empty string but to allocate it is to align with the
       returning convention of this function, avoiding unexpected double free.
     */
    return empty_buffer;
  }

  const llong final_width = (inst->width > width ? inst->width : width);

  const llong instlen = length(inst);
  void *const buffer = Allocate(instlen + 1, final_width);
  memmove(buffer, index0, instlen + 1);

  return buffer;
}

boolean String_Contains(const String *const inst, const String *const target)
{
  if (!inst || !target) {
    return false;
  }

  const llong instlen = length(inst);
  const llong targetlen = length(target);
  if (instlen < targetlen) {
    return false;
  }

  /* An empty set belongs to every set, including itself. */
  if (!targetlen) {
    return true;
  }

  return String_Whence(inst, target, 0) > 0;
}

inline String *String_Reverse(String *const inst)
{
  if (!inst) {
    return NULL;
  }

  ig call(Array(byte), Reverse, Getter(String, Data, inst));

  return inst;
}

inline Array(byte) *String_GetData(const String *const inst)
{
  if (!inst) {
    return NULL;
  }

  return inst->data;
}
inline llong String_GetWidth(const String *const inst)
{
  if (!inst) {
    return 0;
  }

  return inst->width;
}
inline Array(llong) *String_GetBreaks(const String *const inst)
{
  if (!inst) {
    return NULL;
  }

  return inst->breaks;
}

IMPL_ARRAY_OBJECT(String)
