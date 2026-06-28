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

# include "common.h"
# include "memory_stack.h"
# include "types.h"

# define isinbound(type, array_ptr, index)                                     \
  (CONCAT(type, _IsInBound)(inst, index))

# define offsetting(type, array_ptr, index)                                    \
  (CONCAT(type, _Offsetting)((array_ptr), (index)))

# define capacity(type, array_ptr)                                             \
  (CONCAT(type, _GetCapacity)(array_ptr))

# define last(type, array_ptr)                                                 \
  (ref(type, (array_ptr), -1))

# define ref(type, array_ptr, index)                                           \
  (CONCAT(type, _Ref)((array_ptr), (index)))

# define refref(type, array_ptr, index)                                        \
  (CONCAT(type, _RefRef)((array_ptr), (index)))

# define assign(type, inst_array_ptr_ptr, other_array_ptr)                     \
  (CONCAT(type, _Assign)((inst_array_ptr_ptr), (other_array_ptr)))

# define transfer(type, inst_array_ptr, src_array_ptr)                         \
  (CONCAT(type, _Transfer)((inst_array_ptr), (src_array_ptr)))

# define iterate(elem_type, it, array_ptr, block)                              \
  {                                                                            \
    const llong _##it##_capacity = elem_type##Array_GetCapacity(array_ptr);    \
    loop (it, _##it##_capacity)                                                \
      block                                                                    \
  }

# define foreach(elem_type, it, array_ptr, block)                              \
  refeach(elem_type, it, array_ptr, block)

# define refeach(elem_type, it, array_ptr, block)                              \
  {                                                                            \
    Array(elem_type) *const _refrefeach_##it##_array                           \
      = (Array(elem_type) *)(array_ptr);                                       \
    register elem_type *it = NULL;                                             \
    iterate (                                                                  \
      elem_type,                                                               \
      _refrefeach_##elem_type##_idx,                                           \
      _refrefeach_##it##_array,                                                \
      {                                                                        \
        it = ref(                                                              \
          Array(elem_type),                                                    \
          _refrefeach_##it##_array,                                            \
          _refrefeach_##elem_type##_idx                                        \
        );                                                                     \
        block                                                                  \
      }                                                                        \
    )                                                                          \
  }

# define refrefeach(elem_type, it, array_ptr, block)                           \
  {                                                                            \
    Array(elem_type) *const _refrefeach_##it##_array                           \
      = (Array(elem_type) *)(array_ptr);                                       \
    register elem_type **it = NULL;                                            \
    iterate (                                                                  \
      elem_type,                                                               \
      _refrefeach_##elem_type##_idx,                                           \
      _refrefeach_##it##_array,                                                \
      {                                                                        \
        it = refref(                                                           \
          Array(elem_type),                                                    \
          _refrefeach_##it##_array,                                            \
          _refrefeach_##elem_type##_idx                                        \
        );                                                                     \
        block                                                                  \
      }                                                                        \
    )                                                                          \
  }

# define compose(elem_type, ...)                                               \
  CONCAT(CONCAT(Array(elem_type), _), Compose)(arglen(__VA_ARGS__), __VA_ARGS__)

# define reverse(type, obj_ptr)                                                \
  (call(type, Reverse, obj_ptr))

# define array(elem_type, capacity)                                            \
  (Create(Array(elem_type), (capacity)))

# define fill(type, array_ptr, value)                                          \
  (call(type, Fill, array_ptr, value))

# define erase(type, array_ptr)                                                \
  (call(type, Erase, array_ptr))

# define grow(type, array_ptr)                                                 \
  (resize(type, array_ptr, (capacity(*(array_ptr)) * 1.5)))

# define resize(type, array_ptr, capacity)                                     \
  (call(type, Resize, array_ptr, capacity))

# define Array(elem_type)                                                      \
  elem_type##Array

# define DEFINE_ARRAY_BASICTYPE(elem_type)                                     \
  struct elem_type##Array {                                                    \
    elem_type *data;                                                           \
    llong capacity;                                                            \
    boolean reserved;                                                          \
    boolean reversed;                                                          \
  };

# define DEFINE_ARRAY_OBJECT(elem_type)                                        \
  struct elem_type##Array {                                                    \
    elem_type **data;  /* For Opaque Pointers. */                              \
    llong capacity;                                                            \
    boolean reserved;                                                          \
    boolean reversed;                                                          \
  };

