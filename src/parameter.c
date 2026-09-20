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
    return null;
  }

  Parameter *inst = Allocate(sizeof(Parameter));
  if (!inst) {
    return null;
  }

  inst->type = CopyOf(String, type);
  inst->identifier = CopyOf(String, identifier);

  return inst;
}

Parameter *Parameter_CopyOf(const Parameter *const other)
{
  if (!other) {
    return null;
  }

  String *const type = CopyOf(String, other->type);
  String *const identifier = other->identifier ? CopyOf(String, other->identifier) : null;

  if (!type || (other->identifier && !identifier)) {
    Delete(String, type);
    Delete(String, identifier);
    return null;
  }

  Parameter *const inst = Create(Parameter, type, identifier);
  if (!inst) {
    Delete(String, type);
    Delete(String, identifier);
    return null;
  }

  return inst;
}

void Parameter_Delete(Parameter *const inst)
{
  if (!inst) {
    return;
  }

  Delete(String, inst->type);
  Delete(String, inst->identifier);
  Deallocate(inst);
}

boolean Parameter_Equals(Parameter *const inst, Parameter *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  if (inst->identifier || other->identifier) {
    if (!inst->identifier || !other->identifier || !Equals(String, inst->identifier, other->identifier)) {
      return false;
    }
  }

  return Equals(String, inst->type, other->type);
}

Array(Parameter) *Parameter_CreateMultiple(const llong cluster_count, ...)
{
  if (!cluster_count) {
    return array(Parameter, 0);
  }

  va_list ap;
  va_start(ap, cluster_count);
  Array(Parameter) *const inst = array(Parameter, cluster_count);
  register llong actual_offset = 0;
  loop (i, cluster_count) {
    Parameter *const cluster = va_arg(ap, Parameter *);
    if (!cluster) {
      continue;
    }

    set(Array(Parameter), inst, actual_offset, cluster);
    actual_offset++;
  }
  va_end(ap);

  return inst;
}

String *Parameter_Literalise(
  Parameter *const inst,
  boolean need_type,
  boolean need_identifier
) {
  if (!inst) {
    return null;
  }

  String *const str_space = string(" ");
  String *lit = null;

  if (need_type && inst->type) {
    lit = append(lit, inst->type);
  }

  if (need_identifier && inst->identifier) {
    if (lit) {
      lit = append(lit, str_space, inst->identifier);
    } else {
      lit = append(lit, inst->identifier);
    }
  }

  Delete(String, str_space);

  return lit ? lit : string("");
}

String *Parameter_GetType(const Parameter *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->type;
}

String *Parameter_GetIdentifier(const Parameter *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->identifier;
}

IMPL_ARRAY(Parameter)
IMPL_ARRAY_LITERALISE_CONFIGS(Parameter, need_type, need_identifier, boolean need_type, boolean need_identifier)
