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

/** @file stream.h */

#ifndef COMPOUND_STREAM_H
# define COMPOUND_STREAM_H

# include <stdio.h>

# include "buffer.h"

# define STREAM_INITIAL_BUFFER_SIZE  4096LL

typedef struct Stream Stream;

ARRAY(Stream)
LITERALISE(Stream)

# define stream(filename_cstr, modes_cstr)                                     \
  Create(Stream, string(filename_cstr), string(modes_cstr))

# define Write(stream_ptr, value_string_ptr)                                   \
  Stream_Write(stream_ptr, value_string_ptr)

# define WriteLine(stream_ptr, value_string_ptr)                               \
  Stream_WriteLine(stream_ptr, value_string_ptr)

# define Read(stream_ptr)                                                      \
  Stream_Read(stream_ptr)

# define ReadLines(stream_ptr)                                                 \
  Stream_ReadLines(stream_ptr)

extern Stream *StdIn, *StdOut, *StdErr;

void InitialiseStream(void);
void DeinitialiseStream(void);

Stream *Stream_Create(String *const filepath, String *const mode);
Stream *Stream_CopyOf(const Stream *const other);
void Stream_Delete(Stream *const inst);
boolean Stream_Equals(const Stream *const inst, const Stream *const other);

boolean Stream_Write(Stream *const inst, const String *const value);
boolean Stream_WriteLine(
  Stream *const inst,
  String *const value
);
String *Stream_Read(Stream *const inst);
Array(String) *Stream_ReadLines(Stream *const inst);

#endif /* COMPOUND_STREAM_H */