# define TYPEDEF_ARRAY(elem_type)                                              \
  typedef struct elem_type##Array elem_type##Array;

/* Default array declaration is set to Object
   since basic types' are more "internal"
   since they are preset already -- users do not use that
   very frequently compare to the Object one. */
# define ARRAY(elem_type)                                                      \
  ARRAY_OBJECT(elem_type)

# define ARRAY_OBJECT(elem_type)                                               \
  TYPEDEF_ARRAY(elem_type)                                                     \
  FUNC_ARRAY_OBJECT(elem_type)                                                 \

# define ARRAY_BASICTYPE(elem_type)                                            \
  TYPEDEF_ARRAY(elem_type)                                                     \
  FUNC_ARRAY_BASICTYPE(elem_type)                                              \

# define FUNC_ARRAY_OBJECT(elem_type)                                          \
  FUNC_ARRAY_COMMON(elem_type)                                                 \
  elem_type *elem_type##Array_Ref(                                             \
    const Array(elem_type) *const inst,                                        \
    const llong index                                                          \
  );                                                                           \
  elem_type **elem_type##Array_RefRef(                                         \
    const Array(elem_type) *const inst,                                        \
    const llong index                                                          \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Clone(                                    \
    const Array(elem_type) *const other                                        \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Erase(Array(elem_type) *const inst);      \
  boolean elem_type##Array_Equals(                                             \
    const Array(elem_type) *const arr1,                                        \
    const Array(elem_type) *const arr2,                                        \
    boolean (*const IsEqual)(                                                  \
      const elem_type *const obj1,                                             \
      const elem_type *const obj2                                              \
    )                                                                          \
  );                                                                           \
  elem_type **elem_type##Array_GetData(const Array(elem_type) *const inst);

# define FUNC_ARRAY_BASICTYPE(elem_type)                                       \
  FUNC_ARRAY_COMMON(elem_type)                                                 \
  elem_type *elem_type##Array_Ref(                                             \
    const Array(elem_type) *const inst,                                        \
    const llong index                                                          \
  );                                                                           \
  boolean elem_type##Array_Equals(                                             \
    const Array(elem_type) *const arr1,                                        \
    const Array(elem_type) *const arr2,                                        \
    boolean (*const IsEqual)(                                                  \
      const elem_type *const obj1,                                             \
      const elem_type *const obj2                                              \
    )                                                                          \
  );                                                                           \
  elem_type *elem_type##Array_GetData(const Array(elem_type) *const inst);

