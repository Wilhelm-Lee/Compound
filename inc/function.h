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

/** @file function.h */

#ifndef COMPOUND_FUNCTION_H
# define COMPOUND_FUNCTION_H

# include "body.h"
# include "signature.h"

typedef struct Function Function;

ARRAY(Function)
LITERALISE_ARGS(
  Function,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters,
  boolean need_body,
  boolean need_semicolon
)

# define function(returning_type_str, identifier_str, param_clusters, ...)     \
  Create(                                                                      \
    Function,                                                                  \
    Create(                                                                    \
      Signature,                                                               \
      returning_type_str,                                                      \
      identifier_str,                                                          \
      param_clusters                                                           \
    ),                                                                         \
    body(__VA_ARGS__)                                                          \
  )

Function *Function_Create(Signature *const signature, Body *const body);
Function *Function_CopyOf(const Function *const other);
void Function_Delete(Function *const inst);
boolean Function_Equals(Function *const obj1, Function *const obj2);
Signature *Function_GetSignature(const Function *const inst);
Body *Function_GetBody(const Function *const inst);

#endif  /* COMPOUND_FUNCTION_H */
