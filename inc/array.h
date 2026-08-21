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

# include <string.h>
# include <stdarg.h>

# include "common.h"
# include "memory_stack.h"
# include "types.h"

# define isinbound(type, array_ptr, index)                                     \
  (call(type, IsInBound, (inst), (index)))

# define offsetting(type, array_ptr, index)                                    \
  (call(type, Offsetting, (array_ptr), (index)))

# define capacity(type, array_ptr)                                             \
  (call(type, GetCapacity, (array_ptr)))

# define last(type, array_ptr)                                                 \
  (get(type, (array_ptr), -1))

# define ref(type, array_ptr, index)                                           \
  (call(type, Ref, (array_ptr), (index)))

# define refref(type, array_ptr, index)                                        \
  (call(type, RefRef, (array_ptr), (index)))

# define get(type, array_ptr, index)                                           \
  (call(type, Get, (array_ptr), (index)))

# define set(type, array_ptr, index, object)                                   \
  (call(type, Set, (array_ptr), (index), (object)))

# define transfer(type, inst_array_ptr, src_array_ptr)                         \
  (call(type, Transfer, (inst_array_ptr), (src_array_ptr)))

# define iterate(type, it, array_ptr, block)                                   \
  {                                                                            \
    type *const __iterate_array_ptr_##it = (type *const)(array_ptr);           \
    const llong __iterate_capacity_##it = CONCAT(type, _GetCapacity)(          \
      __iterate_array_ptr_##it                                                 \
    );                                                                         \
    loop (it, __iterate_capacity_##it)                                         \
      block                                                                    \
  }

