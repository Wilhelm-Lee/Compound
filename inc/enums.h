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

/** @file enums.h */

#ifndef COMPOUND_ENUMS_H
# define COMPOUND_ENUMS_H

# include "field.h"

typedef struct Enums Enums;

ARRAY(Enums)
LITERALISE_ARGS(Enums, int index_from_values_to_literalise)

Enums *Enums_Create(
  Array(Parameter) *const param,
  String *const identifier,
  String *const values_str
);
Enums *Enums_CopyOf(Enums *const other);
void Enums_Delete(Enums *const inst);
boolean Enums_Equals(Enums *const inst, Enums *const other);

#endif  /* COMPOUND_ENUMS_H */
