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

/** @file constructor.h */

#ifndef COMPOUND_CONSTRUCTOR_H
# define COMPOUND_CONSTRUCTOR_H

# include "method.h"

typedef struct Constructor Constructor;

ARRAY(Constructor)
LITERALISE_ARGS(Constructor, boolean need_body)

# define _constructor(class_identifier_literal, super, access_literal, block, ...)        \
  Create(Constructor, super, method(class_identifier_literal, access_literal, Class *, Constructor, block, __VA_ARGS__))

# define constructor(class_identifier_literal, access_literal, block, ...)                               \
  _constructor(class_identifier_literal, null, access_literal, block, __VA_ARGS__)

Constructor *Constructor_Create(Constructor *const super, Method *const method);
Constructor *Constructor_CopyOf(Constructor *const other);
void Constructor_Delete(Constructor *const inst);
boolean Constructor_Equals(Constructor *const obj1, Constructor *const obj2);

#endif  /* COMPOUND_CONSTRUCTOR_H */
