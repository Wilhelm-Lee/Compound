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

Status String_Create(String *inst, const size_t length, const size_t width)
{
  avail(inst);
  state(!width, InvalidStringWidth);
  state(length == UINT32_MAX, StringTooLong);
  
  fail(call(Array, byte, Create) with (&inst->data, length));
  fail(call(Array, byte, Insert) with (&inst->data, last(inst->data), '\0'));
  
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
  String *longer_string = ((maxlen == (string1).data.length) ? (&string1) : (&string2));
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

Status String_Substr(const String source, String *store,
                     const size_t off, const size_t len)
{
  state(off + len > length(source), StringIndexOutOfBound);
  
  fail(call(String,, Create) with (store, len, source.width));
  iterate (i, store->data) {
    fail(setbyte(store, i, getbyte(source, i + off)));
  }
  
  RETURN(NormalStatus);
}

// Status String_Format(String *inst, const String format, ...)
// {
//   avail(inst);
  
  
// }
