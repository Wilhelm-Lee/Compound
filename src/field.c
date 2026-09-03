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

/** @file field.c */

#include "../inc/field.h"

struct Field {
  Access access;
  Signature *signature;
  String *value;  // The initial value; can be null as when not given.
  llong numerical_identifier;
};

Field *Field_Create(
  const Access access,
  Signature *const signature,
  String *const value
) {
  if (!signature) {
    return null;
  }

  Field *const inst = Allocate(1, sizeof(Field));
  if (!inst) {
    return null;
  }

  inst->access = access;
  inst->signature = signature;
  inst->value = value;
  inst->numerical_identifier = -1;

  return inst;
}

Field *Field_CopyOf(Field *const other)
{
  if (!other) {
    return null;
  }

  return Create(
    Field,
    other->access,
    CopyOf(Signature, other->signature),
    CopyOf(String, other->value)
  );
}

void Field_Delete(Field *const inst)
{
  if (!inst) {
    return;
  }

  Delete(Signature, inst->signature);
  Delete(String, inst->value);
  Deallocate(inst);
}

boolean Field_Equals(Field *const obj1, Field *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return obj1->access == obj2->access &&
         Equals(Signature, obj1->signature, obj2->signature) &&
         Equals(String, obj1->value, obj2->value);
}

String *Field_Literalise(
  Field *const inst,
  boolean need_init_value,
  boolean need_semicolon
) {
  if (!inst) {
    return null;
  }

  String *rtn = lit(Signature, inst->signature, yes, yes, yes, yes, no);

  if (inst->value && need_init_value) {
    rtn = append(rtn, string(" = "), inst->value);
  }

  if (need_semicolon) {
    rtn = Concat(String, rtn, string("; "));
  }

  return rtn;
}

inline void _Field_SetNumericalIdentifier(
  Field *const inst,
  const llong numerical_identifier
) {
  if (!inst) {
    return;
  }

  inst->numerical_identifier = numerical_identifier;
}

inline String *Field_GetIdentifier(Field *const inst)
{
  if (!inst) {
    return nll;
  }

  return Getter(Signature, Identifier, inst->signature);
}

IMPL_ARRAY(Field)
IMPL_ARRAY_LITERALISE_CONFIGS(
  Field,
  need_init_value,
  need_semicolon,
  boolean need_init_value,
  boolean need_semicolon
)
