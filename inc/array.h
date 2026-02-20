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

/** @file array.h */

#ifndef COMPOUND_ARRAY_H
# define COMPOUND_ARRAY_H

# include <stdarg.h>

# include "common.h"
# include "memory.h"

/* Returns the indexer respecting to @reversed. */
/* Formula: |(R * L - I)| - R, R = { x | 0, 1 }, I (= N */
# define at(array_ptr, index)\
  (llabs(((array_ptr)->reversed *\
          (array_ptr)->capacity) - (index)) - (array_ptr)->reversed)

# define ref(type, array_ptr, index)\
  (type##Array_RefIdx((array_ptr), (index)))

# define get(type, array_ptr, index)\
  (type##Array_GetIdx((array_ptr), (index)))

# define set(type, array_ptr, index, object)\
  (type##Array_SetIdx((array_ptr), (index), (object)))

# define iterate(it, array_ptr)\
  loop (it, (array_ptr)->capacity)

# define foreach(type, it, array_ptr, block)\
  {\
    type it = EMPTY;\
    iterate (_foreach_##type##_idx, (array_ptr)) {\
      it = get(type, (array_ptr), _foreach_##type##_idx);\
      block\
    }\
  }

# define refeach(type, it, array_ptr, block)\
  {\
    type *it = EMPTY;\
    iterate (_refeach_##type##_idx, (array_ptr)) {\
      it = ref(type, (array_ptr), _refeach_##type##_idx);\
      block\
    }\
  }

# define compose(type, ...)\
  ({\
    Array(type) _compose = EMPTY;\
    call(Array, type, Compose)\
         with (&_compose, arglen(__VA_ARGS__), __VA_ARGS__));\
    _compose;\
  })

# define zip(...)\
  compose(ptr, __VA_ARGS__)

/* Ensure the accessing on @arrayA is compatible
   with the respect for capacity of @arrayB. */
# define parallel(type, idx, arrayA_ptr, arrayB_ptr, block)\
  iterate (idx, arrayA_ptr) {\
    type *const A = ref(type, arrayA_ptr, idx);\
    type *const B = ref(type, arrayB_ptr, idx);\
    block;\
  }

# define _parallel(idx, block, ...)\
  ({\
    Array _##idx##_parallel = zip(__VA_ARGS__);\
    iterate (idx, _##idx##_parallel)\
      block\
    Delete(Array)) with (&_##idx##_parallel);\
  })

# define fillup(type, array_ptr, value)\
  {\
    type _fillup_value = (value);\
    refeach (type, _fillup, array_ptr, {\
      *_fillup = (_fillup_value);\
    })\
  }

# define assign(type, arraydst_ptr, arraysrc_ptr)\
  parallel (type, _assign_idx, arraydst_ptr, arraysrc_ptr, {\
    *A = *B;\
  })

# define clone(type, arrayA, arrayB)\
  parallel (type, _clone_idx, arrayA, arrayB, {\
    CopyOf(type) with (A, *B));\
  })

/* Delete elements in @array_ptr. */
# define erase(type, array_ptr)\
  refeach (type, _erase##array_ptr, array_ptr, {\
    Delete(type), (_erase##array_ptr));\
  })

/* Delete elements in @array_ptr by reference. */
# define eraseref(type, array_ptr)\
  iterate (i##array_ptr, array_ptr) {\
    Delete(type, (type **)ref(ptr, array_ptr, i##array_ptr));\
  }

# define reverse(array_ptr)\
  ((array_ptr)->reversed = !(array_ptr)->reversed, (array_ptr))

# define array(type, capacity)\
  (Create(Array(type), (capacity)))

# define empty(type)\
  ({\
    Array(type) _empty = EMPTY;\
    Create(Array(type)) with (&_empty, 0));\
    _empty;\
  })

# define last(array_ptr)\
  ((array_ptr)->capacity)

# define grow(type, array_ptr)\
  call(Array, type, Resize)\
         with (&(array_ptr), (llong)((array_ptr)->capacity * 1.5)))

# define resize(type, array_ptr_ptr, capacity)\
  call (Array, type, Resize) with (array_ptr_ptr, capacity)

# define Array(type)\
  type##Array

# define DEFINE_ARRAY(type)\
  DEFINE_ARRAY_T(type, type)

# define DEFINE_ARRAY_T(name, type)\
  struct name##Array {\
    type *data;\
    llong capacity;\
    boolean reserved;\
    boolean reversed;\
  }

# define TYPEDEF_ARRAY(name)\
  typedef DEFINE_ARRAY(name)  name##Array;

# define FUNC_ARRAY(name)\
  Array(name) *name##Array_Create(const llong capacity);\
  Array(name) *name##Array_CopyOf(const Array(name) *const other);\
  void name##Array_Delete(Array(name) **const inst);\
  name *name##Array_RefIdx(const Array(name) *const inst, const llong index);\
  name name##Array_GetIdx(const Array(name) *const inst, const llong index);\
  void name##Array_SetIdx(const Array(name) *const inst,\
                          const llong index, const name data);\
  Array(name)*name##Array_Resize(Array(name)**const inst,const llong capacity);\
  Array(name) *name##Array_Insert(Array(name) **const inst, const llong index,\
                                  const name data);\
  Array(name) *name##Array_Remove(Array(name) **const inst, const llong index);\
  boolean name##Array_Equals(\
    const Array(name) *const arr1,\
    const Array(name) *const arr2,\
    boolean (*const IsEqual)(const name *const A, const name *const B)\
  );

