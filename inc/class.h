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

typedef struct Class Class;

/* This effectively declares for every future Class types. */
ARRAY(Class)
LITERALISE_ARGS(Class, boolean want_fancy, boolean need_member_definition)

# define class(access_literal, identifier_literal, ...)\
  Class *c_##identifier_literal = Create(\
    Class,\
    ACCESS_##access_literal,\
    string(nameof(identifier_literal)),\
    null,\
    null,\
    null,\
    null,\
    null,\
    null,\
    null\
  );\
  typedef Class identifier_literal;\
  ARRAY(identifier_literal)\
  {\
    Class *const this = c_##identifier_literal;\
    Class *super = nll;\
    ig this;\
    String *const CLASS_IDENTIFIER_STR = string(\
      nameof(identifier_literal)\
    );\
    ig CLASS_IDENTIFIER_STR;\
    destructor()  /* The default destructor. */\
    __VA_ARGS__\
    Class_Inherit(this, super);\
  }

# define inherit(super_class_name_literal)\
  super = c_##super_class_name_literal;

# define override(method_name_literal, ...)\
  {\
    Method *const found = Class_GetMethodByIdentifier(this, string(nameof(method_name_literal)));\
    if (found) {\
      Body *const body = Getter(Function, Body, Getter(Method, Function, found));\
      Delete(String, Getter(Body, Text, body));\
      Setter(Body, Text, body, string(#__VA_ARGS__));\
    }\
  }

# define new(class_name_literal, ...)\
  (Create(class_name_literal, __VA_ARGS__))

# define del(class_name_literal, inst)\
  Delete(class_name_literal, inst)

# define of(class_name_literal, field_name_literal)\
  EMPTY

# define invoke(...)\
  EMPTY

Class *Class_Create(
  const Access access,
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
boolean Class_Equals(const Class *const obj1, const Class *const obj2);
boolean Class_Recreate(
  FILE *const header,
  FILE *const source,
  Class *const inst
);
Class *Class_AddField(Class *const inst, Field *const field);
Class *Class_AddMethod(Class *const inst, Method *const method);
void Class_Inherit(Class *const inst, Class *const super);
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
