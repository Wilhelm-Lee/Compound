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
    return NULL;
  }

  Function *const inst = Allocate(1, sizeof(Function));
  if (!inst) {
    return NULL;
  }

  inst->signature = signature;
  inst->body = body;

  return inst;
}

Function *Function_CopyOf(const Function *const other)
{
  if (!other) {
    return NULL;
  }

  return Create(Function, other->signature, other->body);
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

String *Function_Literalise(Function *const inst, boolean need_body)
{
  if (!inst) {
    return null;
  }

  if (!need_body) {
    return concat(lit(Signature, inst->signature, yes), string(";"));
  }

  return concat(lit(Signature, inst->signature, yes), lit(Body, inst->body));
}

IMPL_ARRAY(Function)
IMPL_ARRAY_LITERALISE_CONFIGS(Function, need_body, boolean need_body)