# define foreach(elem_type, it, array_ptr, block)                              \
  {                                                                            \
    Array(elem_type) *const _foreach_array_ptr_##it =                          \
      (Array(elem_type) *const)(array_ptr);                                    \
    elem_type it = (elem_type)EMPTY;                                           \
    iterate (                                                                  \
      Array(elem_type),                                                        \
      _foreach_idx_##it,                                                       \
      (_foreach_array_ptr_##it),                                               \
      {                                                                        \
        it = get(                                                              \
          Array(elem_type),                                                    \
          (_foreach_array_ptr_##it),                                           \
          _foreach_idx_##it                                                    \
        );                                                                     \
        block                                                                  \
      }                                                                        \
    )                                                                          \
  }

# define refeach(elem_type, it, array_ptr, block)                              \
  _refeach_type(elem_type, elem_type, it, array_ptr, block)

# define _refeach_type(elem_type, nickname, it, array_ptr, block)              \
  {                                                                            \
    Array(nickname) *const __refeach_array_ptr_##it =                          \
      (Array(nickname) *const)(array_ptr);                                     \
    elem_type *it = NULL;                                                      \
    iterate (                                                                  \
      Array(nickname),                                                         \
      _refeach_idx_##it,                                                       \
      (__refeach_array_ptr_##it),                                              \
      {                                                                        \
        it = (elem_type *)(void *)ref(                                         \
          Array(nickname),                                                     \
          (__refeach_array_ptr_##it),                                          \
          _refeach_idx_##it                                                    \
        );                                                                     \
        block                                                                  \
      }                                                                        \
    )                                                                          \
  }

# define refrefeach(elem_type, it, array_ptr, block)                           \
  _refrefeach_type(elem_type, elem_type, it, array_ptr, block)

# define _refrefeach_type(elem_type, nickname, it, array_ptr, block)           \
  {                                                                            \
    Array(nickname) *const __refrefeach_array_ptr_##it =                       \
      (Array(nickname) *const)(array_ptr);                                     \
    elem_type **it = NULL;                                                     \
    iterate (                                                                  \
      Array(nickname),                                                         \
      _refrefeach_idx_##it,                                                    \
      (__refrefeach_array_ptr_##it),                                           \
      {                                                                        \
        it = (elem_type **)refref(                                             \
          Array(nickname),                                                     \
          (__refrefeach_array_ptr_##it),                                       \
          _refrefeach_idx_##it                                                 \
        );                                                                     \
        block                                                                  \
      }                                                                        \
    )                                                                          \
  }

# define reverse(type, obj_ptr)                                                \
  (call(type, Reverse, (obj_ptr)))

# define array(elem_type, capacity)                                            \
  (Create(Array(elem_type), (capacity)))

# define fill(type, array_ptr, value)                                          \
  (call(type, Fill, (array_ptr), (value)))

# define erase(type, array_ptr)                                                \
  (call(type, Erase, (array_ptr)))

# define resize(type, array_ptr, capacity)                                     \
  (call(type, Resize, (array_ptr), (capacity)))

# define grow(type, array_ptr)                                                 \
  (resize(type, (array_ptr), (capacity(*(array_ptr)) * 1.5)))

# define Array(elem_type)                                                      \
  elem_type##Array

/* Alias. */
# define DEFINE_ARRAY(elem_type)                                               \
  DEFINE_ARRAY_OBJECT(elem_type, elem_type)

// boolean represents;
# define _DEFINE_ARRAY_COMMON_MEMBERS                                          \
  llong capacity;                                                              \
  boolean reserved;                                                            \
  boolean reversed;

# define DEFINE_ARRAY_BASICTYPE(elem_type)                                     \
  DEFINE_ARRAY_BASICTYPE_NICKNAME(elem_type, elem_type)

# define DEFINE_ARRAY_BASICTYPE_NICKNAME(elem_type, nickname)                  \
  struct nickname##Array {                                                     \
    elem_type *data;                                                           \
    _DEFINE_ARRAY_COMMON_MEMBERS                                               \
  };

# define DEFINE_ARRAY_OBJECT(elem_type)                                        \
  struct elem_type##Array {                                                    \
    elem_type **data;                                                          \
    _DEFINE_ARRAY_COMMON_MEMBERS                                               \
  };

# define TYPEDEF_ARRAY(elem_type)                                              \
  typedef struct elem_type##Array elem_type##Array;

/* In order to preserve the consistency of calling convension,
 * the @elem_type is left unused intentionally. */
# define TYPEDEF_ARRAY_NICKNAME(elem_type, nickname)                           \
  typedef struct nickname##Array nickname##Array;

/* Default array declaration is set to Object
   since basic types are more "internal"
   since they have presets already -- users do not use that
   very frequently compare to the Object one. */
# define ARRAY(elem_type)                                                      \
  ARRAY_OBJECT(elem_type)

# define ARRAY_OBJECT(elem_type)                                               \
  TYPEDEF_ARRAY(elem_type)                                                     \
  FUNC_ARRAY_OBJECT(elem_type)                                                 \
  ARRAY_COMMON(elem_type)

# define ARRAY_BASICTYPE(elem_type)                                            \
  ARRAY_BASICTYPE_NICKNAME(elem_type, elem_type)

# define ARRAY_BASICTYPE_NICKNAME(elem_type, nickname)                         \
  TYPEDEF_ARRAY_NICKNAME(elem_type, nickname)                                  \
  FUNC_ARRAY_BASICTYPE_NICKNAME(elem_type, nickname)                           \
  ARRAY_COMMON_NICKNAME(elem_type, nickname)

# define ARRAY_COMMON(elem_type)                                               \
  ARRAY_COMMON_NICKNAME(elem_type, elem_type)

# define ARRAY_COMMON_NICKNAME(elem_type, nickname)                            \
  FUNC_ARRAY_COMMON_NICKNAME(elem_type, nickname)

# define FUNC_ARRAY_OBJECT(elem_type)                                          \
  elem_type *elem_type##Array_Get(                                             \
    const Array(elem_type) *const inst,                                        \
    const llong index                                                          \
  );                                                                           \
  void elem_type##Array_Set(                                                   \
    const Array(elem_type) *const inst,                                        \
    const llong index,                                                         \
    elem_type *const value                                                     \
  );                                                                           \
  elem_type **elem_type##Array_RefRef(                                         \
    const Array(elem_type) *const inst,                                        \
    const llong index                                                          \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Create(const llong capacity);             \
  Array(elem_type) *elem_type##Array_CopyOf(                                   \
    const Array(elem_type) *const other                                        \
  );                                                                           \
  void elem_type##Array_Delete(Array(elem_type) *const inst);                  \
  Array(elem_type) *elem_type##Array_Clone(                                    \
    const Array(elem_type) *const other                                        \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Erase(Array(elem_type) *const inst);      \
  Array(elem_type) *elem_type##Array_Fill(                                     \
    Array(elem_type) *const inst,                                              \
    elem_type *value                                                           \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Insert(                                   \
    Array(elem_type) *const inst,                                              \
    const llong index,                                                         \
    elem_type *const value                                                     \
  );                                                                           \
  boolean elem_type##Array_Equals(                                             \
    Array(elem_type) *const arr1,                                              \
    Array(elem_type) *const arr2,                                              \
    boolean (*const IsEqual)(                                                  \
      elem_type *const obj1,                                                   \
      elem_type *const obj2                                                    \
    )                                                                          \
  );                                                                           \
  elem_type **elem_type##Array_GetData(const Array(elem_type) *const inst);

