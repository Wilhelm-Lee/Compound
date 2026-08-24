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
  Function *function;
};

Method *Method_Create(const Access access, Function *const function)
{
  Method *const inst = Allocate(1, sizeof(Method));
  if (!inst) {
    return null;
  }

  inst->access = access;
  inst->function = function;
  if (!inst->function) {
    Deallocate(inst);
    return null;
  }

  return inst;
}

Method *Method_CopyOf(Method *const other)
{
  if (!other) {
    return null;
  }

  return Create(Method, other->access, other->function);
}

void Method_Delete(Method *const inst)
{
  if (!inst) {
    return;
  }

  Delete(Function, inst->function);
  Deallocate(inst);
}

boolean Method_Equals(Method *const obj1, Method *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return obj1->access == obj2->access &&
         Equals(Function, obj1->function, obj2->function);
}

IMPL_ARRAY(Method)
