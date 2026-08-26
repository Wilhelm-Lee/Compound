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

/** @file signature.h */

#ifndef COMPOUND_SIGNATURE_H
# define COMPOUND_SIGNATURE_H

# include "parameter.h"

typedef struct Signature Signature;

ARRAY(Signature)
LITERALISE_ARGS(Signature, boolean need_identifier)

Signature *Signature_Create(
  String *const returning,
  String *const identifier,
  Array(Parameter) *const parameters
);
Signature *Signature_CopyOf(const Signature *const other);
void Signature_Delete(Signature *const inst);
boolean Signature_Equals(
  Signature *const obj1,
  Signature *const obj2
);

#endif  /* COMPOUND_SIGNATURE_H */
