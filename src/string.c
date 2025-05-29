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

#include <Compound/string.h>

IMPL_ARRAY(String)

Status String_Create(String *inst, const size_t length, const size_t width)
{
  avail(inst);
  state(!width, InvalidStringWidth);
  state(length == UINT32_MAX, StringTooLong);

  if (!length) {
    fail(call(Array, byte, Create) with (&inst->data, 1));
    fail(set(&inst->data, byte, 0, 0));  // Null-terminator.
  } else {
    fail(call(Array, byte, Create) with (&inst->data, length));
    fail(call(Array, byte, Insert) with (&inst->data, last(inst->data), '\0'));
  }
  
  fail(call(Array, size_t, Create) with (&inst->breaks, 0));
  
  inst->width = width;
  
  RETURN(NormalStatus);
}

Status String_CopyOf(String *inst, const String other)
{
  avail(inst);
  state(!other.width, InvalidStringWidth);
  
  fail(call(Array, byte, CopyOf) with (&inst->data, other.data));
  
  inst->width = other.width;
  
  RETURN(NormalStatus);
}

Status String_Delete(String *inst)
{
  avail(inst);
  
  fail(call(Array, size_t, Delete) with (&inst->breaks));
  
  fail(call(Array, byte, Delete) with (&inst->data));
  
  inst->width = 0;
  
  RETURN(NormalStatus);
}

Status String_Update(String *inst, const char *content)
{
  avail(inst);
  state(!content, UnavailableCharString);
  
  ig call(String,, Delete) with (inst);
  
  const size_t length = strlen(content);
  fail(call(String,, Create(inst, length, sizeof(char))));
  for (register size_t i = 0; i < length; i++) {
    fail(set(&inst->data, byte, i, content[i]));
  }
  
  RETURN(NormalStatus);
}

Status String_Concat(String *inst, const String string)
{
  avail(inst);
  
  String tmp = EMPTY;
  fail(call(String,, Create)
       with (&tmp, length(*inst) + length(string),
             max(inst->width, string.width)));
  
  iterate (i, tmp.data) {
    if (i < length(*inst)) {
      fail(setbyte(&tmp, i, getbyte(*inst, i)));
      continue;
    }
    
    fail(setbyte(&tmp, i, getbyte(string, i - length(*inst))));
  }
  
  fail(call(String,, Delete) with (inst));
  fail(call(String,, CopyOf) with (inst, tmp));
  
  fail(call(String,, Delete) with (&tmp));
  
  RETURN(NormalStatus);
}

Status String_Compare(int *store, const String string1, const String string2)
{
  avail(store);

  *store = 0;
  const int maxlen = max(length(string1), length(string2));
  String *longer_string = (String *)((maxlen == (string1).data.length) ? (&string1) : (&string2));
  iterate (i, longer_string->data) {
    if (getbyte(string1, i) == getbyte(string2, i)) {
      continue;
    }
    
    *store = (getbyte(string1, i) - getbyte(string2, i));
    break;
  }
  
  RETURN(NormalStatus);
}

// Status String_Format(String *inst, const String format, ...)
// {
//   avail(inst);
//   state(!format.data.data, UnavailableString);
  
//   String buff = EMPTY;
//   fail(call(String,, Create(&buff, NULL, sizeof(char *))));
  
//   const size_t written = vsnprintf(NULL, STRING_LENGTH_MAXIMUM, )
  
//   fail(call(String,, CopyOf(inst, buff)));
//   fail(call(String,, Delete(&buff)));
// }

Status String_Substr(String *store, const String source,
                     const size_t off, const size_t len)
{
  state(off + len > length(source), StringIndexOutOfBound);

  if (!len) {
    *store = (String)EMPTY;
    
    RETURN(NormalStatus);
  }
  
  fail(call(String,, Create) with (store, len, source.width));
  for (register size_t i = 0; i < len; i++) {
    fail(setbyte(store, i, getbyte(source, i + off)));
  }
  
  RETURN(NormalStatus);
}

boolean _String_MatchesAny(const byte source, const String targets)
{
  if (!source || !length(targets)) {
    return false;
  }
  
  stringing (t, targets, {
    if (source == t) {
      return true;
    }
  })
  
  return false;
}

Status String_Tokens(String *inst, const String delim, size_t *count)
{
  avail(inst);
  state(!length(*inst) || !length(delim), NormalStatus);

  size_t tokenth = 0;
  size_t begin = 0;
  size_t end = 0;
  boolean refreshed = false;
  for (register size_t i = 0; i < length(*inst); i++) {
    const boolean delimed = _String_MatchesAny(getbyte(*inst, i), delim);

    /* First byte of a token. */
    if (!refreshed && !delimed) {
      refreshed = true;
      tokenth++;
      begin = i;
      fail(call(Array, size_t, Insert)
           with (&inst->breaks, last(inst->breaks), begin));
    }
    
    /* The byte should be skipped. */
    if (!refreshed && delimed) {
      continue;
    }
    
    /* A byte within a token. */
    if (refreshed && !delimed) {
      continue;
    }

    /* One byte right after a token. */
    if (refreshed && delimed) {
      refreshed = false;
      end = i - 1;
      fail(call(Array, size_t, Insert)
           with (&inst->breaks, last(inst->breaks), end - begin + 1));
    }
  }
  
  if (count) {
    *count = tokenth;
  }
  
  RETURN(NormalStatus);
}

Status String_Breaks(String *store, const String source, const size_t tokenth)
{
  const size_t off = get(source.breaks, size_t, tokenth * 2);
  size_t len = get(source.breaks, size_t, tokenth * 2 + 1);
  
  /* Set @len as the remaining length of string if no value provided. */
  if (!len) {
    len = last(source.data) - off - 1;
  }
  
  fail(call(String,, Substr) with (store, source, off, len));

  RETURN(NormalStatus);
}

size_t String_FirstOf(const String source, const byte target,
                      const size_t offset)
{
  if (offset >= length(source)) {
    return -1;
  }
  
  for (register size_t i = offset; i < length(source); i++) {
    if (target == getbyte(source, i)) {
      return i;
    }
  }
  
  return -1;
}

size_t String_LastOf(const String source, const byte target,
                     const size_t offset)
{
  if (offset >= length(source)) {
    return -1;
  }
  
  for (register size_t i = offset; i < length(source); i++) {
    if (target == getbyte(source, (length(source) - i - 1))) {
      return i;
    }
  }
  
  return -1;
}
