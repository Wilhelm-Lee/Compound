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

/** @file preprocessor.c */

#include "../inc/preprocessor.h"

struct Preprocessor {
  String *identifier;
  Array(String) *args;
};

Preprocessor *Preprocessor_Create(
  String *const identifier,
  Array(String) *const args
) {
  if (!identifier) {
    return null;
  }

  Preprocessor *const inst = Allocate(1, sizeof(Preprocessor));
  if (!inst) {
    return null;
  }

  inst->identifier = identifier;
  inst->args = args;

  return inst;
}

Preprocessor *Preprocessor_CopyOf(Preprocessor *const other)
{
  if (!other) {
    return null;
  }

  return Create(Preprocessor, other->identifier, other->args);
}

void Preprocessor_Delete(Preprocessor *const inst)
{
  if (!inst) {
    return;
  }

  erase(Array(String), inst->args);
  Delete(Array(String), inst->args);
  Delete(String, inst->identifier);
}

boolean Preprocessor_Equals(Preprocessor *const obj1, Preprocessor *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return Equals(String, obj1->identifier, obj2->identifier) &&
         Equals(Array(String), obj1->args, obj2->args, String_Equals);
}

String *Preprocessor_Literalise(Preprocessor *const inst)
{
  if (!inst) {
    return null;
  }

  String *const str_space = string(" ");

  /* Preprocessors require a NEWLINE at the end. */
  String *result = append(
    string("#"),
    inst->identifier,
    str_space,
    lit(
      Array(String),
      inst->args,
      null,
      str_space,
      null
    ),
    string(NEWLINE)
);

  Delete(String, str_space);

  return result;
}

IMPL_ARRAY(Preprocessor)
IMPL_ARRAY_LITERALISE(Preprocessor)
