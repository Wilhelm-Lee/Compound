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

/** @file stream.c. */

#include "../inc/stream.h"

struct Stream {
  Buffer *buffer;
  String *filepath;
  String *mode;
  FILE *fileptr;
};

Stream *Stream_Create(const String *const filepath, const String *const mode)
{
  if (!filepath || !mode) {
    return null;
  }

  Stream *const inst = Allocate(1, sizeof(Stream));
  if (!inst) {
    return null;
  }

  inst->buffer = Create(Buffer, INITIAL_STREAM_BUFFER_SIZE);
  inst->filepath = CopyOf(String, filepath);
  inst->mode = CopyOf(String, mode);
  inst->fileptr = null;

  return inst;
}

Stream *Stream_CopyOf(const Stream *const other)
{
  if (!other) {
    return null;
  }

  Stream *const inst = Allocate(1, sizeof(Stream));
  if (!inst) {
    return null;
  }

  inst->buffer = CopyOf(Buffer, other->buffer);
  inst->filepath = CopyOf(String, other->filepath);
  inst->mode = CopyOf(String, other->mode);
  inst->fileptr = other->fileptr;

  return inst;
}

void Stream_Delete(Stream *const inst)
{
  if (!inst) {
    return;
  }

  if (inst->fileptr) {
    Stream_Close(inst);
  }

  Delete(Buffer, inst->buffer);
  Delete(String, inst->filepath);
  Delete(String, inst->mode);
  Deallocate(inst);
}

boolean Stream_Equals(const Stream *const obj1, const Stream *const obj2)
{
  if (!obj1 || !obj2) return false;
  if (obj1 == obj2) return true;

  return Equals(String, obj1->filepath, obj2->filepath) &&
         Equals(String, obj1->mode, obj2->mode);
}

boolean Stream_Open(Stream *const inst)
{
  if (!inst) {
    return false;
  }

  char *const filepath_cstr = flatten(char, inst->filepath);
  char *const mode_cstr = flatten(char, inst->mode);

  inst->fileptr = fopen(filepath_cstr, mode_cstr);

  Deallocate(filepath_cstr);
  Deallocate(mode_cstr);

  if (!inst->fileptr) {
    fprintf(stderr, "Cannot open file."NL);
    return false;
  }

  return true;
}

boolean Stream_Close(Stream *const inst)
{
  if (!inst || !inst->fileptr) {
    return false;
  }

  Flush(inst);

  fclose(inst->fileptr);
  inst->fileptr = null;

  return true;
}

boolean Stream_Flush(Stream *const inst)
{
  if (!inst || !inst->fileptr) {
    return false;
  }

  const llong cursor = Getter(Buffer, Cursor, inst->buffer);

  if (cursor > 0) {
    Array(byte) *const data = Getter(Buffer, Data, inst->buffer);
    fwrite(ref(Array(byte), data, 0), sizeof(byte), cursor, inst->fileptr);
    call(Buffer, Clear, inst->buffer);
  }

  return true;
}

boolean Stream_Write(Stream *const inst, const String *const value)
{
  /* Added strict fileptr validation to match Stream_Read */
  if (!inst || !inst->fileptr || !value) {
    return false;
  }

  const llong value_len = Length(String, value);
  if (!value_len) {
    return true;
  }

  /* C standard requires a positioning operation when transitioning
   * from reading to writing. If the buffer is currently empty (Cursor == 0),
   * we are beginning a new write batch directly after a flush or a read
   * operation. This is the perfect moment to safely sync the stream state. */
  if (Getter(Buffer, Cursor, inst->buffer) == 0) {
    fseek(inst->fileptr, 0, SEEK_CUR);
  }

  Array(byte) *const data = Getter(Buffer, Data, inst->buffer);
  const llong cap = capacity(Array(byte), data);

  loop (i, value_len) {
    /* Trigger automatic flush if capacity is reached. */
    if (Getter(Buffer, Cursor, inst->buffer) >= cap) {
      Flush(inst);
    }

    call(Buffer, Append, inst->buffer, getbyte(value, i));
  }

  return true;
}

