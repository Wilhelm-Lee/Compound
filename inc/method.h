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

/** @file method.h */

#ifndef COMPOUND_METHOD_H
# define COMPOUND_METHOD_H

# include "access.h"
# include "function.h"

typedef struct Method Method;

ARRAY(Method)
LITERALISE_ARGS(Method, boolean need_body)

# define method(class_identifier_literal, access_literal, returning_literal, identifier_literal, block, ...)\
  Create(Method, ACCESS_##access_literal, function(returning_literal, class_identifier_literal##_##identifier_literal, body(block), param(class_identifier_literal *const, this), __VA_ARGS__))

Method *Method_Create(const Access access, Function *const function);
Method *Method_CopyOf(Method *const other);
void Method_Delete(Method *const inst);
boolean Method_Equals(Method *const obj1, Method *const obj2);

#endif  /* COMPOUND_METHOD_H */
