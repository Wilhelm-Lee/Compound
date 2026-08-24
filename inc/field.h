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

typedef struct Field Field;

ARRAY(Field)

# define field(access_literal, type_literal, identifier_literal, ...)          \
  Create(                                                                      \
    Field,                                                                     \
    ACCESS_##access_literal,                                                   \
    Create(                                                                    \
      Signature,                                                               \
      string(nameof(type_literal)),                                            \
      string(nameof(identifier_literal)),                                      \
      null                                                                     \
    ),                                                                         \
    string(nameof(__VA_ARGS__))                                                \
  )

Field *Field_Create(
  const Access access,
  Signature *const signature,
  String *const value
);
Field *Field_CopyOf(Field *const other);
void Field_Delete(Field *const inst);
boolean Field_Equals(Field *const obj1, Field *const obj2);

String *Field_Literalise(const Field *const inst);
void Field_Recreate(FILE *const fp, const Field *const inst);

#endif  /* COMPOUND_FIELD_H */
