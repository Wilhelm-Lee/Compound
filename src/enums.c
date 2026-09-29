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

/** @file enums.c */

#include "../inc/enums.h"

struct Enums {
  Array(Parameter) *params;
  String *identifier;
  Array(String) *values;
};

static Array(String) *ParseValues(String *const values_str)
{
  if (!values_str) {
    return nll;
  }

  if (blank(values_str)) {
    return Compose(Array(String), string(""));
  }

  return tokenise(values_str, ",");
}

Enums *Enums_Create(
  Array(Parameter) *const params,
  String *const identifier,
  String *const values_str
) {
  if (!params
   || !identifier
   || !Length(Array(Parameter), params)
   || blank(identifier)
  ) {
    return nll;
  }

  Enums *const inst = Allocate(sizeof(Enums));
  if (!inst) {
    return nll;
  }

  inst->params = CopyOf(Array(Parameter), params);
  inst->identifier = identifier;
  inst->values = ParseValues(values_str);

  return inst;
}

Enums *Enums_CopyOf(Enums *const other)
{
  if (!other) {
    return nll;
  }

  if (!other->params
   || !other->identifier
   || !Length(Array(Parameter), other->params)
   || blank(other->identifier)
  ) {
    return nll;
  }

  Enums *const inst = Allocate(sizeof(Enums));
  if (!inst) {
    return nll;
  }

  inst->params = CopyOf(Array(Parameter), other->params);
  inst->identifier = CopyOf(String, other->identifier);
  inst->values = CopyOf(Array(String), other->values);

  return inst;
}

void Enums_Delete(Enums *const inst)
{
  if (!inst) {
    return;
  }

  erase(Array(Parameter), inst->params);
  Delete(Array(Parameter), inst->params);
  Delete(String, inst->identifier);
  erase(Array(String), inst->values);
  Delete(Array(String), inst->values);
}

boolean Enums_Equals(Enums *const inst, Enums *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  return Equals(Array(Parameter), inst->params, other->params, Parameter_Equals)
      && Equals(String, inst->identifier, other->identifier)
      && Equals(Array(String), inst->values, other->values, String_Equals);
}

String *Enums_Literalise(Enums *const inst, int index_from_values_to_literalise)
{
  if (!inst) {
    return nll;
  }

  return append(
    nll,
    string("    "),
    ref(Array(Parameter), inst->params, index_from_values_to_literalise),
    string(" "),
    inst->identifier,
    string(" = "),
    ref(Array(String), inst->values, index_from_values_to_literalise),
    string(";"NL)
  );
}

IMPL_ARRAY(Enums)
IMPL_ARRAY_LITERALISE_ARGS(
  Enums,
  index_from_values_to_literalise,
  int index_from_values_to_literalise
)
