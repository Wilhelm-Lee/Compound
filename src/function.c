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

/** @file function.c */

#include "../inc/function.h"

struct Function {
  Signature *signature;
  Body *body;
};

Function *Function_Create(
  Signature *const signature,
  Body *const body
) {
  if (!signature || !body) {
    return null;
  }

  Function *const inst = Allocate(1, sizeof(Function));
  if (!inst) {
    return null;
  }

  inst->signature = signature;
  inst->body = body;

  return inst;
}

Function *Function_CopyOf(const Function *const other)
{
  if (!other) {
    return null;
  }

  return Create(
    Function,
    CopyOf(Signature, other->signature),
    CopyOf(Body, other->body)
  );
}

void Function_Delete(Function *const inst)
{
  if (!inst) {
    return;
  }

  Delete(Body, inst->body);
  Delete(Signature, inst->signature);
  Deallocate(inst);
}

boolean Function_Equals(Function *const obj1, Function *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return Equals(Signature, obj1->signature, obj2->signature) &&
         Equals(Body, obj1->body, obj2->body);
}

String *Function_Literalise(
  Function *const inst,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters,
  boolean need_body,
  boolean need_semicolon
) {
  if (!inst) {
    return null;
  }

  String *lit = lit(
    Signature,
    inst->signature,
    need_returning,
    need_identifier,
    need_param_types,
    need_param_identifiers,
    need_parameters
  );

  if (need_body) {
    lit = append(lit, lit(Body, inst->body));
    return lit;
  }

  if (need_semicolon) {
    lit = append(lit, string(";"), string(NL));
  }

  return lit;
}

Signature *Function_GetSignature(const Function *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->signature;
}

Body *Function_GetBody(const Function *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->body;
}

IMPL_ARRAY(Function)
IMPL_ARRAY_LITERALISE_CONFIGS(
  Function,
  need_returning,
  need_identifier,
  need_param_types,
  need_param_identifiers,
  need_parameters,
  need_body,
  need_semicolon,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters,
  boolean need_body,
  boolean need_semicolon
)
