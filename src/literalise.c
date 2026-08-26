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

/** @file literalise.c */

#include "../inc/literalise.h"

struct Literaliser {
  Literaliser *nested;
  String *(*Literalise)(void *inst);
};

Literaliser *Literaliser_Create(
  Literaliser *nested,
  String *(*Literalise)(void *inst)
) {
  Literaliser *inst = Allocate(1, sizeof(Literaliser));
  if (!inst) {
    return null;
  }

  inst->nested = nested;
  inst->Literalise = Literalise;

  return inst;
}

Literaliser *Literaliser_CopyOf(Literaliser *const other)
{
  if (!other) {
    return null;
  }

  return Create(Literaliser, other->nested, other->Literalise);
}

void Literaliser_Delete(Literaliser *const inst)
{
  if (!inst) {
    return;
  }

  Deallocate(inst);
}

boolean Literaliser_Equals(Literaliser *const obj1, Literaliser *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return obj1->nested == obj2->nested && obj1->Literalise == obj2->Literalise;
}

String *Literaliser_Literalise(Literaliser *const inst, void *const object)
{
  if (!inst) {
    return null;
  }

  String *buffer = Create(String, 0, sizeof(char));
  while (inst->nested) {
    buffer = concat(buffer, inst->Literalise(object));
  }

  return buffer;
}
