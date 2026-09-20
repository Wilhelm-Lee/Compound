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

/** @file method.c */

#include "../inc/method.h"

struct Method {
  Access access;
  String *class_identifier;
  Function *function;
};

Method *Method_Create(
  const Access access,
  String *const class_identifier,
  Function *const function
) {
  if (!class_identifier || !function) {
    return null;
  }

  Method *const inst = Allocate(sizeof(Method));
  if (!inst) {
    return null;
  }

  inst->access = access;
  inst->class_identifier = class_identifier;
  inst->function = function;

  return inst;
}

Method *Method_CopyOf(Method *const other)
{
  if (!other) {
    return null;
  }

  String *const class_identifier = CopyOf(String, other->class_identifier);
  Function *const function = CopyOf(Function, other->function);
  if (!class_identifier || !function) {
    return null;
  }

  Method *const inst = Create(Method, other->access, class_identifier, function);
  if (!inst) {
    Delete(String, class_identifier);
    Delete(Function, function);
    return null;
  }

  return inst;
}

void Method_Delete(Method *const inst)
{
  if (!inst) {
    return;
  }

  Delete(Function, inst->function);
  Delete(String, inst->class_identifier);
  Deallocate(inst);
}

boolean Method_Equals(Method *const inst, Method *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  return inst->access == other->access &&
         Equals(Function, inst->function, other->function);
}

String *Method_Literalise(
  Method *const inst,
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

  return lit(
    Function,
    inst->function,
    need_returning,
    need_identifier,
    need_param_types,
    need_param_identifiers,
    need_parameters,
    need_body,
    need_semicolon
  );
}

inline Access Method_GetAccess(const Method *const inst)
{
  if (!inst) {
    return ACCESS_PRIVATE;
  }

  return inst->access;
}

inline Function *Method_GetFunction(const Method *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->function;
}

inline String *Method_GetIdentifier(Method *const inst)
{
  if (!inst) {
    return nll;
  }

  return Getter(
    Signature,
    Identifier,
    Getter(Function, Signature, inst->function)
  );
}

IMPL_ARRAY(Method)
IMPL_ARRAY_LITERALISE_CONFIGS(
  Method,
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
