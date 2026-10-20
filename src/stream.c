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
  llong reading_offset;
};

Stream _StdIn = Empty;
Stream _StdOut = Empty;
Stream _StdErr = Empty;

Stream *StdIn = &_StdIn;
Stream *StdOut = &_StdOut;
Stream *StdErr = &_StdErr;

/* System streams do not have filepath or mode. */
static inline boolean IsSystemStream(const Stream *const inst)
{
  return inst && !inst->filepath && !inst->mode;
}

static inline boolean IsImmediateStream(const Stream *const inst)
{
  return inst && !inst->buffer;
}

static boolean Flush(Stream *const inst)
{
  if (!inst || !inst->fileptr) {
    return false;
  }

  /* Immediate streams don't give users chances to flush
   * before they output the bytes directly to the destination already. */
  if (IsImmediateStream(inst)) {
    return true;
  }

  const llong cursor = Getter(Buffer, Cursor, inst->buffer);

  if (cursor > 0) {
    fwrite(
      ref(Array(byte), Getter(Buffer, Data, inst->buffer), 0),
      sizeof(byte),
      cursor,
      inst->fileptr
    );
    call(Buffer, Clear, inst->buffer);
  }

  return true;
}

static inline boolean Reset(Stream *const inst)
{
  return inst && inst->fileptr && !fseek(inst->fileptr, 0, SEEK_CUR);
}

static boolean Open(Stream *const inst)
{
  if (!inst) {
    return false;
  }

  if (IsSystemStream(inst) || inst->fileptr) {
    return true;
  }

  char *const filepath_cstr = flatten(char, inst->filepath);
  char *const mode_cstr = flatten(char, inst->mode);

  inst->fileptr = fopen(filepath_cstr, mode_cstr);

  Deallocate(filepath_cstr);
  Deallocate(mode_cstr);

  /* TODO: Temporal solution applied before fully integrating error handling
   * for Compound. */
  if (!inst->fileptr) {
    fprintf(stderr, "Cannot open file."NL);
    return false;
  }

  Array(byte) *buffer = Getter(Buffer, Data, inst->buffer);
  const int buflen = Length(Array(byte), buffer);
  char *raw_buf = (char *)Getter(Array(byte), Data, buffer);

  /*  Only attach custom buffer if it is valid and non-empty. */
  if (raw_buf && buflen > 0) {
    /* You must call setvbuf() after opening the stream and
     * before performing any read or write operations. */
    setvbuf(
      inst->fileptr,
      raw_buf,
      _IONBF,
      buflen
    );
  }

  return true;
}

static boolean Close(Stream *const inst)
{
  if (!inst || !inst->fileptr) {
    return false;
  }

  if (IsSystemStream(inst)) {
    return true;
  }

  Flush(inst);

  Reset(inst);

  fclose(inst->fileptr);
  inst->fileptr = null;

  return true;
}

void InitialiseStream(void)
{
  _StdIn = (Stream) {
    .buffer = Create(Buffer, STREAM_INITIAL_BUFFER_SIZE),
    .filepath = nll,
    .mode = nll,
    .fileptr = stdin
  };
  _StdOut = (Stream) {
    .buffer = Create(Buffer, STREAM_INITIAL_BUFFER_SIZE),
    .filepath = nll,
    .mode = nll,
    .fileptr = stdout
  };
  _StdErr = (Stream) {
    .buffer = nll,
    .filepath = nll,
    .mode = nll,
    .fileptr = stderr
  };

  if (!StdIn->buffer || !StdOut->buffer) {
    fprintf(
      stderr,
      "Failed to initialise system streams during stream buffer allocations: "NL
      "StdIn->buffer = (%p), StdOut->buffer = (%p) and StdErr->buffer = (%p)."NL
      , (void *)StdIn->buffer, (void *)StdOut->buffer, (void *)StdErr->buffer
    );

    exit(1);
  }
}

void DeinitialiseStream(void)
{
  Flush(StdIn);
  Delete(Buffer, StdIn->buffer);
  StdIn->fileptr = nll;

  Flush(StdOut);
  Delete(Buffer, StdOut->buffer);
  StdOut->fileptr = nll;

  Flush(StdErr);
  StdErr->fileptr = nll;
}

Stream *Stream_Create(String *const filepath, String *const mode)
{
  if (!filepath || !mode) {
    return null;
  }

  Stream *const inst = Allocate(sizeof(Stream));
  if (!inst) {
    return null;
  }

  inst->buffer = Create(Buffer, STREAM_INITIAL_BUFFER_SIZE);
  inst->filepath = CopyOf(String, filepath);
  inst->mode = CopyOf(String, mode);
  inst->fileptr = null;
  inst->reading_offset = 0;

  return inst;
}

