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

/** @file preprocessor.h */

#ifndef COMPOUND_PREPROCESSOR_H
# define COMPOUND_PREPROCESSOR_H

# include "literalisation.h"
# include "string.h"

typedef struct Preprocessor Preprocessor;

ARRAY(Preprocessor);

# define macro(...)                                                            \
  preprocessor(__VA_ARGS__)

# define preprocessor(identifier, ...)                                         \
  Create(                                                                      \
    Preprocessor,                                                              \
    string(nameof(identifier)),                                                \
    ComposeFromCstr(                                                           \
      Array(String),                                                           \
      QUOTE_EACH(__VA_ARGS__)                                                  \
    )                                                                          \
  )                                                                            \

Preprocessor *Preprocessor_Create(
  String *const identifier,
  Array(String) *const args
);
Preprocessor *Preprocessor_CopyOf(Preprocessor *const other);
void Preprocessor_Delete(Preprocessor *const inst);
boolean Preprocessor_Equals(Preprocessor *const obj1, Preprocessor *const obj2);
String *Preprocessor_Literalise(Preprocessor *const inst);
void Preprocessor_Realise(FILE *const fp, Preprocessor *const inst);

#endif  /* COMPOUND_PREPROCESSOR_H */
