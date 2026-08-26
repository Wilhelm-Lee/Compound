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

/** @file destructor.h */

#ifndef COMPOUND_DESTRUCTOR_H
# define COMPOUND_DESTRUCTOR_H

# include "method.h"

typedef struct Destructor Destructor;

ARRAY(Destructor)
LITERALISE_ARGS(Destructor, boolean need_body)

# define _destructor(class_identifier_literal, super, block, ...)                        \
  Create(Destructor, super, method(class_identifier_literal, private, void, Destructor, block, __VA_ARGS__))

# define destructor(class_identifier_literal, block)                                     \
  _destructor(class_identifier_literal, null, block, null)

Destructor *Destructor_Create(Destructor *const super, Method *const method);
Destructor *Destructor_CopyOf(Destructor *const other);
void Destructor_Delete(Destructor *const inst);
boolean Destructor_Equals(Destructor *const obj1, Destructor *const obj2);

#endif  /* COMPOUND_DESTRUCTOR_H */
