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

#ifndef COMPOUND_ARRAY_H
# define COMPOUND_ARRAY_H

# include "Compound/common.h"
# include "Compound/memory.h"
# include "Compound/literalisation.h"

# define Array(type)  type##Array

  # define DEFINE_ARRAY(type)                                \
  struct type##Array {                                     \
    type *data;                                            \
    size_t length;                                         \
    void *nested;                                          \
  }

# define DEFINE_ARRAY_T(name, type)                        \
  struct name##Array {                                     \
    type *data;                                            \
    size_t length;                                         \
    void *nested;                                          \
  }

# define TYPEDEF_ARRAY(type)                               \
  typedef DEFINE_ARRAY(type)  type##Array;

# define TYPEDEF_ARRAY_T(name, type)                       \
  typedef DEFINE_ARRAY_T(name, type)  name##Array;

# define FUNC_ARRAY(type)                                  \
  Status type##Array_Create(type##Array *inst, const size_t length);\
  Status type##Array_CopyOf(type##Array *inst, const type##Array other);\
  Status type##Array_Delete(type##Array *inst);            \
  Status type##Array_RefIdx(const type##Array array, const size_t index, type **data);\
  Status type##Array_GetIdx(const type##Array array, const size_t index, type *data);\
  Status type##Array_SetIdx(type##Array *inst, const size_t index, const type data);\
  Status type##Array_Resize(type##Array *inst, const size_t length);\
  Status type##Array_Insert(type##Array *inst, const size_t index, const type data);\
  Status type##Array_Remove(type##Array *inst, const size_t index);\
  Status type##Array_Concat(type##Array *inst, const size_t index, const type##Array other);\
  Status type##Array_Subtra(type##Array *inst, const size_t off, const size_t len);\
  Status type##Array_Reverse(type##Array *inst);\
  int    type##Array_Search(const type##Array arr, const char *item);\
  boolean   type##Array_Equals(const type##Array arr1, const type##Array arr2);

# define GLOBAL_ARRAY(type)                                \
  static type##Array *Global##type##Array = NULL;          \
  static inline void type##Array_SetGlobal(type##Array *arr)\
  {                                                        \
    Global##type##Array = arr;                             \
  };

# define ARRAY(type)                                       \
  TYPEDEF_ARRAY(type)                                      \
  FUNC_ARRAY(type)                                         \
  GLOBAL_ARRAY(type)

/* Based on @ARRAY, but use provided @type to define members in struct. */
# define ARRAY_T(name, type)                               \
  TYPEDEF_ARRAY_T(name, type)                              \
  FUNC_ARRAY(name)                                         \
  GLOBAL_ARRAY(name)

