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

/** @file parameter.h */

#ifndef COMPOUND_PARAMETER_H
# define COMPOUND_PARAMETER_H

# include "string.h"
# include "literalisation.h"

typedef struct Parameter Parameter;

ARRAY(Parameter);

# define param(...)                                                            \
  (parameter(__VA_ARGS__))

# define parameter(type, ...)                                                  \
  (Create(Parameter, string(nameof(type)), string(nameof(__VA_ARGS__))))

Parameter *Parameter_Create(
  String *const type,
  String *const identifier
);
Parameter *Parameter_CopyOf(const Parameter *const other);
void Parameter_Delete(Parameter *const inst);
boolean Parameter_Equals(Parameter *const obj1, Parameter *const obj2);

String *Parameter_Literalise(const Parameter *const inst);

void Parameter_Recreate(FILE *const fp, const Parameter *const inst);

#endif  /* COMPOUND_PARAMETER_H */