# define FUNC_ARRAY_BASICTYPE(elem_type)                                       \
  FUNC_ARRAY_BASICTYPE_NICKNAME(elem_type, elem_type)

# define FUNC_ARRAY_BASICTYPE_NICKNAME(elem_type, nickname)                    \
  nickname nickname##Array_Get(                                                \
    const Array(nickname) *const inst,                                         \
    const llong index                                                          \
  );                                                                           \
  void nickname##Array_Set(                                                    \
    const Array(nickname) *const inst,                                         \
    const llong index,                                                         \
    nickname value                                                             \
  );                                                                           \
  Array(nickname) *nickname##Array_Create(const llong capacity);               \
  Array(nickname) *nickname##Array_CopyOf(                                     \
    const Array(nickname) *const other                                         \
  );                                                                           \
  void nickname##Array_Delete(Array(nickname) *const inst);                    \
  Array(nickname) *nickname##Array_Fill(                                       \
    Array(nickname) *const inst,                                               \
    elem_type value                                                            \
  );                                                                           \
  Array(nickname) *nickname##Array_Insert(                                     \
    Array(nickname) *const inst,                                               \
    const llong index,                                                         \
    nickname value                                                             \
  );                                                                           \
  boolean nickname##Array_Equals(                                              \
    Array(nickname) *const arr1,                                               \
    Array(nickname) *const arr2,                                               \
    boolean (*const IsEqual)(                                                  \
      elem_type *const obj1,                                                   \
      elem_type *const obj2                                                    \
    )                                                                          \
  );                                                                           \
  nickname *nickname##Array_GetData(const Array(nickname) *const inst);

# define FUNC_ARRAY_COMMON(elem_type)                                          \
  FUNC_ARRAY_COMMON_NICKNAME(elem_type, elem_type)

# define FUNC_ARRAY_COMMON_NICKNAME(elem_type, nickname)                       \
  nickname *nickname##Array_Ref(                                               \
    const Array(nickname) *const inst,                                         \
    const llong index                                                          \
  );                                                                           \
  boolean nickname##Array_IsInBound(                                           \
    const Array(nickname) *const inst,                                         \
    const llong index                                                          \
  );                                                                           \
  llong nickname##Array_Offsetting(                                            \
    const Array(nickname) *const inst,                                         \
    const llong index                                                          \
  );                                                                           \
  Array(nickname) *nickname##Array_Resize(                                     \
    Array(nickname) *const inst,                                               \
    const llong capacity                                                       \
  );                                                                           \
  Array(nickname) *nickname##Array_Remove(                                     \
    Array(nickname) *const inst,                                               \
    const llong index                                                          \
  );                                                                           \
  Array(nickname) *nickname##Array_Reverse(Array(nickname) *const inst);       \
  Array(nickname) *nickname##Array_Compose(const llong arglen, ...);           \
  llong nickname##Array_Length(Array(nickname) *const inst);                   \
  llong nickname##Array_GetCapacity(const Array(nickname) *const inst);        \
  boolean nickname##Array_GetReserved(const Array(nickname) *const inst);      \
  boolean nickname##Array_GetReversed(const Array(nickname) *const inst);