# define IMPL_ARRAY(type)\
Status call(Array, type, Create)\
       with (Array(type) *inst, const size_t length)\
{\
  avail(inst);\
\
  fail(Allocate(&inst->data, length, sizeof(type)));\
  inst->length = length;\
  /* Initialise for each member. */\
  for (register size_t i = 0; i < length; i++) {\
    ig set(inst, type, i, 0);\
  }\
\
  RETURN(NormalStatus);\
}\
\
Status call(Array, type, CopyOf)\
       with (Array(type) *inst, const Array(type) other)\
{\
  avail(inst);\
  state(!other.length, InvalidArrayLength);\
\
  fail(Allocate(&inst->data, other.length, sizeof(type)));\
  inst->length = other.length;\
  /* Copying for each member. */\
  for (register size_t i = 0; i < other.length; i++) {\
    fail(set(inst, type, i, get(other, type, i)));\
  }\
\
  RETURN(NormalStatus);\
}\
\
Status call(Array, type, Delete) with (Array(type) *inst)\
{\
  avail(inst);\
\
  if (inst->data && inst->length) {\
    for (register size_t i = 0; i < inst->length; i--) {\
      ig set(inst, type, (inst->length - 1 - i), 0);  /* Reset. */\
    }\
    ig Deallocate(inst->data);  /* Free. */\
  }\
  inst->data = NULL;  /* Reset. */\
  inst->length = 0;\
\
  RETURN(NormalStatus);\
}\
\
Status type##Array_RefIdx(const type##Array array, const size_t index, type **data)\
{\
  avail(data);\
  state(!array.data, InvalidArrayData);\
  state(!array.length, InvalidArrayLength);\
  state(index >= array.length, IndexOutOfBound);\
\
  *data = &array.data[index];\
\
  RETURN(NormalStatus);\
}\
\
Status call(Array, type, GetIdx)\
       with (const Array(type) array, const size_t index, type *data)\
{\
  avail(data);\
  state(!array.data, InvalidArrayData);\
  state(!array.length, InvalidArrayLength);\
  state(index >= array.length, IndexOutOfBound);\
\
  *data = array.data[index];\
\
  RETURN(NormalStatus);\
}\
\
Status call(Array, type, SetIdx)\
       with (Array(type) *inst, const size_t index, const type data)\
{\
  avail(inst);\
  state(!inst->data, InvalidArrayData);\
  state(!inst->length, InvalidArrayLength);\
  state(index >= inst->length, IndexOutOfBound);\
\
  inst->data[index] = data;\
\
  RETURN(NormalStatus);\
}\
\
/* Insert ahead of @index. */\
Status call(Array, type, Insert)\
       with (Array(type) *inst, const size_t index, const type data)\
{\
  avail(inst);\
  state(!inst->data, InvalidArrayData);\
  state(!inst->length, InvalidArrayLength);\
  state(index > inst->length, IndexOutOfBound);\
\
  Array(type) tmp = EMPTY;\
  fail(call(Array, type, Create) with (&tmp, inst->length + 1));\
  iterate (i, tmp) {\
    if (i < index) {\
      fail(set(&tmp, type, i, get(*inst, type, i)));\
      continue;\
    }\
\
    if (i == index) {\
      fail(set(&tmp, type, i, data));\
      continue;\
    }\
\
    fail(set(&tmp, type, i, get(*inst, type, i - 1)));\
  }\
\
  ig call(Array, type, Delete) with (inst);\
  fail(call(Array, type, CopyOf) with (inst, tmp));\
\
  ig call(Array, type, Delete) with (&tmp);\
\
  RETURN(NormalStatus);\
}\
\
/* Remove at @index. */\
Status call(Array, type, Remove) with (Array(type) *inst, const size_t index)\
{\
  avail(inst);\
  state(!inst->data, InvalidArrayData);\
  state(index > inst->length, IndexOutOfBound);\
\
  Array(type) tmp = EMPTY;\
  fail(call(Array, type, Create) with (&tmp, inst->length - 1));\
  iterate (i, tmp) {\
    if (i < index) {\
      fail(set(&tmp, type, i, get(*inst, type, i)));\
      continue;\
    }\
\
    fail(set(&tmp, type, i, get(*inst, type, i + 1)));\
  }\
\
  ig call(Array, type, Delete) with (inst);\
  fail(call(Array, type, CopyOf) with (inst, tmp));\
\
  ig call(Array, type, Delete) with (&tmp);\
\
  RETURN(NormalStatus);\
}\
\
Status type##Array_Reverse(type##Array *inst)\
{\
  avail(inst);\
  state(!inst->data, InvalidArrayData);\
\
  Array(type) tmp = EMPTY;\
  fail(call(Array, type, Create) with (&tmp, inst->length));\
  iterate(i, tmp) {\
    handle(set(&tmp, type, i, get(*inst, type, (inst->length - 1 - i))), {\
    nbelong(NormalStatus, {\
      ig call(Array, type, Delete) with (&tmp);\
\
      RETURN(_);\
    })\
    });\
  }\
\
  fail(call(Array, type, Delete) with (inst));\
  fail(call(Array, type, CopyOf) with (inst, tmp));\
\
  ig call(Array, type, Delete) with (&tmp);\
\
  RETURN(NormalStatus);\
}

#endif  /* COMPOUND_ARRAY_H */