# define FUNC_ARRAY_COMMON(elem_type)                                          \
  Array(elem_type) *elem_type##Array_Create(const llong capacity);             \
  Array(elem_type) *elem_type##Array_CopyOf(                                   \
    const Array(elem_type) *const other                                        \
  );                                                                           \
  void elem_type##Array_Delete(Array(elem_type) *const inst);                  \
  elem_type *elem_type##Array_Set(                                             \
    const Array(elem_type) *const inst,                                        \
    const llong index,                                                         \
    elem_type *const value                                                     \
  );                                                                           \
  boolean elem_type##Array_IsInBound(                                          \
    const Array(elem_type) *const inst,                                        \
    const llong index                                                          \
  );                                                                           \
  llong elem_type##Array_Offsetting(                                           \
    const Array(elem_type) *const inst,                                        \
    const llong index                                                          \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Assign(                                   \
    Array(elem_type) **const inst,                                             \
    const Array(elem_type) *const src                                          \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Fill(                                     \
    Array(elem_type) *const inst,                                              \
    const elem_type *value                                                     \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Resize(                                   \
    Array(elem_type) *const inst,                                              \
    const llong capacity                                                       \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Insert(                                   \
    Array(elem_type) *const inst,                                              \
    const llong index,                                                         \
    elem_type *const value                                                     \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Remove(                                   \
    Array(elem_type) *const inst,                                              \
    const llong index                                                          \
  );                                                                           \
  Array(elem_type) *elem_type##Array_Reverse(Array(elem_type) *const inst);    \
  llong elem_type##Array_GetCapacity(const Array(elem_type) *const inst);      \
  boolean elem_type##Array_GetReserved(const Array(elem_type) *const inst);    \
  boolean elem_type##Array_GetReversed(const Array(elem_type) *const inst);

/* Objects are Opaque Pointer designed. */
# define IMPL_ARRAY_OBJECT(elem_type)                                          \
DEFINE_ARRAY_OBJECT(elem_type)                                                 \
IMPL_ARRAY_COMMON(elem_type)                                                   \
inline elem_type *elem_type##Array_Ref(                                        \
  const Array(elem_type) *const inst,                                          \
  const llong index                                                            \
) {                                                                            \
  elem_type **const refref = elem_type##Array_RefRef(inst, index);             \
  if (!refref) {                                                               \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return *refref;                                                              \
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
  llong final_index = index;                                                   \
  if (index < 0) {                                                             \
    final_index = (inst->capacity - index - 1);                                \
  }                                                                            \
                                                                               \
  return &(inst->data[offsetting(Array(elem_type), inst, final_index)]);       \
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
  register llong i = 0;                                                        \
  refeach (elem_type, it, inst, {                                              \
    *it = *CopyOf(elem_type, ref(Array(elem_type), other, i));                 \
    i++;                                                                       \
/*  if (belong(RuntimeError)) {  */                                            \
/*    interrupted = i;           */                                            \
/*    break;                     */                                            \
/*  }                            */                                            \
  })                                                                           \
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
inline boolean elem_type##Array_Equals(                                        \
  const Array(elem_type) *const arr1,                                          \
  const Array(elem_type) *const arr2,                                          \
  boolean (*const IsEqual)(                                                    \
    const elem_type *const obj1,                                               \
    const elem_type *const obj2                                                \
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
    elem_type *const A = ref(Array(elem_type), arr1, i);                       \
    elem_type *const B = ref(Array(elem_type), arr2, i);                       \
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
inline elem_type **elem_type##Array_GetData(const Array(elem_type) *const inst)\
{                                                                              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return inst->data;                                                           \
}


# define IMPL_ARRAY_BASICTYPE(elem_type)                                       \
DEFINE_ARRAY_BASICTYPE(elem_type)                                              \
IMPL_ARRAY_COMMON(elem_type)                                                   \
inline elem_type *elem_type##Array_Ref(                                        \
  const Array(elem_type) *const inst,                                          \
  const llong index                                                            \
) {                                                                            \
  if (!inst || !isinbound(Array(elem_type), inst, index)) {                    \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  llong final_index = index;                                                   \
  if (index < 0) {                                                             \
    final_index = (inst->capacity - index - 1);                                \
  }                                                                            \
                                                                               \
  return &(inst->data[offsetting(Array(elem_type), inst, final_index)]);       \
}                                                                              \
                                                                               \
inline boolean elem_type##Array_Equals(                                        \
  const Array(elem_type) *const arr1,                                          \
  const Array(elem_type) *const arr2,                                          \
  boolean (*const IsEqual)(                                                    \
    const elem_type *const obj1,                                               \
    const elem_type *const obj2                                                \
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
    elem_type *const A = ref(Array(elem_type), arr1, i);                       \
    elem_type *const B = ref(Array(elem_type), arr2, i);                       \
    if (!A || !B || (*A != *B)) {                                              \
      return false;                                                            \
    }                                                                          \
  }                                                                            \
                                                                               \
  return true;                                                                 \
}                                                                              \
inline elem_type *elem_type##Array_GetData(const Array(elem_type) *const inst) \
{                                                                              \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  return inst->data;                                                           \
}


# define IMPL_ARRAY_COMMON(elem_type)                                          \
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
  /* Copying contents after all allocations are successful is intentional. */  \
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
  /* Copying array cells after other members is intentional. */                \
  assign(Array(elem_type), &inst, other);                                      \
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
  inst->capacity = 0;                                                          \
  inst->reserved = 0;                                                          \
  inst->reversed = 0;                                                          \
  Deallocate(inst);                                                            \
}                                                                              \
                                                                               \
inline boolean elem_type##Array_IsInBound(                                     \
  const Array(elem_type) *const inst,                                          \
  const llong index                                                            \
) {                                                                            \
  if (!inst) {                                                                 \
    return false;                                                              \
  }                                                                            \
                                                                               \
  return (index < inst->capacity);                                             \
}                                                                              \
                                                                               \
inline llong elem_type##Array_Offsetting(                                      \
  const Array(elem_type) *const inst,                                          \
  const llong index                                                            \
) {                                                                            \
  if (!inst) {                                                                 \
    return index;                                                              \
  }                                                                            \
                                                                               \
  /* Formula:  f(R,I,C) = I + R * (C - 1 - 2I) */                              \
  return ((index) + inst->reversed * (inst->capacity - 1 - 2 * (index)));      \
}                                                                              \
                                                                               \
inline Array(elem_type) *elem_type##Array_Assign(                              \
  Array(elem_type) **const inst,                                               \
  const Array(elem_type) *const src                                            \
) {                                                                            \
  if (!inst || !*inst) {                                                       \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (!src) {                                                                  \
    return *inst;                                                              \
  }                                                                            \
                                                                               \
  register llong i = 0;                                                        \
  refeach (elem_type, it, src, {                                               \
    *ref(Array(elem_type), *inst, i) = *it;                                    \
    i++;                                                                       \
  })                                                                           \
                                                                               \
  return *inst;                                                                \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_Fill(                                       \
  Array(elem_type) *const inst,                                                \
  const elem_type *value                                                       \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  refeach (elem_type, it, inst, {                                              \
    if (!value) {                                                              \
      *it = (elem_type)EMPTY;                                                  \
      continue;                                                                \
    }                                                                          \
                                                                               \
    *it = *value;                                                              \
  })                                                                           \
                                                                               \
  return inst;                                                                 \
}                                                                              \
                                                                               \
Array(elem_type) *elem_type##Array_Resize(                                     \
  Array(elem_type) *const inst,                                                \
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
  Array(elem_type) *array = Create(Array(elem_type), capacity);                \
  if (!array) {                                                                \
    return inst;                                                               \
  }                                                                            \
                                                                               \
  const llong delta = capacity - inst->capacity;                               \
                                                                               \
  /* Extending. */                                                             \
  if (delta > 0) {                                                             \
    memcpy(array->data, inst->data, inst->capacity * sizeof(elem_type));       \
    memset(array->data, 0, delta * sizeof(elem_type));                         \
                                                                               \
    return array;                                                              \
  }                                                                            \
                                                                               \
  memcpy(array->data, inst->data, capacity * sizeof(elem_type));               \
                                                                               \
  return array;                                                                \
}                                                                              \
                                                                               \
/* Insert before @index. */                                                    \
Array(elem_type) *elem_type##Array_Insert(                                     \
  Array(elem_type) *const inst,                                                \
  const llong index,                                                           \
  elem_type *const value                                                       \
) {                                                                            \
  if (!inst) {                                                                 \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  if (index < 0 || index > inst->capacity) {                                   \
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
  if (index > 0) {                                                             \
    memcpy(newarr->data, inst->data, index * sizeof(elem_type));               \
  }                                                                            \
                                                                               \
  *ref(Array(elem_type), newarr, index) = *value;                              \
                                                                               \
  llong remaining = inst->capacity - index;                                    \
  if (remaining > 0) {                                                         \
    memcpy(                                                                    \
      &newarr->data[index + 1],                                                \
      &inst->data[index],                                                      \
      remaining * sizeof(elem_type)                                            \
    );                                                                         \
  }                                                                            \
                                                                               \
  Delete(Array(elem_type), inst);                                              \
                                                                               \
  return newarr;                                                               \
}                                                                              \
                                                                               \
/* Remove before @index. */                                                    \
Array(elem_type) *elem_type##Array_Remove(                                     \
  Array(elem_type) *const inst,                                                \
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
  Array(elem_type) *newarr = Create(Array(elem_type), inst->capacity - 1);     \
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
  Delete(Array(elem_type), inst);                                              \
                                                                               \
  return newarr;                                                               \
}                                                                              \
                                                                               \
inline Array(elem_type) *elem_type##Array_Reverse(Array(elem_type) *const inst)\
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
inline llong elem_type##Array_GetCapacity(const Array(elem_type) *const inst)  \
{                                                                              \
  if (!inst) {                                                                 \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  return inst->capacity;                                                       \
}                                                                              \
                                                                               \
inline boolean elem_type##Array_GetReserved(const Array(elem_type) *const inst)\
{                                                                              \
  if (!inst) {                                                                 \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  return inst->reserved;                                                       \
}                                                                              \
                                                                               \
inline boolean elem_type##Array_GetReversed(const Array(elem_type) *const inst)\
{                                                                              \
  if (!inst) {                                                                 \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  return inst->reversed;                                                       \
}

#endif  /* COMPOUND_ARRAY_H */