# define IMPL_ARRAY(elem_type)                                                 \
  IMPL_ARRAY_OBJECT(elem_type)

/* Objects are Opaque Pointer designed. */
# define IMPL_ARRAY_OBJECT(elem_type)                                          \
DEFINE_ARRAY_OBJECT(elem_type)                                                 \
IMPL_ARRAY_COMMON(elem_type)                                                   \
IMPL_ARRAY_OBJECT_COMPOSE(elem_type)                                           \
inline elem_type *elem_type##Array_Ref(                                        \
  const Array(elem_type) *const inst,                                          \
  const llong index                                                            \
) {                                                                            \
  if (!inst || !isinbound(Array(elem_type), inst, index)) {                    \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return (inst->data[offsetting(Array(elem_type), inst, index)]);              \
}                                                                              \
                                                                               \
inline elem_type **elem_type##Array_RefRef(                                    \
  const Array(elem_type) *const inst,                                          \
  const llong index                                                            \
) {                                                                            \
  if (!inst || !isinbound(Array(elem_type), inst, index)) {                    \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return &(inst->data[offsetting(Array(elem_type), inst, index)]);             \
}                                                                              \
                                                                               \
inline elem_type *elem_type##Array_Get(                                        \
  const Array(elem_type) *const inst,                                          \
  const llong index                                                            \
) {                                                                            \
  if (!inst || !isinbound(Array(elem_type), inst, index)) {                    \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return (inst->data[offsetting(Array(elem_type), inst, index)]);              \
}                                                                              \
                                                                               \
inline void elem_type##Array_Set(                                              \
  const Array(elem_type) *const inst,                                          \
  const llong index,                                                           \
  elem_type *const value                                                       \
) {                                                                            \
  if (!inst || !isinbound(Array(elem_type), inst, index)) {                    \
    return;                                                                    \
  }                                                                            \
                                                                               \
  inst->data[offsetting(Array(elem_type), inst, index)] = value;               \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_Clone(const Array(elem_type) *const other)  \
{                                                                              \
  if (!other) {                                                                \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Array(elem_type) *inst = Create(Array(elem_type), other->capacity);          \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
/* llong interrupted = -1; */                                                  \
  for (register llong i = 0; i < other->capacity; i++) {                       \
    *refref(Array(elem_type), inst, i) = CopyOf(                               \
      elem_type,                                                               \
      ref(Array(elem_type), other, i)                                          \
    );                                                                         \
/*  if (belong(RuntimeError)) {  */                                            \
/*    interrupted = i;           */                                            \
/*    break;                     */                                            \
/*  }                            */                                            \
  }                                                                            \
                                                                               \
/* if (interrupted < 0) { */                                                   \
/* return inst; */                                                             \
/* } */                                                                        \
                                                                               \
/* for (register llong i = interrupted - 1; i >= 0; i--) { */                  \
/* Delete(elem_type, &inst->data[i]); */                                       \
/* } */                                                                        \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_Insert(                                     \
  Array(elem_type) *const inst,                                                \
  const llong index,                                                           \
  elem_type *const value                                                       \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (index > inst->capacity) {                                                \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (!value) {                                                                \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  Array(elem_type) *newarr = Create(Array(elem_type), inst->capacity + 1);     \
  if (!newarr) {                                                               \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  const llong pivot = offsetting(Array(elem_type), newarr, index);             \
  for (register llong i = 0; i < pivot; i++) {                                 \
    newarr->data[i] = inst->data[i];                                           \
  }                                                                            \
  newarr->data[pivot] = value;                                                 \
  for (register llong i = pivot; i < inst->capacity; i++) {                    \
    newarr->data[i + 1] = inst->data[i];                                       \
  }                                                                            \
                                                                               \
  Delete(Array(elem_type), inst);                                              \
                                                                               \
  return newarr;                                                               \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_Create(const llong capacity)                \
{                                                                              \
  if (capacity < 0) {                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Array(elem_type) *inst = Allocate(1, sizeof(Array(elem_type)));              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->data = Allocate(capacity, sizeof(elem_type *));                        \
  if (!inst->data) {                                                           \
    Deallocate(inst);                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->capacity = capacity;                                                   \
  inst->reserved = (!capacity);                                                \
  inst->reversed = false;                                                      \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_CopyOf(const Array(elem_type) *const other) \
{                                                                              \
  if (!other) {                                                                \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Array(elem_type) *inst = Allocate(1, sizeof(Array(elem_type)));              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->data = Allocate(other->capacity, sizeof(elem_type *));                 \
  if (!inst->data) {                                                           \
    Deallocate(inst);                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->capacity = other->capacity;                                            \
  inst->reserved = other->reserved;                                            \
  inst->reversed = other->reversed;                                            \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
void elem_type##Array_Delete(Array(elem_type) *const inst)                     \
{                                                                              \
  if (!inst) {                                                                 \
    return;                                                                    \
  }                                                                            \
                                                                               \
  Deallocate(inst->data);                                                      \
  Deallocate(inst);                                                            \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_Erase(Array(elem_type) *const inst)         \
{                                                                              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  const llong capa = capacity(Array(elem_type), inst);                         \
  for (register llong i = 0; i < capa; i++) {                                  \
    Delete(elem_type, inst->data[i]);                                          \
  }                                                                            \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_Fill(                                       \
  Array(elem_type) *const inst,                                                \
  elem_type *value                                                             \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  refrefeach (elem_type, it, inst, {                                           \
    if (!it || !value) {                                                       \
      continue;                                                                \
    }                                                                          \
                                                                               \
    *it = value;                                                               \
  })                                                                           \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
inline boolean elem_type##Array_Equals(                                        \
  Array(elem_type) *const arr1,                                                \
  Array(elem_type) *const arr2,                                                \
  boolean (*const IsEqual)(                                                    \
    elem_type *const obj1,                                                     \
    elem_type *const obj2                                                      \
  )                                                                            \
) {                                                                            \
  if (                                                                         \
    (!arr1 || !arr2) ||                                                        \
    (arr1->capacity) != (arr2->capacity) ||                                    \
    (arr1->capacity) != (arr2->capacity)                                       \
  ) {                                                                          \
    return false;                                                              \
  }                                                                            \
                                                                               \
  if (arr1 == arr2) {                                                          \
    return true;                                                               \
  }                                                                            \
                                                                               \
  /* The capacity of the two arrays are now the same;                          \
     picking one of them results the same. */                                  \
  for (register llong i = 0; i < arr1->capacity; i++) {                        \
    elem_type *A = get(Array(elem_type), arr1, i);                             \
    elem_type *B = get(Array(elem_type), arr2, i);                             \
    if (!A || !B) {                                                            \
      return false;                                                            \
    }                                                                          \
                                                                               \
    if ((IsEqual && !IsEqual(A, B)) || !Equals(elem_type, A, B)) {             \
      return false;                                                            \
    }                                                                          \
  }                                                                            \
                                                                               \
  return true;                                                                 \
}                                                                              \
                                                                               \
inline elem_type **elem_type##Array_GetData(const Array(elem_type) *const inst)\
{                                                                              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return inst->data;                                                           \
}

# define IMPL_ARRAY_BASICTYPE(elem_type)                                       \
IMPL_ARRAY_BASICTYPE_NICKNAME(elem_type, elem_type)                            \

# define IMPL_ARRAY_BASICTYPE_NICKNAME(elem_type, nickname)                    \
IMPL_ARRAY_BASICTYPE_NICKNAME_PROMPTEDTYPE(elem_type, nickname, nickname)

# define IMPL_ARRAY_BASICTYPE_NICKNAME_PROMPTEDTYPE(                           \
  elem_type,                                                                   \
  nickname,                                                                    \
  prompted_type                                                                \
)                                                                              \
DEFINE_ARRAY_BASICTYPE_NICKNAME(elem_type, nickname)                           \
IMPL_ARRAY_BASICTYPE_COMPOSE_NICKNAME(elem_type, nickname, prompted_type)      \
IMPL_ARRAY_COMMON_NICKNAME(elem_type, nickname)                                \
inline nickname *nickname##Array_Ref(                                          \
  const Array(nickname) *const inst,                                           \
  const llong index                                                            \
) {                                                                            \
  if (!inst || !isinbound(Array(nickname), inst, index)) {                     \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return &(inst->data[offsetting(Array(nickname), inst, index)]);              \
}                                                                              \
                                                                               \
inline nickname nickname##Array_Get(                                           \
  const Array(nickname) *const inst,                                           \
  const llong index                                                            \
) {                                                                            \
  nickname *const ref = nickname##Array_Ref(inst, index);                      \
  if (!ref) {                                                                  \
    return (nickname)EMPTY;                                                    \
  }                                                                            \
                                                                               \
  return *ref;                                                                 \
}                                                                              \
                                                                               \
inline void nickname##Array_Set(                                               \
  const Array(nickname) *const inst,                                           \
  const llong index,                                                           \
  nickname value                                                               \
) {                                                                            \
  elem_type *ref = ref(Array(nickname), inst, index);                          \
  if (!ref) {                                                                  \
    return;                                                                    \
  }                                                                            \
                                                                               \
  *ref = value;                                                                \
}                                                                              \
                                                                               \
Array(nickname) *nickname##Array_Insert(                                       \
  Array(nickname) *const inst,                                                 \
  const llong index,                                                           \
  nickname value                                                               \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (index > inst->capacity) {                                                \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Array(nickname) *newarr = Create(Array(nickname), inst->capacity + 1);       \
  if (!newarr) {                                                               \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  const llong pivot = offsetting(Array(nickname), newarr, index);              \
  for (register llong i = 0; i < pivot; i++) {                                 \
    newarr->data[i] = inst->data[i];                                           \
  }                                                                            \
  newarr->data[pivot] = value;                                                 \
  for (register llong i = pivot; i < inst->capacity; i++) {                    \
    newarr->data[i + 1] = inst->data[i];                                       \
  }                                                                            \
                                                                               \
  Delete(Array(nickname), inst);                                               \
                                                                               \
  return newarr;                                                               \
}                                                                              \
                                                                               \
Array(nickname) *nickname##Array_Create(const llong capacity)                  \
{                                                                              \
  if (capacity < 0) {                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Array(nickname) *inst = Allocate(1, sizeof(Array(nickname)));                \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->data = Allocate(capacity, sizeof(elem_type));                          \
  if (!inst->data) {                                                           \
    Deallocate(inst);                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->capacity = capacity;                                                   \
  inst->reserved = (!capacity);                                                \
  inst->reversed = false;                                                      \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
Array(nickname) *nickname##Array_CopyOf(const Array(nickname) *const other)    \
{                                                                              \
  if (!other) {                                                                \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Array(nickname) *inst = Allocate(1, sizeof(Array(nickname)));                \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->data = Allocate(other->capacity, sizeof(elem_type));                   \
  if (!inst->data) {                                                           \
    Deallocate(inst);                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  memcpy(inst->data, other->data, sizeof(elem_type) * other->capacity);        \
  inst->capacity = other->capacity;                                            \
  inst->reserved = other->reserved;                                            \
  inst->reversed = other->reversed;                                            \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
void nickname##Array_Delete(Array(nickname) *const inst)                       \
{                                                                              \
  if (!inst) {                                                                 \
    return;                                                                    \
  }                                                                            \
                                                                               \
  Deallocate(inst->data);                                                      \
  Deallocate(inst);                                                            \
}                                                                              \
                                                                               \
Array(nickname) *nickname##Array_Fill(                                         \
  Array(nickname) *const inst,                                                 \
  elem_type value                                                              \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (!inst->data) {                                                           \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  refeach (nickname, it, inst, {                                               \
    *it = value;                                                               \
  })                                                                           \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
inline boolean nickname##Array_Equals(                                         \
  Array(nickname) *const arr1,                                                 \
  Array(nickname) *const arr2,                                                 \
  boolean (*const IsEqual)(                                                    \
    elem_type *const obj1,                                                     \
    elem_type *const obj2                                                      \
  )                                                                            \
) {                                                                            \
  ignore IsEqual;                                                              \
                                                                               \
  if (                                                                         \
    (!arr1 || !arr2) ||                                                        \
    (arr1->capacity) != (arr2->capacity) ||                                    \
    (arr1->capacity) != (arr2->capacity)                                       \
  ) {                                                                          \
    return false;                                                              \
  }                                                                            \
                                                                               \
  if (arr1 == arr2) {                                                          \
    return true;                                                               \
  }                                                                            \
                                                                               \
  /* The capacity of the two arrays are now the same;                          \
     picking one of them results the same. */                                  \
  for (register llong i = 0; i < arr1->capacity; i++) {                        \
    elem_type A = get(Array(nickname), arr1, i);                               \
    elem_type B = get(Array(nickname), arr2, i);                               \
    if (A != B) {                                                              \
      return false;                                                            \
    }                                                                          \
  }                                                                            \
                                                                               \
  return true;                                                                 \
}                                                                              \
inline nickname *nickname##Array_GetData(const Array(nickname) *const inst)    \
{                                                                              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return inst->data;                                                           \
}

# define IMPL_ARRAY_COMMON(elem_type)                                          \
  IMPL_ARRAY_COMMON_NICKNAME(elem_type, elem_type)

# define IMPL_ARRAY_COMMON_NICKNAME(elem_type, nickname)                       \
inline boolean nickname##Array_IsInBound(                                      \
  const Array(nickname) *const inst,                                           \
  const llong index                                                            \
) {                                                                            \
  if (!inst) {                                                                 \
    return false;                                                              \
  }                                                                            \
                                                                               \
  return (index < inst->capacity);                                             \
}                                                                              \
                                                                               \
inline llong nickname##Array_Offsetting(                                       \
  const Array(nickname) *const inst,                                           \
  const llong index                                                            \
) {                                                                            \
  if (!inst) {                                                                 \
    return index;                                                              \
  }                                                                            \
                                                                               \
  llong final_index = index;                                                   \
  if (index < 0) {                                                             \
    final_index = index + inst->capacity;                                      \
  }                                                                            \
                                                                               \
  /* Formula:  f(R,I,C) = I + R * (C - 1 - 2I) */                              \
  return ((final_index) + inst->reversed * (inst->capacity - 1-2*final_index));\
}                                                                              \
                                                                               \
Array(nickname) *nickname##Array_Resize(                                       \
  Array(nickname) *const inst,                                                 \
  const llong capacity                                                         \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (capacity == inst->capacity) {                                            \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  Array(nickname) *array = Create(Array(nickname), capacity);                  \
  if (!array) {                                                                \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  const llong delta = capacity - inst->capacity;                               \
  const llong final_capacity = delta > 0 ? inst->capacity : capacity;          \
                                                                               \
  memcpy(array->data, inst->data, final_capacity * sizeof(elem_type));         \
                                                                               \
  /* Extending. */                                                             \
  if (delta > 0) {                                                             \
    memset(array->data, 0, delta * sizeof(elem_type));                         \
  }                                                                            \
                                                                               \
  Delete(Array(nickname), inst);                                               \
                                                                               \
  return array;                                                                \
}                                                                              \
                                                                               \
/* Remove before @index. */                                                    \
Array(nickname) *nickname##Array_Remove(                                       \
  Array(nickname) *const inst,                                                 \
  const llong index                                                            \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (index < 0 || index >= inst->capacity) {                                  \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Array(nickname) *newarr = Create(Array(nickname), inst->capacity - 1);       \
  if (!newarr) {                                                               \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  if (index > 0) {                                                             \
    memcpy(                                                                    \
      &newarr->data[newarr->capacity],                                         \
      &inst->data[index], index * sizeof(elem_type)                            \
    );                                                                         \
  }                                                                            \
                                                                               \
  llong remaining = newarr->capacity - index;                                  \
  if (remaining > 0) {                                                         \
    memcpy(                                                                    \
      &newarr->data[index],                                                    \
      &inst->data[index + 1],                                                  \
      remaining * sizeof(elem_type)                                            \
    );                                                                         \
  }                                                                            \
                                                                               \
  Delete(Array(nickname), inst);                                               \
                                                                               \
  return newarr;                                                               \
}                                                                              \
                                                                               \
inline Array(nickname) *nickname##Array_Reverse(Array(nickname) *const inst)   \
{                                                                              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  inst->reversed = !inst->reversed;                                            \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
inline llong nickname##Array_Length(Array(nickname) *const inst)               \
{                                                                              \
  if (!inst) {                                                                 \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  const llong capa = capacity(Array(nickname), inst);                          \
  if (!capa) {                                                                 \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  register llong length = capa;                                                \
  /* This way, BasicType arrays will always                                    \
   * return the capacity for its length. */                                    \
  while (length >= 0) {                                                        \
    if (ref(Array(nickname), inst, length)) {                                  \
      break;                                                                   \
    }                                                                          \
                                                                               \
    length--;                                                                  \
  }                                                                            \
                                                                               \
  return length + 1;                                                           \
}                                                                              \
                                                                               \
inline llong nickname##Array_GetCapacity(const Array(nickname) *const inst)    \
{                                                                              \
  if (!inst) {                                                                 \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  return inst->capacity;                                                       \
}                                                                              \
                                                                               \
inline boolean nickname##Array_GetReserved(const Array(nickname) *const inst)  \
{                                                                              \
  if (!inst) {                                                                 \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  return inst->reserved;                                                       \
}                                                                              \
                                                                               \
inline boolean nickname##Array_GetReversed(const Array(nickname) *const inst)  \
{                                                                              \
  if (!inst) {                                                                 \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  return inst->reversed;                                                       \
}

# define IMPL_ARRAY_COMPOSE(elem_type)                                         \
IMPL_ARRAY_OBJECT_COMPOSE(elem_type)

# define IMPL_ARRAY_BASICTYPE_COMPOSE(prompted_type, elem_type)                \
IMPL_ARRAY_BASICTYPE_NICKNAME(elem_type, nickname)                             \
IMPL_ARRAY_BASICTYPE_COMPOSE_NICKNAME(elem_type, nickname, prompted_type)

# define IMPL_ARRAY_BASICTYPE_COMPOSE_NICKNAME(                                \
  elem_type,                                                                   \
  nickname,                                                                    \
  prompted_type                                                                \
)                                                                              \
/* Compose should always directly copy the value given. */                     \
Array(nickname) *nickname##Array_Compose(const llong arglen, ...)              \
{                                                                              \
  if (!arglen) {                                                               \
    return Create(Array(nickname), 0);                                         \
  }                                                                            \
                                                                               \
  Array(nickname) *const inst = Create(Array(nickname), arglen);               \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  va_list ap;                                                                  \
  va_start(ap, arglen);                                                        \
  _refeach_type (elem_type, nickname, it, inst, {                              \
    *it = va_arg(ap, prompted_type);                                           \
  })                                                                           \
  va_end(ap);                                                                  \
                                                                               \
  return inst;                                                                 \
}

# define IMPL_ARRAY_OBJECT_COMPOSE(elem_type)                                  \
/* Compose should always directly copy the reference given to the value. */    \
Array(elem_type) *elem_type##Array_Compose(const llong arglen, ...)            \
{                                                                              \
  if (!arglen) {                                                               \
    return Create(Array(elem_type), 0);                                        \
  }                                                                            \
                                                                               \
  Array(elem_type) *const inst = Create(Array(elem_type), arglen);             \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  va_list ap;                                                                  \
  va_start(ap, arglen);                                                        \
  _refrefeach_type (elem_type, elem_type, itptr, inst, {                       \
    *itptr = va_arg(ap, elem_type *);                                          \
  })                                                                           \
  va_end(ap);                                                                  \
                                                                               \
  return inst;                                                                 \
}

#endif  /* COMPOUND_ARRAY_H */
