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

# include "constructor.h"
# include "destructor.h"
# include "field.h"
# include "method.h"
# include "memory_stack.h"

typedef struct Class Class;

ARRAY(Class)

# define class(access_literal, name_literal, ...)\
  (Create(Class, ACCESS_##access_literal, string(nameof(name_literal)))); typedef Class name_literal; ARRAY(name_literal) __VA_ARGS__

# define new(name_literal, ...)

Class *Class_Create(const Access access, String *const name);
Class *Class_CopyOf(Class *const other);
void Class_Delete(Class *const inst);
boolean Class_Equals(const Class *const obj1, const Class *const obj2);

#endif  /* COMPOUND_CLASS_H */
