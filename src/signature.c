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

  Signature *const inst = Allocate(1, sizeof(Signature));
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

  return Create(
    Signature,
    CopyOf(String, other->returning),
    CopyOf(String, other->identifier),
    CopyOf(Array(Parameter), other->parameters)
  );
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
  Signature *const obj1,
  Signature *const obj2
) {
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return Equals(String, obj1->returning, obj2->returning) &&
         Equals(String, obj1->identifier, obj2->identifier) &&
         Equals(Array(Parameter), obj1->parameters, obj2->parameters, null);
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

  String *lit = null;

  if (need_returning) {
    lit = append(inst->returning, string(" "));
  }

  if (need_identifier) {
    lit = append(lit, inst->identifier);
  }

  if (need_parameters && inst->parameters) {
    lit = append(
      lit,
      string("("),
      lit(
        Array(Parameter),
        inst->parameters,
        null,
        string(", "),
        null,
        need_param_types,
        need_param_identifiers
      )
    );

    lit = Concat(String, lit, string(")"));
    // lit = replace(lit, string(", )"), string(")"), 0);
  }

  if (!inst->parameters) {
    lit = append(lit, string("(void)"));
  }

  lit = replace(lit, string("* "), string("*"), 0);
  lit = replace(lit, string("()"), string("(void)"), 0);

  return lit;
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
