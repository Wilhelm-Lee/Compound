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

# define function(returning, identifier, body, ...)                            \
  Create(                                                                      \
    Function,                                                                  \
    Create(                                                                    \
      Signature,                                                               \
      string(nameof(returning)),                                               \
      string(nameof(identifier)),                                              \
      Compose(Array(Parameter), __VA_ARGS__)                                   \
    ),                                                                         \
    body                                                                       \
  )

/* Before the generation, this macro is suppose to be making no difference to
 * the current version of the source code. */
# define invoke(identifier, ...)

/* This is the actual macro used after the generation.
 * It is appended to the "user/header.h". */
# define _invoke(identifier, ...)                                               \
  CONCAT(identifier, (__VA_ARGS__))

Function *Function_Create(Signature *const signature, Body *const body);
Function *Function_CopyOf(const Function *const other);
void Function_Delete(Function *const inst);
boolean Function_Equals(Function *const obj1, Function *const obj2);

String *Function_Literalise(const Function *const inst);
void Function_Recreate(FILE *const fp, const Function *const inst);

#endif  /* COMPOUND_FUNCTION_H */
