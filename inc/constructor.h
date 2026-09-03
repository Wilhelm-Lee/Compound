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

/** @file constructor.h */

#ifndef COMPOUND_CONSTRUCTOR_H
# define COMPOUND_CONSTRUCTOR_H

# include "method.h"

typedef struct Constructor Constructor;

ARRAY(Constructor)
LITERALISE_ARGS(
  Constructor,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters,
  boolean need_body,
  boolean need_semicolon
)

# define constructor(                                                          \
    param_clusters,                                                            \
    ...                                                                        \
  )                                                                            \
  (call(                                                                       \
    Class,                                                                     \
    SetConstructor,                                                            \
    this,                                                                      \
    Create(                                                                    \
      Constructor,                                                             \
      null,                                                                    \
      Create(                                                                  \
        Method,                                                                \
        ACCESS_PUBLIC,                                                         \
        Create(                                                                \
          Function,                                                            \
          Create(                                                              \
            Signature,                                                         \
            append(CLASS_IDENTIFIER_STR, string(" *")),                        \
            append(CLASS_IDENTIFIER_STR, string("_"), string(nameof(Create))), \
            param_clusters                                                     \
          ),                                                                   \
          body(__VA_ARGS__)                                                    \
        )                                                                      \
      )                                                                        \
    )                                                                          \
  ));

Constructor *Constructor_Create(Constructor *const super, Method *const method);
Constructor *Constructor_CopyOf(Constructor *const other);
void Constructor_Delete(Constructor *const inst);
boolean Constructor_Equals(Constructor *const obj1, Constructor *const obj2);
void Constructor_Inherit(Constructor *const inst, Constructor *const super);
Constructor *Constructor_GetSuper(const Constructor *const inst);
Method *Constructor_GetMethod(const Constructor *const inst);
void Constructor_SetSuper(Constructor *const inst, Constructor *const super);

#endif  /* COMPOUND_CONSTRUCTOR_H */
