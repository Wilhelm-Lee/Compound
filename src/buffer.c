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

/** @file buffer.c */

#include "../inc/buffer.h"

struct Buffer {
  Array(byte) *data;
  llong cursor;
};

Buffer *Buffer_Create(const llong capacity)
{
  if (capacity < 0) {
    return null;
  }

  Buffer *const inst = Allocate(1, sizeof(Buffer));
  if (!inst) {
    return null;
  }

  inst->data = array(byte, capacity);
  if (!inst->data) {
    Deallocate(inst);
    return null;
  }

  inst->cursor = 0;

  return inst;
}

Buffer *Buffer_CopyOf(const Buffer *const other)
{
  if (!other) {
    return null;
  }

  Buffer *const inst = Allocate(1, sizeof(Buffer));
  if (!inst) {
    return null;
  }

  inst->data = CopyOf(Array(byte), other->data);
  inst->cursor = other->cursor;

  return inst;
}

void Buffer_Delete(Buffer *const inst)
{
  if (!inst) {
    return;
  }

  Delete(Array(byte), inst->data);
  Deallocate(inst);
}

boolean Buffer_Equals(const Buffer *const obj1, const Buffer *const obj2)
{
  if (!obj1 || !obj2) return false;
  if (obj1 == obj2) return true;

  return obj1->cursor == obj2->cursor &&
         Equals(Array(byte), obj1->data, obj2->data, null);
}

boolean Buffer_Sample(Buffer *const inst, const String *const string)
{
  if (!inst || !string) {
    return false;
  }

  Delete(Array(byte), inst->data);

  const llong string_len = Length(String, string);
  if (string_len) {
    inst->data = CopyOf(Array(byte), Getter(String, Data, string));
  } else {
    inst->data = array(byte, 0);
  }

  inst->cursor = string_len;

  return true;
}

void Buffer_Rewind(Buffer *const inst)
{
  if (!inst) {
    return;
  }

  inst->cursor = 0;
}

void Buffer_Clear(Buffer *const inst)
{
  if (!inst || !inst->data) {
    return;
  }

  fill(Array(byte), inst->data, 0);
  Buffer_Rewind(inst);
}

boolean Buffer_Append(Buffer *const inst, const byte value)
{
  if (!inst) {
    return false;
  }

  /* Automatically expand the buffer array if capacity is reached. */
  if (inst->cursor >= capacity(Array(byte), inst->data)) {
    inst->data = grow(Array(byte), inst->data);
    if (!inst->data) {
      return false;
    }
  }

  set(Array(byte), inst->data, inst->cursor, value);
  inst->cursor++;

  return true;
}

boolean Buffer_Write(Buffer *const inst, const String *const value)
{
  if (!inst || !value) {
    return false;
  }

  const llong value_len = Length(String, value);
  if (!value_len) {
    return false;
  }

  /* Validate capacity and dynamically scale as needed. */
  while (inst->cursor + value_len > capacity(Array(byte), inst->data)) {
    inst->data = grow(Array(byte), inst->data);
    if (!inst->data) {
      return false;
    }
  }

  loop (i, value_len) {
    set(Array(byte), inst->data, inst->cursor + i, getbyte(value, i));
  }
  inst->cursor += value_len;

  return true;
}

String *Buffer_Literalise(Buffer *const inst)
{
  if (!inst) {
    return nll;
  }

  return string((char *)Getter(Array(byte), Data, inst->data));
}

inline Array(byte) *Buffer_GetData(const Buffer *const inst)
{
  if (!inst) return null;
  return inst->data;
}

inline llong Buffer_GetCursor(const Buffer *const inst)
{
  if (!inst) return 0;
  return inst->cursor;
}

IMPL_ARRAY(Buffer)
IMPL_ARRAY_LITERALISE(Buffer)
