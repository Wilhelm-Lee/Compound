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

#include "../inc/parameter.h"

struct Parameter {
  String *type;
  String *identifier;
};

Parameter *Parameter_Create(
  String *const type,
  String *const identifier
) {
  if (!type) {
    return NULL;
  }

  Parameter *inst = Allocate(1, sizeof(Parameter));
  if (!inst) {
    return NULL;
  }

  inst->type = type;
  inst->identifier = identifier;

  return inst;
}

Parameter *Parameter_CopyOf(const Parameter *const other)
{
  if (!other) {
    return NULL;
  }

  return Create(Parameter, other->type, other->identifier);
}

void Parameter_Delete(Parameter *const inst)
{
  if (!inst) {
    return;
  }

  Deallocate(inst->type);
  Deallocate(inst->identifier);
  Deallocate(inst);
}

boolean Parameter_Equals(Parameter *const obj1, Parameter *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return Equals(String, obj1->identifier, obj2->identifier) &&
         Equals(String, obj1->type, obj2->type);
}

String *Parameter_Literalise(const Parameter *const inst)
{
  if (!inst) {
    return null;
  }

  char *const flatten_type = flatten(char, inst->type);
  char *flatten_identifier = "";
  String *ret = format("%s", flatten_type);
  if (inst->identifier && !blank(inst->identifier)) {
    flatten_identifier = flatten(char, inst->identifier);
    ret = format("%s %s", flatten_type, flatten_identifier);
  }

  Deallocate(flatten_type);
  Deallocate(flatten_identifier);

  return ret;
}

void Parameter_Realise(FILE *const fp, const Parameter *const inst)
{
  if (!inst || !fp) {
    return;
  }

  String *const lit = lit(Parameter, inst);
  char *const flatten = flatten(char, lit);

  fprintf(fp, "%s", flatten);

  Deallocate(flatten);
  Delete(String, lit);
}

IMPL_ARRAY(Parameter);