/*
  Status name##Array_Concat(Array(name) *const inst, const llong index,
                            const Array(name) other);
  Status name##Array_Compose(Array(name) *const inst, const llong capacity,...);
*/

# define ARRAY(type)\
  TYPEDEF_ARRAY(type)\
  FUNC_ARRAY(type)

# define IMPL_ARRAY(type, EqualsSolution)\
Array(type) *type##Array_Create(const llong capacity)\
{\
  if (capacity < 0) {\
    return NULL;\
  }\
\
  Array(type) *array = Allocate(1, sizeof(Array(type)));\
  \
  array->data = Allocate(capacity, sizeof(type));\
  array->capacity = capacity;\
  array->reserved = (!capacity);\
  array->reversed = false;\
\
  return array;\
}\
\
Array(type) *type##Array_CopyOf(const Array(type) *const other)\
{\
  if (!other) {\
    return NULL;\
  }\
\
  Array(type) *array = Allocate(1, sizeof(Array(type)));\
\
  array->data = Allocate(other->capacity, sizeof(type));\
\
  array->capacity = other->capacity;\
  array->reserved = other->reserved;\
  array->reversed = other->reversed;\
\
  /* Copying every member. */\
  loop (i, other->capacity) {\
    set(type, array, i, get(type, other, i));\
  }\
\
  return array;\
}\
\
void type##Array_Delete(Array(type) **const inst)\
{\
  if (!inst || !*inst) {\
    return;\
  }\
\
  Deallocate((*inst)->data);\
  (*inst)->data = NULL;\
  (*inst)->capacity = 0;\
  (*inst)->reserved = false;\
  (*inst)->reversed = false;\
  Deallocate(*inst);\
  *inst = NULL;\
}\
/* Fetch for the address of index-given object in @array. */\
inline type *type##Array_RefIdx(const Array(type) *const inst, const llong index)\
{\
  if (!inst) {\
    return NULL;\
  }\
  \
  if (index >= inst->capacity) {\
    return NULL;\
  }\
  \
  return (&inst->data[index]);\
}\
\
inline type type##Array_GetIdx(const Array(type) *const inst,const llong index)\
{\
  type *const ref = type##Array_RefIdx(inst, index);\
  \
  if (!ref) {\
    return (type)EMPTY;\
  }\
  \
  return *ref;\
}\
inline void type##Array_SetIdx(const Array(type) *const inst,\
  const llong index, const type data)\
{\
  type *const ref = type##Array_RefIdx(inst, index);\
  \
  if (!ref) {\
    return;\
  }\
  \
  *ref = (data);\
}\
\
Array(type) *type##Array_Resize(Array(type) **const inst, const llong capacity)\
{\
  if (!inst || !*inst) {\
    return NULL;\
  }\
  \
  if (capacity == (*inst)->capacity) {\
    return *inst;\
  }\
  \
  Array(type) *array = array(type, capacity);\
  for (register llong i = 0; i < capacity; i++) {\
    if (i >= (*inst)->capacity) {\
      break;\
    }\
  \
    set(type, array, i, get(type, *inst, i));\
  }\
  \
  Delete(Array(type), inst);\
  \
  return array;\
}\
\
/* Insert before @index. */\
Array(type) *type##Array_Insert(\
  Array(type) **const inst,\
  const llong index,\
  const type data\
) {\
  if (!inst || !*inst) {\
    return NULL;\
  }\
  \
  if (index < 0 || index > (*inst)->capacity) {\
    return NULL;\
  }\
  \
  Array(type) *newarr = array(type, (*inst)->capacity + 1);\
  for (register llong i = 0; i < index; i++) {\
    set(type, newarr, i, get(type, *inst, i));\
  }\
  set(type, newarr, index, data);\
  for (register llong i = index + 1; i <= (*inst)->capacity; i++) {\
    set(type, newarr, i, get(type, *inst, i - 1));\
  }\
  \
  Delete(Array(type), inst);\
  \
  return newarr;\
}\
\
/* Remove before @index. */\
Array(type) *type##Array_Remove(Array(type) **const inst, const llong index)\
{\
  if (!inst || !*inst) {\
    return NULL;\
  }\
  \
  if (index < 0 || index > (*inst)->capacity) {\
    return NULL;\
  }\
  \
  Array(type) *newarr = array(type, (*inst)->capacity - 1);\
  for (register llong i = 0; i < index; i++) {\
    set(type, newarr, i, get(type, *inst, i));\
  }\
  for (register llong i = index; i < newarr->capacity; i++) {\
    set(type, newarr, i, get(type, *inst, i + 1));\
  }\
  \
  Delete(Array(type), inst);\
  \
  return newarr;\
}\
\
boolean type##Array_Equals(\
  const Array(type) *const arr1,\
  const Array(type) *const arr2,\
  boolean (*const IsEqual)(const type *const obj1, const type *const obj2)\
) {\
  if ((!arr1 || !arr2) || (arr1->capacity != arr2->capacity) ||\
      (arr1->capacity != arr2->capacity)) {\
    return false;\
  }\
  \
  iterate (i, arr1) {\
    type *A = ref(type, arr1, i);\
    type *B = ref(type, arr2, i);\
    if (!A || !B) {\
      return false;\
    }\
  \
    if (IsEqual) {\
      if (!IsEqual(A, B)) {\
        return false;\
      }\
    /* No given custom equality evaluator, falling back. */\
    } else {\
      if (EqualsSolution) {\
        return false;\
      }\
    }\
  }\
  \
  return true;\
}

#endif  /* COMPOUND_ARRAY_H */
