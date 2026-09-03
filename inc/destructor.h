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

/** @file destructor.h */

#ifndef COMPOUND_DESTRUCTOR_H
# define COMPOUND_DESTRUCTOR_H

# include "method.h"

typedef struct Destructor Destructor;

ARRAY(Destructor)
LITERALISE_ARGS(
  Destructor,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters,
  boolean need_body,
  boolean need_semicolon
)

# define destructor(                                                           \
    ...                                                                        \
  )                                                                            \
  (call(                                                                       \
    Class,                                                                     \
    SetDestructor,                                                             \
    this,                                                                      \
    Create(                                                                    \
      Destructor,                                                              \
      null,                                                                    \
      Create(                                                                  \
        Method,                                                                \
        ACCESS_PUBLIC,                                                         \
        Create(                                                                \
          Function,                                                            \
          Create(                                                              \
            Signature,                                                         \
            string(nameof(void)),\
            append(CLASS_IDENTIFIER_STR, string("_"), string(nameof(Destructor))),\
            params_str(param_str(append(CLASS_IDENTIFIER_STR, string(" *const")), string(nameof(this))))\
          ),                                                                   \
          body(__VA_ARGS__)                                                    \
        )                                                                      \
      )                                                                        \
    )                                                                          \
  ));

Destructor *Destructor_Create(Destructor *const super, Method *const method);
Destructor *Destructor_CopyOf(Destructor *const other);
void Destructor_Delete(Destructor *const inst);
boolean Destructor_Equals(Destructor *const obj1, Destructor *const obj2);
void Destructor_Inherit(Destructor *const inst, Destructor *const super);
Destructor *Destructor_GetSuper(const Destructor *const inst);
Method *Destructor_GetMethod(const Destructor *const inst);
void Destructor_SetSuper(Destructor *const inst, Destructor *const super);

#endif  /* COMPOUND_DESTRUCTOR_H */
