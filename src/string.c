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
  Array(llong) *frags;  // Fragment descriptors.
};

String *String_Create(const llong length, const llong width)
{
  if (width <= 0) {
    return null;
  }

  if (length > INT32_MAX) {
    return null;
  }

  String *inst = Allocate(1, sizeof(String));
  if (!inst) {
    return null;
  }

  inst->data = array(byte, length + 1);
  if (!inst->data) {
    Deallocate(inst);
    return null;
  }

  inst->width = width;
  inst->breaks = array(llong, 0);
  if (!inst->breaks) {
    Deallocate(inst->data);
    Deallocate(inst);
    return null;
  }

  inst->frags = array(llong, 0);
  if (!inst->frags) {
    Delete(Array(llong), inst->breaks);
    Deallocate(inst->data);
    Deallocate(inst);
    return null;
  }

  return inst;
}

String *String_CopyOf(const String *const other)
{
  if (!other) {
    return null;
  }

  if (other->width <= 0) {
    return null;
  }

  const llong length = Length(String, other);

  if (length > INT32_MAX) {
    return null;
  }

  String *inst = Allocate(1, sizeof(String));
  if (!inst) {
    return null;
  }

  inst->data = array(byte, length + 1);
  if (!inst->data) {
    Deallocate(inst);
    return null;
  }

  memmove(refbyte(inst, 0), refbyte(other, 0), length + 1);
  inst->width = other->width;
  inst->breaks = CopyOf(Array(llong), other->breaks);
  if (!inst->breaks) {
    Deallocate(inst->data);
    Deallocate(inst);
    return null;
  }

  inst->frags = CopyOf(Array(llong), other->frags);
  if (!inst->frags) {
    Delete(Array(llong), inst->breaks);
    Deallocate(inst->data);
    Deallocate(inst);
    return null;
  }

  return inst;
}

void String_Delete(String *const inst)
{
  if (!inst) {
    return;
  }

  inst->width = 0;
  Delete(Array(llong), inst->frags);
  Delete(Array(llong), inst->breaks);
  Delete(Array(byte), inst->data);
  Deallocate(inst);
}

inline boolean String_Equals(
  String *const string1,
  String *const string2
) {
  return (!compare(string1, string2))
         && Equals(Array(llong), string1->frags, string2->frags, null)
         && Equals(Array(llong), string1->breaks, string2->breaks, null);
}

inline String *String_Transfer(
  String **const receiver,
  String **const provider
) {
  if (!receiver) {
    return null;
  }

  // *receiver = provider;
  *receiver = *provider;
  *provider = null;

  return *receiver;
}

String *String_Update(String *const inst, const char *restrict const cstr)
{
  /* For getting an empty string, provide "" instead. */
  if (!cstr) {
    return null;
  }

  const llong length = strnlen(cstr, STRING_LENGTH_MAXIMUM);

  String *const string = String_Create(length, sizeof(cstr[0]));

  /* Clean resource before re-creating. */
  if (inst) {
    Delete(String, inst);
  }

  memmove(Getter(Array(byte), Data, string->data), cstr, sizeof(char) * length);

  return string;
}

int String_Compare(const String *const string1, const String *const string2)
{
  if (!string1 || !string2) {
    return false;
  }

  const llong string1_len = Length(String, string1);
  const llong string2_len = Length(String, string2);
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
    return null;
  }

  if (!string1) {
    return CopyOf(String, string2);
  }

  if (!string2) {
    return CopyOf(String, string1);
  }

  const llong string1_len = Length(String, string1);
  const llong string2_len = Length(String, string2);
  const llong width = (
    string1->width > string2->width ?
      string1->width :
      string2->width
  );

  String *const concat = Create(String, string1_len + string2_len, width);

  iterate (byte, i, concat->data, {
    if (i < string1_len) {
      setbyte(concat, i, getbyte(string1, i));
      continue;
    }

    setbyte(concat, i, getbyte(string2, i - string1_len));
  })

  Delete(String, string1);

  return concat;
}

