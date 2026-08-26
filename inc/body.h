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

/** @file body.h */

#ifndef COMPOUND_BODY_H
# define COMPOUND_BODY_H

# include "string.h"

typedef struct Body Body;

ARRAY(Body)
LITERALISE(Body)

# define body(...)                                                             \
  Create(Body, null, string(nameof(__VA_ARGS__)))

Body *Body_Create(void *(*Execution)(void *), String *const text);
Body *Body_CopyOf(const Body *const other);
void Body_Delete(Body *const inst);
boolean Body_Equals(Body *const obj1, Body *const obj2);
String *Body_GetText(const Body *const inst);

#endif  /* COMPOUND_BODY_H */