boolean Stream_WriteLine(
  Stream *const inst,
  const String *const value,
  const llong lines_to_skip
) {
  if (!inst || !inst->fileptr) {
    return false;
  }

  /* Phase 1: Skip existing lines if requested. */
  if (lines_to_skip > 0) {
    /* Flush any pending writes in our Buffer before switching to system reads. */
    Stream_Flush(inst);

    register llong skip_countdown = lines_to_skip;
    while (skip_countdown > 0) {
      byte read_byte = 0;
      size_t count = fread(&read_byte, sizeof(byte), 1, inst->fileptr);

      if (count == 0 || feof(inst->fileptr) || ferror(inst->fileptr)) {
        break; /* Hit EOF or error before finishing our skips. */
      }

      if (read_byte == '\n') {
        skip_countdown--;
      }
    }

    /* C standard requires a positioning operation between read and write. */
    fseek(inst->fileptr, 0, SEEK_CUR);
  }

  /* Phase 2: Write the target line. */
  String *newline = string(NEWLINE);
  const llong value_len = Length(String, value);

  if (!value_len) {
    Stream_Write(inst, newline);
    Delete(String, newline);

    return true;
  }

  Stream_Write(inst, value);
  Stream_Write(inst, newline);

  Delete(String, newline);

  return true;
}

String *Stream_Read(Stream *const inst)
{
  if (!inst || !inst->fileptr) {
    return null;
  }

  /* Push any pending writes from our custom buffer to the OS. */
  Stream_Flush(inst);

  /* C standard requires a positioning operation between write and read. */
  fseek(inst->fileptr, 0, SEEK_CUR);

  boolean chars_read = false;
  while (true) {
    byte read_byte = 0;
    size_t count = fread(&read_byte, sizeof(byte), 1, inst->fileptr);

    if (count == 0 || feof(inst->fileptr) || ferror(inst->fileptr)) {
      /* Return null if we hit EOF immediately without reading anything. */
      if (!chars_read && Getter(Buffer, Cursor, inst->buffer) == 0) { /**/
        return null;
      }
      break;
    }

    chars_read = true;
    call(Buffer, Append, inst->buffer, read_byte);
  }

  const llong cursor = Getter(Buffer, Cursor, inst->buffer);
  Array(byte) *const data = Getter(Buffer, Data, inst->buffer);

  char *c_str = Allocate(cursor + 1, sizeof(char));
  if (cursor > 0) {
    memmove(c_str, ref(Array(byte), data, 0), cursor);
  }
  c_str[cursor] = '\0';

  String *value = string(c_str);

  Deallocate(c_str);
  call(Buffer, Clear, inst->buffer);

  return value;
}

String *Stream_ReadLine(Stream *const inst, const llong lines_to_skip)
{
  if (!inst || !inst->fileptr) {
    return null;
  }

  /* Push any pending writes from our custom buffer to the system. */
  Flush(inst);

  /* C standard requires a positioning operation or fflush between write and read. */
  fseek(inst->fileptr, 0, SEEK_CUR);

  register llong skip_countdown = lines_to_skip;

  /* Still skipped. */
  while (skip_countdown > 0) {
    byte read_byte = 0;
    size_t count = fread(&read_byte, sizeof(byte), 1, inst->fileptr);

    /* EOF. */
    if (count == 0 || feof(inst->fileptr) || ferror(inst->fileptr)) {
      return null;
    }

    if (read_byte == '\n') {
      skip_countdown--;
    }
  }

  /* Read the actual target line. */
  boolean chars_read = false;
  while (true) {
    byte read_byte = 0;
    size_t count = fread(&read_byte, sizeof(byte), 1, inst->fileptr);

    if (count == 0 || feof(inst->fileptr) || ferror(inst->fileptr)) {
      if (!chars_read && Getter(Buffer, Cursor, inst->buffer) == 0) {
        return null;
      }
      break;
    }

    if (read_byte == '\n') {
      break;
    }

    if (read_byte == '\r') {
      continue;
    }

    chars_read = true;
    call(Buffer, Append, inst->buffer, read_byte);
  }

  const llong cursor = Getter(Buffer, Cursor, inst->buffer);
  Array(byte) *const data = Getter(Buffer, Data, inst->buffer);

  char *c_str = Allocate(cursor + 1, sizeof(char));
  if (cursor > 0) {
    memmove(c_str, ref(Array(byte), data, 0), cursor);
  }
  c_str[cursor] = '\0';

  String *value = string(c_str);

  call(Buffer, Clear, inst->buffer);

  return value;
}

String *Stream_Literalise(Stream *const inst)
{
  if (!inst) {
    return nll;
  }

  return append(string("Stream: "), inst->filepath);
}

IMPL_ARRAY(Stream)
IMPL_ARRAY_LITERALISE(Stream)
