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

/** @file constructor.c */

#include "../inc/constructor.h"

struct Constructor {
  Constructor *super;  // As for Constructors, @super is the first one to call.
  Method *method;
};

Constructor *Constructor_Create(Constructor *const super, Method *const method)
{
  if (!method) {
    return null;
  }

  Constructor *const inst = Allocate(sizeof(Constructor));
  if (!inst) {
    return null;
  }

  inst->super = super;
  inst->method = method;

  return inst;
}

Constructor *Constructor_CopyOf(Constructor *const other)
{
  if (!other) {
    return null;
  }

  return Create(
    Constructor,
    CopyOf(Constructor, other->super),
    CopyOf(Method, other->method)
  );
}

void Constructor_Delete(Constructor *const inst)
{
  if (!inst) {
    return;
  }

  Delete(Method, inst->method);
  Deallocate(inst);
}

boolean Constructor_Equals(Constructor *const inst, Constructor *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  return inst->super == other->super &&
         Equals(Method, inst->method, other->method);
}

String *Constructor_Literalise(
  Constructor *const inst,
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
    Method,
    inst->method,
    need_returning,
    need_identifier,
    need_param_types,
    need_param_identifiers,
    need_parameters,
    need_body,
    need_semicolon
  );
}

void Constructor_Inherit(Constructor *const inst, Constructor *const super)
{
  if (!inst || !super) {
    return;
  }

  inst->super = super;

  Body *const inst_body = Getter(
    Function, Body,
    Getter(Method, Function, Getter(Constructor, Method, inst))
  );
  Body *const super_body = Getter(
    Function, Body,
    Getter(Method, Function, Getter(Constructor, Method, super))
  );

  if (!inst_body || !super_body) {
    return;
  }

  String *const super_text = Getter(Body, Text, super_body);
  String *const inst_text = Getter(Body, Text, inst_body);

  String *const combined = append(nll, super_text, inst_text);

  Setter(Body, Text, inst_body, combined);
}

Constructor *Constructor_GetSuper(const Constructor *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->super;
}

Method *Constructor_GetMethod(const Constructor *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->method;
}

void Constructor_SetSuper(Constructor *const inst, Constructor *const super)
{
  if (!inst) {
    return;
  }

  inst->super = super;
}

IMPL_ARRAY(Constructor)
IMPL_ARRAY_LITERALISE_CONFIGS(
  Constructor,
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
