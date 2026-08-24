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

/** @file class.c */

#include "../inc/class.h"

struct Class {
  Access access;
  String *identifier;
  Class *super;
  Class *this;
  Array(Method) *methods;
  Array(Field) *fields;
  Constructor *constructor;
  Destructor *destructor;
};

Class *Class_Create(const Access access, String *const identifier)
{
  if (!identifier || blank(identifier)) {
    return null;
  }

  Class *const inst = Allocate(1, sizeof(Class));
  if (!inst) {
    return null;
  }

  inst->access = access;
  inst->identifier = identifier;
  inst->super = null;
  inst->this = inst;
  inst->methods = array(Method, 0);
  if (!inst->methods) {
    Deallocate(inst);
    return null;
  }
  inst->fields = array(Field, 0);
  if (!inst->fields) {
    erase(Array(Method), inst->methods);
    Delete(Array(Method), inst->methods);
    Deallocate(inst);
    return null;
  }

  inst->constructor = constructor(private, {}, param(Class *const, this));
  if (!inst->constructor) {
    erase(Array(Method), inst->methods);
    Delete(Array(Method), inst->methods);
    erase(Array(Field), inst->fields);
    Delete(Array(Field), inst->fields);
    Deallocate(inst);
    return null;
  }

  inst->destructor = destructor(private, {});
  if (!inst->destructor) {
    Delete(Constructor, inst->constructor);
    erase(Array(Method), inst->methods);
    Delete(Array(Method), inst->methods);
    erase(Array(Field), inst->fields);
    Delete(Array(Field), inst->fields);
    Deallocate(inst);
    return null;
  }

  return inst;
}

Class *Class_CopyOf(Class *const other)
{
  if (!other) {
    return NULL;
  }

  Class *const inst = Create(Class, other->access, other->identifier);
  if (!inst) {
    return null;
  }

  /* With sharing the same @name as well as the @predecessor, it is
   * not ideal to distinguish the duplication and the original instance using
   * conventional approaches.
   *
   * It is worth noticing that to be able to identify two instances of Class,
   * users are therefore needed to use UID, which, is effectively separated
   * logically from the fields embedded in the struct, and, can be utilised to
   * distinguish instances apart.
   *
   * With that said, Equals is recognising the @identifier for comparison over
   * the equality check on @name.
   */
  String *const copy = string(" copy");
  if (!copy) {
    Delete(Class, inst);
    return null;
  }

  inst->identifier = concat(other->identifier, copy);

  Delete(String, copy);

  return inst;
}

void Class_Delete(Class *const inst)
{
  if (!inst) {
    return;
  }

  Delete(String, inst->identifier);
  erase(Array(Method), inst->methods);
  Delete(Array(Method), inst->methods);
  Delete(Constructor, inst->constructor);
  Delete(Destructor, inst->destructor);
  erase(Array(Field), inst->fields);
  Delete(Array(Field), inst->fields);
  Deallocate(inst);
}

boolean Class_Equals(const Class *const obj1, const Class *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return
    Equals(String, obj1->identifier, obj2->identifier) &&
    obj1->super == obj2->super &&
    Equals(Array(Method), obj1->methods, obj2->methods, Method_Equals);
}

IMPL_ARRAY(Class)
