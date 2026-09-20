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

/** @file signature.c */

#include "../inc/signature.h"

struct Signature {
  String *returning;
  String *identifier;
  Array(Parameter) *parameters;
};

Signature *Signature_Create(
  String *const returning,
  String *const identifier,
  Array(Parameter) *const parameters
) {
  if (!identifier) {
    return null;
  }

  Signature *const inst = Allocate(sizeof(Signature));
  if (!inst) {
    return null;
  }

  inst->returning = returning;
  inst->identifier = identifier;
  inst->parameters = parameters ? parameters : noparam;

  return inst;
}

Signature *Signature_CopyOf(const Signature *const other)
{
  if (!other) {
    return null;
  }

  String *const returning = other->returning ? CopyOf(String, other->returning) : null;
  String *const identifier = other->identifier ? CopyOf(String, other->identifier) : null;
  Array(Parameter) *const parameters = other->parameters ? Clone(Array(Parameter), other->parameters) : null;

  if (!identifier || (other->returning && !returning) || (other->parameters && !parameters)) {
    Delete(String, returning);
    Delete(String, identifier);
    if (parameters) {
      erase(Array(Parameter), parameters);
      Delete(Array(Parameter), parameters);
    }
    return null;
  }

  Signature *const inst = Create(Signature, returning, identifier, parameters);
  if (!inst) {
    Delete(String, returning);
    Delete(String, identifier);
    if (parameters) {
      erase(Array(Parameter), parameters);
      Delete(Array(Parameter), parameters);
    }
    return null;
  }

  return inst;
}

void Signature_Delete(Signature *const inst)
{
  if (!inst) {
    return;
  }

  Delete(String, inst->returning);
  Delete(String, inst->identifier);
  erase(Array(Parameter), inst->parameters);
  Delete(Array(Parameter), inst->parameters);
  Deallocate(inst);
}

boolean Signature_Equals(
  Signature *const inst,
  Signature *const other
) {
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  if (inst->returning || other->returning) {
    if (!inst->returning || !other->returning || !Equals(String, inst->returning, other->returning)) {
      return false;
    }
  }

  return Equals(String, inst->identifier, other->identifier) &&
         Equals(Array(Parameter), inst->parameters, other->parameters, Parameter_Equals);
}

String *Signature_Literalise(
  Signature *const inst,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters
) {
  if (!inst) {
    return null;
  }

  String *const str_space = string(" ");
  String *const str_comma_space = string(", ");
  String *const str_open = string("(");
  String *const str_close = string(")");
  String *const str_void = string("(void)");
  String *const str_asterisk = string("*");
  String *const str_asterisk_space = string("* ");

  String *lit = null;

  if (need_returning && inst->returning) {
    lit = append(lit, inst->returning, str_space);
  }

  if (need_identifier && inst->identifier) {
    lit = append(lit, inst->identifier);
  }

  if (need_parameters) {
    const llong param_count = inst->parameters ? Length(Array(Parameter), inst->parameters) : 0;
    if (param_count > 0) {
      String *const params_str = lit(
        Array(Parameter),
        inst->parameters,
        null,
        str_comma_space,
        null,
        need_param_types,
        need_param_identifiers
      );
      lit = append(lit, str_open, params_str, str_close);
      Delete(String, params_str);
    } else {
      lit = append(lit, str_void);
    }
  }

  if (lit) {
    lit = replace(lit, str_asterisk_space, str_asterisk, 0);
  }

  Delete(String, str_asterisk_space);
  Delete(String, str_asterisk);
  Delete(String, str_void);
  Delete(String, str_close);
  Delete(String, str_open);
  Delete(String, str_comma_space);
  Delete(String, str_space);

  return lit ? lit : string("");
}

String *Signature_GetReturning(const Signature *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->returning;
}

String *Signature_GetIdentifier(const Signature *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->identifier;
}

Array(Parameter) *Signature_GetParameters(Signature *const inst)
{
  if (!inst) {
    return nll;
  }

  return inst->parameters;
}

IMPL_ARRAY(Signature)
IMPL_ARRAY_LITERALISE_CONFIGS(
  Signature,
  need_returning,
  need_identifier,
  need_param_types,
  need_param_identifiers,
  need_parameters,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters
)
