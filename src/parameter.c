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

  Parameter *const inst = Allocate(sizeof(Parameter));
  if (!inst) {
    return null;
  }

  inst->type = CopyOf(String, other->type);
  inst->identifier = CopyOf(String, other->identifier);

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

  return Equals(String, inst->identifier, other->identifier)
      && Equals(String, inst->type, other->type);
}

Array(Parameter) *Parameter_CreateMultiple(const int cluster_count, ...)
{
  if (!cluster_count) {
    return array(Parameter, 0);
  }

  va_list ap;
  va_start(ap, cluster_count);
  Array(Parameter) *const inst = array(Parameter, cluster_count);
  register int actual_offset = 0;
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

Array(Parameter) *Parameter_CreateLazyMultiple(
  const char *restrict const parameter_clusters_cstr
) {
  String *const parameter_clusters_str = string(parameter_clusters_cstr);
  if (!parameter_clusters_str) {
    return nll;
  }

  /* Lazy parameters require tokenisation to separate each one by commas. */
  Array(String) *const tokens = tokenise(parameter_clusters_str, ",");
  if (!tokens) {
    Delete(String, parameter_clusters_str);
    return nll;
  }

  Array(Parameter) *const parameters = array(Parameter, Length(Array(String), tokens));
  register int i = 0;
  refrefeach (Parameter, param, parameters, {
    String *token = trim(CopyOf(String, ref(Array(String), tokens, i)));

    register const int space_cutidx = lastoccur(token, ' ', 0);
    register const int asterisk_cutidx = lastoccur(token, '*', 0) + 1;
    /* Skip the asterisk
     * -- @strcut includes the byte on @index, which is the asterisk. */
    register const int cutidx = max(space_cutidx, asterisk_cutidx);

    if (cutidx >= 0) {
      String *const identifier = strcut(&token, cutidx);
      *param = Create(Parameter, token, identifier);
    }

    i ++;
  })

  return parameters;
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

  if (need_type) {
    lit = CopyOf(String, inst->type);
  }

  if (lit) {
    lit = Concat(String, lit, str_space);
  }

  /* No @identifier needed -- @type already includes all information given. */
  /* { .type = "int argc", .identifier = nll }
   * instead of
   * { .type = "int", .identifier = "argc" }
   */
  if (need_identifier) {
    lit = Concat(String, lit, inst->identifier);
  }

  Delete(String, str_space);

  return lit;
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
