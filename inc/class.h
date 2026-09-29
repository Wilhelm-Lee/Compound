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

/** @file class.h */

#ifndef COMPOUND_CLASS_H
# define COMPOUND_CLASS_H

# include <time.h>

# include "constructor.h"
# include "destructor.h"
# include "field.h"
# include "literalise.h"
# include "memory_stack.h"
# include "method.h"
# include "stream.h"

typedef struct Class Class;

/* This effectively declares for every future Class types. */
ARRAY(Class)
LITERALISE_ARGS(Class, boolean need_member_definition)

extern void _Class_EraseFields(Class *const inst);
extern void _Class_EraseMethods(Class *const inst);

# define class(access_visibility_literal, identifier_literal, ...)             \
  typedef Class identifier_literal;                                            \
  identifier_literal *c_##identifier_literal = Create(                         \
    Class,                                                                     \
    ACCESS_VISIBILITY_##access_visibility_literal,                             \
    string(nameof(identifier_literal)),                                        \
    null,                                                                      \
    null,                                                                      \
    null,                                                                      \
    null,                                                                      \
    null,                                                                      \
    null,                                                                      \
    null                                                                       \
  );                                                                           \
  ARRAY(identifier_literal)                                                    \
  {                                                                            \
    identifier_literal *const this = c_##identifier_literal;                   \
    ig this;                                                                   \
    identifier_literal *super = nll;                                           \
    ig super;  /* Kept for user to modify themselves. */                       \
    String *const CLASS_IDENTIFIER_STR = string(                               \
      nameof(identifier_literal)                                               \
    );                                                                         \
    ig CLASS_IDENTIFIER_STR;                                                   \
    constructor (private, (void), {})  /* The default constructor. */          \
    destructor ()  /* The default destructor. */                               \
    __VA_ARGS__                                                                \
    Class_Inherit(this, super);                                                \
  }

/* record (public, ExitCodes, (int code)) */
/* record (protected, User, (String *name, int id)) */
/* record (private, RGBChannel, (int red_value, int green_value, int blue_value)) */
/* It's translated to a struct anyway; no performance loss nor design redundancy. */
# define record(                                                               \
    access_visibility_literal,                                                 \
    identifier_literal,                                                        \
    lazy_param_clusters,                                                       \
    ...                                                                        \
  )                                                                            \
  class (access_visibility_literal, identifier_literal, {                      \
    constructor (public, lazy_param_clusters, {})                              \
    Array(Parameter) *const _record_types = lazy_params lazy_param_clusters;   \
    __VA_ARGS__                                                                \
    /* Records are forbidden inheriting. */                                    \
    super = nll;                                                               \
  })

# define enums(identifier_literal, ...)                                        \
  (Create(Enums, _record_types, string(nameof(identifier_literal)), string(#__VA_ARGS__)))

# define inherit(super_class_name_literal)                                     \
  super = c_##super_class_name_literal;

# define override(                                                             \
    access_visibility_literal,                                                 \
    return_type_literal,                                                       \
    method_name_literal,                                                       \
    lazy_param_clusters,                                                       \
    ...                                                                        \
  )                                                                            \
  (                                                                            \
    Class_OverrideMethod(                                                      \
      this,                                                                    \
      string(nameof(method_name_literal)),                                     \
      string(#__VA_ARGS__))                                                    \
  );

# define virtual(                                                              \
    access_visibility_literal,                                                 \
    return_type_literal,                                                       \
    identifier_literal,                                                        \
    lazy_param_clusters                                                        \
  )                                                                            \
  method(                                                                      \
    access_visibility_literal,                                                 \
    return_type_literal,                                                       \
    identifier_literal,                                                        \
    lazy_param_clusters,                                                       \
    ;                                                                          \
  )

# define new(class_name_literal, ...)                                          \
  (Create(class_name_literal, __VA_ARGS__))

# define del(class_name_literal, inst)                                         \
  Delete(class_name_literal, inst)

# define of(class_name_literal, field_name_literal)                            \
  EMPTY

# define invoke(...)                                                           \
  EMPTY

Class *Class_Create(
  const AccessVisibility visibility,
  String *const identifier,
  Class *const super,
  Array(Field) *const fields,
  Array(Method) *const methods,
  Constructor *const constructor,
  Destructor *const destructor,
  Method *const Equals,
  Method *const Literalise
);
Class *Class_CopyOf(Class *const other);
void Class_Delete(Class *const inst);
boolean Class_Equals(const Class *const inst, const Class *const other);
boolean Class_Recreate(
  Stream *const header,
  Stream *const source,
  Class *const inst
);
Class *Class_AddField(Class *const inst, Field *const field);
Class *Class_AddMethod(Class *const inst, Method *const method);
void Class_Inherit(Class *const inst, Class *const super);
void Class_OverrideMethod(Class *const inst, String *const method_identifier, String *const body_text);
void Class_SetConstructor(Class *const inst, Constructor *const constructor);
void Class_SetDestructor(Class *const inst, Destructor *const destructor);

Field *Class_GetFieldByIdentifier(
  Class *const inst,
  String *const field_identifier
);
Method *Class_GetMethodByIdentifier(
  Class *const inst,
  String *const method_identifier
);

#endif  /* COMPOUND_CLASS_H */
