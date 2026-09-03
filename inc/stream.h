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

# define INITIAL_STREAM_BUFFER_SIZE  4096LL

typedef struct Stream Stream;

ARRAY(Stream)
LITERALISE(Stream)

# define stream(filename_cstr, modes_cstr)\
  Create(Stream, string(filename_cstr), string(modes_cstr))

# define Open(stream_ptr)\
  Stream_Open(stream_ptr)

# define Close(stream_ptr)\
  Stream_Close(stream_ptr)

# define Flush(stream_ptr)\
  Stream_Flush(stream_ptr)

# define Write(stream_ptr, value_ptr)\
  Stream_Write(stream_ptr, value_ptr)

# define WriteLine(stream_ptr, value_ptr)\
  Stream_WriteLine(stream_ptr, value_ptr)

# define Read(stream_ptr)\
  Stream_Read(stream_ptr)

# define ReadLine(stream_ptr, lines_to_skip)\
  Stream_ReadLine(stream_ptr, lines_to_skip)

Stream *Stream_Create(const String *const filepath, const String *const mode);
Stream *Stream_CopyOf(const Stream *const other);
void Stream_Delete(Stream *const inst);
boolean Stream_Equals(const Stream *const obj1, const Stream *const obj2);

boolean Stream_Open(Stream *const inst);
boolean Stream_Close(Stream *const inst);
boolean Stream_Flush(Stream *const inst);

boolean Stream_Write(Stream *const inst, const String *const value);
boolean Stream_WriteLine(
  Stream *const inst,
  const String *const value,
  const llong lines_to_skip
);
String *Stream_Read(Stream *const inst);
String *Stream_ReadLine(Stream *const inst, const llong lines_to_skip);

#endif /* COMPOUND_STREAM_H */
