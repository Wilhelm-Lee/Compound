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
    return NULL;
  }

  Signature *const inst = Allocate(1, sizeof(Signature));
  if (!inst) {
    return NULL;
  }

  inst->returning = returning;
  inst->identifier = identifier;
  inst->parameters = parameters;

  return inst;
}

Signature *Signature_CopyOf(const Signature *const other)
{
  if (!other) {
    return NULL;
  }

  return Create(
    Signature,
    other->returning,
    other->identifier,
    other->parameters
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
         Equals(Array(Parameter), obj1->parameters, obj2->parameters, NULL);
}

String *Signature_Literalise(const Signature *const inst)
{
  if (!inst) {
    return null;
  }

  char *const flatten_returning = flatten(char, inst->returning);
  char *const flatten_identifier = flatten(char, inst->identifier);

  String *format = format("%s %s", flatten_returning, flatten_identifier);
  if (inst->parameters) {
    format = concat(format, string("("));
  }
  refeach (Parameter, param, inst->parameters, {
    if (!param) {
      break;
    }

    String *lit = lit(Parameter, param);
    if (!lit) {
      Delete(String, format);
      Deallocate(flatten_identifier);
      Deallocate(flatten_returning);
      return null;
    }

    format = concat(format, lit);
    format = concat(format, string(", "));
  })

  if (inst->parameters) {
    format = concat(format, string(")"));
    format = replace(&format, string(", )"), string(")"), 0);
  }

  Deallocate(flatten_identifier);
  Deallocate(flatten_returning);

  return format;
}

void Signature_Recreate(FILE *const fp, const Signature *const inst)
{
  if (!inst || !fp) {
    return;
  }

  String *const lit = lit(Signature, inst);
  char *const flatten = flatten(char, lit);

  fprintf(fp, "%s", flatten);

  Deallocate(flatten);
  Delete(String, lit);
}