String *String_Format(const char *restrict const format, ...)
{
  if (!format) {
    return null;
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

  /* Parse the %{Object} pivots. */


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
    return null;
  }

  const llong sourcelen = Length(String, source);
  if (!sourcelen) {
    return string("");
  }

  llong final_length = length;

  /* Not giving effective length means the maximum length after offset. */
  if (length < 0) {
    final_length = sourcelen - offset;
  }

  const llong source_length = Length(String, source);
  if (offset + final_length > source_length) {
    return null;
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

  return !Length(String, source);
}

boolean String_Blank(const String *const source)
{
  if (!source) {
    return false;
  }

  const llong sourcelen = Length(String, source);
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

String *String_RemoveLeadingWhitespace(String *const inst)
{
  if (!inst) {
    return null;
  }

  llong first_non_whitespace_byte = -1;
  const llong len = Length(String, inst);

  /* Iterate forward and break on the first valid character */
  loop (i, len) {
    if (!String_MatchesAny(getbyte(inst, i), WHITESPACE)) {
      first_non_whitespace_byte = i;
      break;
    }
  }

  /* Not found. The whole string was whitespace. */
  if (first_non_whitespace_byte < 0) {
    Delete(String, inst);
    return string("");
  }

  String *substring = substr(inst, first_non_whitespace_byte, -1);
  Delete(String, inst);

  return substring;
}

String *String_RemoveTrailingWhitespace(String *const inst)
{
  if (!inst) {
    return null;
  }

  llong last_non_whitespace_byte = -1;
  const llong len = Length(String, inst);

  /* Iterate backwards efficiently using rloop */
  rloop (i, len) {
    if (!String_MatchesAny(getbyte(inst, i), WHITESPACE)) {
      last_non_whitespace_byte = i;
      break;
    }
  }

  /* Not found. The whole string was whitespace. */
  if (last_non_whitespace_byte < 0) {
    Delete(String, inst);
    return string("");
  }

  /* Create substring from index 0 with length (last_index + 1) */
  String *substring = substr(inst, 0, last_non_whitespace_byte + 1);
  Delete(String, inst);

  return substring;
}

String *String_Trim(String *inst)
{
  if (!inst) {
    return null;
  }

  inst = String_RemoveLeadingWhitespace(inst);
  inst = String_RemoveTrailingWhitespace(inst);

  return inst;
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

  const llong instlen = Length(String, inst);
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
  for (register llong i = 0; i < Length(String, inst); i++) {
    const boolean delimed = String_MatchesAny(getbyte(inst, i), delim_cstr);

    /* First byte of a token. */
    if (!refreshed && !delimed) {
      refreshed = true;
      tokenth++;
      begin = i;
      inst->breaks = call(
        Array(llong),
        Insert,
        inst->breaks,
        capacity(Array(llong), inst->breaks),
        begin
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
        capacity(Array(llong), inst->breaks),
        calc
      );
    }
  }

  return tokenth;
}

String *String_Breaks(const String *const source, const llong tokenth)
{
  if (!source || tokenth < 0) {
    return null;
  }

  const llong sourcelen = Length(String, source);
  if (!sourcelen) {
    return null;
  }

  const llong count = String_CountTokens(source);

  if (!count) {
    return null;
  }

  const llong offset = get(Array(llong), source->breaks, tokenth * 2);
  llong length = get(Array(llong), source->breaks, tokenth * 2 + 1);

  /* Set @length as the remaining length of string if no value is provided. */
  if (!length) {
    length = sourcelen - offset;
  }

  return substr(source, offset, length);
}

Array(String) *String_Gather(const String *const inst)
{
  if (!inst) {
    return null;
  }

  const llong count = String_CountTokens(inst);
  if (!count) {
    return null;
  }

  Array(String) *tokens = array(String, count);
  iterate (String, i, tokens, {
    set(Array(String), tokens, i, breaks(inst, i));
  })

  return tokens;
}

inline llong String_CountFragments(const String *const inst)
{
  if (!inst || !inst->frags) return 0;

  return (call(Array(llong), GetCapacity, inst->frags) % 2) +
         (call(Array(llong), GetCapacity, inst->frags) / 2);
}

llong String_Fragmentise(String *const inst, const char *restrict const delim_cstr)
{
  if (!inst) {
    return -1;
  }

  const llong instlen = Length(String, inst);
  if (!instlen) {
    return 0;
  }

  String *delim_str = string(delim_cstr);
  const llong delim_len = Length(String, delim_str);
  if (!delim_len) {
    Delete(String, delim_str);
    return -1;
  }

  /* Reset previous fragments to avoid cross-contamination */
  Delete(Array(llong), inst->frags);
  inst->frags = array(llong, 0);

  llong fragth = 0;
  llong offset = 0;
  llong whence_idx = -1;

  while ((whence_idx = whence(inst, delim_str, offset)) >= 0) {
    const llong frag_len = whence_idx - offset;

    /* Skip empty fragments to mirror String_Tokens behavior */
    if (frag_len > 0) {
      inst->frags = call(Array(llong), Insert, inst->frags, capacity(Array(llong), inst->frags), offset);
      inst->frags = call(Array(llong), Insert, inst->frags, capacity(Array(llong), inst->frags), frag_len);
      fragth++;
    }

    offset = whence_idx + delim_len; /* Jump entirely over the delim substring */
  }

  /* Capture the remaining tail of the string */
  if (offset < instlen) {
    inst->frags = call(Array(llong), Insert, inst->frags, capacity(Array(llong), inst->frags), offset);
    inst->frags = call(Array(llong), Insert, inst->frags, capacity(Array(llong), inst->frags), instlen - offset);
    fragth++;
  }

  Delete(String, delim_str);

  return fragth;
}

String *String_Pieces(const String *const source, const llong fragth)
{
  if (!source || fragth < 0) {
    return null;
  }

  const llong count = String_CountFragments(source);
  if (!count || fragth >= count) {
    return null;
  }

  const llong offset = get(Array(llong), source->frags, fragth * 2);
  const llong length = get(Array(llong), source->frags, fragth * 2 + 1);

  return substr(source, offset, length);
}

Array(String) *String_Collect(const String *const inst)
{
  if (!inst) {
    return null;
  }

  const llong count = String_CountFragments(inst);
  if (!count) {
    return null;
  }

  Array(String) *fragments = array(String, count);

  iterate (String, i, fragments, {
    set(Array(String), fragments, i, pieces(inst, i));
  })

  return fragments;
}

inline llong String_Whence(
  const String *const source,
  const String *const target,
  const llong offset
) {
  if (!source || !target || offset < 0) {
    return -1;
  }

  if (source->width != target->width) {
    return -1;
  }

  const llong sourcelen = Length(String, source);
  if (!sourcelen) {
    return -1;
  }

  const llong targetlen = Length(String, target);
  if (!targetlen) {
    return -1;
  }

  if (offset + targetlen > sourcelen) {
    return -1;
  }

  const byte *src_bytes = refbyte(source, 0);
  const byte *tgt_bytes = refbyte(target, 0);
  const llong search_limit = sourcelen - targetlen;

  /* Traverse the continuous memory block using memcmp. */
  for (reg llong i = offset; i <= search_limit; i++) {
    if (memcmp(src_bytes + i, tgt_bytes, targetlen) == 0) {
      return i;
    }
  }

  return -1;
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

  const llong sourcelen = Length(String, source);
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
  String *const source,
  const byte target,
  const llong offset
) {
  if (!source) {
    return -1;
  }

  const llong sourcelen = Length(String, source);
  if (!sourcelen) {
    return -1;
  }

  if (offset < 0 || offset >= sourcelen) {
    return -1;
  }

  for (register llong i = sourcelen - 1; i >= offset; i--) {
    if (target == getbyte(source, i)) {
      return i;
    }
  }

  return -1;
}

String *String_Strcut(
  String **const source,
  const llong index
) {
  if (!source || !*source) {
    return null;
  }

  const llong sourcelen = Length(String, *source);
  if (!sourcelen) {
    return *source;
  }

  if (index <= 0 || index >= sourcelen) {
    return null;
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

  if (!refbyte(string, 0)) {
    return 0;
  }

  return capacity(Array(byte), string->data) - 1;
}

String *String_Insert(
  String *const inst,
  const String *const source,
  const llong index
) {
  if (!inst) {
    return null;
  }

  if (!source) {
    return inst;
  }

  const llong pivot = offsetting(Array(byte), inst->data, index);

  const llong instlen = Length(String, inst);
  if (index > instlen) {
    return inst;
  }

  const llong sourcelen = Length(String, source);
  if (!sourcelen) {
    return inst;
  }

  const llong final_width = (inst->width > source->width
                               ? inst->width
                               : source->width);

  const llong length = instlen + sourcelen;

  String *insert = String_Create(length, final_width);
  memmove(refbyte(insert, 0), refbyte(inst, 0), pivot);
  memmove(refbyte(insert, pivot), refbyte(source, 0), sourcelen);
  memmove(
    refbyte(insert, pivot + sourcelen), refbyte(inst, pivot),
    instlen - pivot
  );
  *refbyte(insert, length) = 0;

  Delete(String, inst);

  return insert;
}

String *String_Remove(
  String **const inst,
  const llong offset,
  const llong length
) {
  if (!inst || !*inst) {
    return null;
  }

  const llong instlen = Length(String, *inst);
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

  const llong contentlen = Length(String, content);
  const llong targetlen = Length(String, target);
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
    return null;
  }

  const llong contentlen = Length(String, content);
  const llong targetlen = Length(String, target);
  if (offset > contentlen || offset + targetlen > contentlen) {
    return null;
  }

  Array(llong) *occurrences = array(llong, contentlen);
  if (!occurrences) {
    return null;
  }

  llong occurrence_accum = 0;
  llong progress = offset;
  llong whence = -1;
  while ((whence = whence(content, target, progress)) >= 0) {
    set(Array(llong), occurrences, occurrence_accum, whence);
    occurrence_accum++;
    progress = whence + 1;
  }

  occurrences = resize(Array(llong), occurrences, occurrence_accum);

  return occurrences;
}

String *String_ReplaceFirst(
  String *const inst,
  const String *target,
  const String *replacement,
  const llong offset
) {
  if (!inst) {
    return null;
  }

  const llong instlen = Length(String, inst);
  const llong targetlen = Length(String, target);
  if (!instlen || !target || !replacement || !targetlen || targetlen > instlen
      || (offset + targetlen) > instlen) {
    return inst;
  }

  const llong occurrence = String_Whence(inst, target, offset);
  if (occurrence < 0) {
    return inst;
  }

  const llong final_width = (inst->width >= replacement->width
                               ? inst->width
                               : replacement->width);

  const llong replacementlen = Length(String, replacement);
  String *replace = String_Create(
    instlen + (replacementlen - targetlen), final_width
  );
  if (!replace) {
    return null;
  }

  memmove(refbyte(replace, 0), refbyte(inst, 0), occurrence);
  memmove(
    refbyte(replace, occurrence),
    refbyte(replacement, 0),
    replacementlen
  );
  memmove(
    refbyte(replace, occurrence + replacementlen),
    refbyte(inst, occurrence + targetlen),
    instlen - (occurrence + targetlen)
  );

  Delete(String, inst);

  return replace;
}

String *String_ReplaceAll(
  String *const inst,
  const String *target,
  const String *replacement,
  const llong offset
) {
  if (!inst) {
    return null;
  }

  const llong instlen = Length(String, inst);
  const llong targetlen = Length(String, target);
  if (!instlen || !target || !replacement || !targetlen || targetlen > instlen
      || (offset + targetlen) > instlen) {
    return inst;
  }

  Array(llong) *occurrences = String_Occurrences(inst, target, offset);
  if (!occurrences) {
    return inst;
  }

  const llong replacementlen = Length(String, replacement);
  const llong diff = replacementlen - targetlen;
  const llong final_width = (inst->width >= replacement->width
                               ? inst->width
                               : replacement->width);

  String *replace = String_Create(
    instlen + (call(Array(llong), GetCapacity, occurrences) * diff), final_width
  );
  if (!replace) {
    Delete(Array(llong), occurrences);
    return null;
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
        refbyte(inst, src_idx),
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
      refbyte(inst, src_idx),
      tail_length
    );
  }

  Delete(Array(llong), occurrences);
  Delete(String, inst);

  return replace;
}

void *String_Flatten(const String *const inst, const llong width)
{
  if (!inst) {
    return null;
  }

  byte *const index0 = refbyte(inst, 0);

  if (!index0) {
    /* Default width is the width of a byte.
       There is no need to initialise the terminating byte since @Allocate
       zeros everything out whatsoever. */
    void *const empty_buffer = Allocate(1, sizeof(byte));
    if (!empty_buffer) {
      return null;
    }

    /* Returning an empty string but to allocate it is to align with the
       returning convention of this function, avoiding unexpected double free.
     */
    return empty_buffer;
  }

  const llong final_width = (inst->width > width ? inst->width : width);

  const llong instlen = Length(String, inst);
  void *const buffer = Allocate(instlen + 1, final_width);
  memmove(buffer, index0, instlen + 1);

  return buffer;
}

boolean String_Contains(const String *const inst, const String *const target)
{
  if (!inst || !target) {
    return false;
  }

  /* Prevent comparing strings with mismatched encodings. */
  if (inst->width != target->width) {
    return false;
  }

  const llong instlen = Length(String, inst);
  const llong targetlen = Length(String, target);
  if (instlen < targetlen) {
    return false;
  }

  /* An empty set belongs to every set, including itself. */
  if (!targetlen) {
    return true;
  }

  return String_Whence(inst, target, 0) >= 0;
}

inline String *String_Reverse(String *const inst)
{
  if (!inst) {
    return null;
  }

  ig call(Array(byte), Reverse, Getter(String, Data, inst));

  return inst;
}

String *String_Append(
  String *const inst,
  Array(String) *const contents
) {
  if (!inst && !contents) {
    return null;
  }

  if (!contents) {
    return inst;
  }

  llong max_width = sizeof(char);
  register llong total_length = Length(String, inst);
  refeach (String, content, contents, {
    if (!content) {
      continue;
    }

    total_length += Length(String, content);
    max_width = content->width > max_width ? content->width : max_width;
  })

  String *const rtn = Create(String, total_length, max_width);
  if (!rtn) {
    return inst;
  }

  /* First write the content from @inst. */
  register llong written = 0;
  if (inst) {  // If @inst is available.
    foreachbyte (elem, inst, {
      *refbyte(rtn, written) = elem;
      written++;
    })
  }

  /* Then write in the strings from @contents. */
  refeach (String, content, contents, {
    if (!content) {
      continue;
    }

    foreachbyte (elem, content, {
      *refbyte(rtn, written) = elem;
      written++;
    })
  })

  Delete(String, inst);
  erase(Array(String), contents);
  Delete(Array(String), contents);

  return rtn;
}

inline String *String_Literalise(String *const inst)
{
  return CopyOf(String, inst);
}

inline Array(byte) *String_GetData(const String *const inst)
{
  if (!inst) {
    return null;
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
    return null;
  }

  return inst->breaks;
}

IMPL_ARRAY(String)
IMPL_ARRAY_LITERALISE(String)
Array(String) *StringArray_ComposeFromCstr(const llong arglen, ...)
{
  if (!arglen) {
    return Create(Array(String), 0);
  }

  Array(String) *const inst = Create(Array(String), arglen);
  if (!inst) {
    return null;
  }

  va_list ap;
  va_start(ap, arglen);
  _refrefeach_type (String, String, itptr, inst, {
    char *arg = va_arg(ap, char *);
    *itptr = string(arg ? arg : "(null)");
  })
  va_end(ap);

  return inst;
}
