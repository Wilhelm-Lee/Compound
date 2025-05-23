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

#ifndef COMPOUND_STREAM_H
# define COMPOUND_STREAM_H

# include "array.h"
# include "arrays.h"
# include "common.h"
# include "platform.h"
# include "status.h"
# include "string.h"

# define Stream(type)  type##Stream

# define DEFINE_STREAM(type)\
  typedef struct {                                         \
    type source;                                           \
    String buffer;                                         \
    boolean opened;                                           \
  } Stream(type);

# define FUNC_STREAM(type)\
  Status type##Stream_Create(Stream(type) *inst, const type source);\
  Status type##Stream_CopyOf(Stream(type) *inst, const Stream(type) other);\
  Status type##Stream_Delete(Stream(type) *inst);          \
  Status type##Stream_Open(Stream(type) *inst);            \
  Status type##Stream_Close(Stream(type) *inst);           \
  Status type##Stream_Read(Stream(type) *inst, const size_t maxbyte, String *buff);\
  Status type##Stream_Write(Stream(type) *inst, const size_t maxbyte, String *buff);\
  Status type##Stream_Flush(Stream(type) *inst);

# define GLOBAL_STREAM(type)                               \
  static type##Stream *Global##type##Stream = NULL;        \
  static inline void type##Stream_SetGlobal(type##Stream *stream)\
  {                                                        \
    Global##type##Stream = stream;                         \
  };

# define STREAM(type)                                      \
  FUNC_STREAM(type)                                        \
  GLOBAL_STREAM(type)

# define opened(stream)                                    \
  ((stream).opened)



#endif  /* COMPOUND_STREAM_H */
