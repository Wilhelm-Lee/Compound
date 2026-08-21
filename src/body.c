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

/** @file body.c */

#include "../inc/body.h"

/* Either it is to provide a in-memory function;
 * or it is to adopt a string for realisation later.
 */
struct Body {
  void *(*Execution)(void);
  String *text;
};

Body *Body_Create(
  void *(*Execution)(void),
  String *const text
) {
  if (!Execution && !text) {
    return null;
  }

  Body *const inst = Allocate(1, sizeof(Body));
  if (!inst) {
    return NULL;
  }

  inst->Execution = Execution;
  inst->text = text;

  return inst;
}

Body *Body_CopyOf(const Body *const other)
{
  if (!other) {
    return NULL;
  }

  return Create(Body, other->Execution, other->text);
}

void Body_Delete(Body *const inst)
{
  if (!inst) {
    return;
  }

  Delete(String, inst->text);
  Deallocate(inst);
}

boolean Body_Equals(Body *const obj1, Body *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return obj1->Execution == obj2->Execution ||
         Equals(String, obj1->text, obj2->text);
}

String *Body_Literalise(const Body *const inst)
{
  if (!inst) {
    return null;
  }

  /* There's no need to literalise an address -- cannot be compiled. */
  if (inst->Execution) {
    return string("");
  }

  return inst->text;
}

void *Body_GetExecution(const Body *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->Execution;
}

String *Body_GetText(const Body *const inst)
{
  if (!inst) {
    return null;
  }

  return inst->text;
}

IMPL_ARRAY(Body);
