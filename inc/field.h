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

/** @file field.h */

#ifndef COMPOUND_FIELD_H
# define COMPOUND_FIELD_H

# include "access.h"
# include "signature.h"
# include "literalise.h"

typedef struct Field Field;

ARRAY(Field)
LITERALISE_ARGS(Field, boolean need_init_value, boolean need_semicolon)

# define create_field(                                                         \
    class_identifier_str,                                                      \
    access_literal,                                                            \
    returning_type_literal,                                                    \
    identifier_literal,                                                        \
    value_literal                                                              \
  )                                                                            \
  Create(                                                                      \
    Field,                                                                     \
    ACCESS_##access_literal,                                                   \
    Create(                                                                    \
      Signature,                                                               \
      string(nameof(returning_type_literal)),                                  \
      string(nameof(identifier_literal)),                                      \
      null                                                                     \
    ),                                                                         \
    string(nameof(value_literal))                                              \
  )

# define field(                                                                \
    access_literal,                                                            \
    returning_type_literal,                                                    \
    identifier_literal,                                                        \
    value_literal                                                              \
  )                                                                            \
  (call(                                                                       \
    Class,                                                                     \
    AddField,                                                                  \
    this,                                                                      \
    create_field(                                                              \
      CLASS_IDENTIFIER_STR,                                                    \
      access_literal,                                                          \
      returning_type_literal,                                                  \
      identifier_literal,                                                      \
      value_literal                                                            \
    )                                                                          \
  ))

Field *Field_Create(
  const Access access,
  Signature *const signature,
  String *const value
);
Field *Field_CopyOf(Field *const other);
void Field_Delete(Field *const inst);
boolean Field_Equals(Field *const obj1, Field *const obj2);
String *Field_GetIdentifier(Field *const inst);
void _Field_SetNumericalIdentifier(
  Field *const inst,
  const llong numerical_identifier
);

#endif  /* COMPOUND_FIELD_H */