Stream *Stream_CopyOf(const Stream *const other)
{
  if (!other) {
    return null;
  }

  Stream *const inst = Allocate(sizeof(Stream));
  if (!inst) {
    return null;
  }

  inst->buffer = CopyOf(Buffer, other->buffer);
  inst->filepath = CopyOf(String, other->filepath);
  inst->mode = CopyOf(String, other->mode);
  inst->fileptr = other->fileptr;
  inst->reading_offset = other->reading_offset;

  if (!inst->buffer || !inst->filepath || !inst->mode) {
    Delete(Buffer, inst->buffer);
    Delete(String, inst->filepath);
    Delete(String, inst->mode);
    return nll;
  }

  return inst;
}

void Stream_Delete(Stream *const inst)
{
  if (!inst) {
    return;
  }

  if (inst->fileptr && !IsSystemStream(inst)) {
    Close(inst);
  }

  Flush(inst);

  Delete(Buffer, inst->buffer);
  Delete(String, inst->filepath);
  Delete(String, inst->mode);
  Deallocate(inst);
}

boolean Stream_Equals(const Stream *const inst, const Stream *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  return Equals(Buffer, inst->buffer, other->buffer)
      && Equals(String, inst->filepath, other->filepath)
      && Equals(String, inst->mode, other->mode);
}

boolean Stream_Write(Stream *const inst, const String *const value)
{
  if (!inst || !value) {
    return false;
  }

  const llong value_len = Length(String, value);
  if (!value_len) {
    return true;
  }

  Open(inst);

  const int valuelen = Length(String, value);
  const int valuewidth = Getter(String, Width, value);

  /* Immediate streams require direct output on-scene. */
  if (IsImmediateStream(inst)) {
    return valuelen && fwrite(
      Getter(Array(byte), Data, Getter(String, Data, value)),
      valuewidth,
      valuelen,
      inst->fileptr
    );
  }

  Array(byte) *const data = Getter(Buffer, Data, inst->buffer);
  const llong cap = capacity(Array(byte), data);
  loop (i, value_len) {
    /* Trigger automatic flush if capacity is reached or is an immediate stream that has no buffer. */
    if (Getter(Buffer, Cursor, inst->buffer) >= cap) {
      Flush(inst);
    }

    call(Buffer, Append, inst->buffer, getbyte(value, i));
  }

  Close(inst);

  return true;
}

boolean Stream_WriteLine(
  Stream *const inst,
  String *const value
) {
  if (!inst || !value) {
    return false;
  }

  String *const newline = string(NL);

  String *content = Concat(String, value, newline);
  if (!content) {
    return false;
  }

  Delete(String, newline);

  return Stream_Write(inst, content);
}

String *Stream_Read(Stream *const inst)
{
  if (!inst) {
    return nll;
  }

  const boolean is_system_stream = IsSystemStream(inst);
  if ((!is_system_stream && !Open(inst)) || !inst->fileptr) {
    return nll;
  }

  Buffer *const buffer = Create(Buffer, STREAM_INITIAL_BUFFER_SIZE);
  if (!buffer) {
    if (!is_system_stream) {
      Close(inst);
    }

    return nll;
  }

  byte chunk[STREAM_INITIAL_BUFFER_SIZE] = Empty;
  size_t read = 0;

  while (true) {
    read = fread(chunk, sizeof(byte), STREAM_INITIAL_BUFFER_SIZE,inst->fileptr);
    if (read <= 0) {
      break;
    }

    loop (i, (int)read) {
      call(Buffer, Append, buffer, chunk[i]);
    }
  }

  // if (ferror(inst->fileptr)) {
  //   /* TODO(william): Emit error Status here. */
  // }

  if (!is_system_stream) {
    Close(inst);
  }

  /* Guarantee null-termination when total read is an exact capacity multiple. */
  call(Buffer, Append, buffer, 0);

  String *const actualised = lit(Buffer, buffer);

  Delete(Buffer, buffer);

  return actualised;
}

Array(String) *Stream_ReadLines(Stream *const inst)
{
  if (!inst) {
    return nll;
  }

  String *read = Stream_Read(inst);
  if (!read) {
    return nll;
  }

  return fragmentise(read, NEWLINE);
}

String *Stream_Literalise(Stream *const inst)
{
  if (!inst) {
    return nll;
  }

  return append(
    nll,
    string("Stream: "),
    CopyOf(String, inst->filepath),
    format(" @%p", inst)
  );
}

IMPL_ARRAY(Stream)
IMPL_ARRAY_LITERALISE(Stream)
