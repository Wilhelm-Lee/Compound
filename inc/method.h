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

/** @file method.h */

#ifndef COMPOUND_METHOD_H
# define COMPOUND_METHOD_H

# include "access.h"
# include "function.h"

typedef struct Method Method;

ARRAY(Method)
LITERALISE_ARGS(
  Method,
  boolean need_returning,
  boolean need_identifier,
  boolean need_param_types,
  boolean need_param_identifiers,
  boolean need_parameters,
  boolean need_body,
  boolean need_semicolon
)

# define _create_method(                                                       \
    access_literal,                                                            \
    returning_type_literal,                                                    \
    identifier_literal,                                                        \
    param_clusters,                                                            \
    ...                                                                        \
  )                                                                            \
    Create(                                                                    \
      Method,                                                                  \
      ACCESS_##access_literal,                                                 \
      CLASS_IDENTIFIER_STR,                                                    \
      function(                                                                \
        string(nameof(returning_type_literal)),                                \
        string(nameof(identifier_literal)),                                    \
        call(                                                                  \
          Array(Parameter),                                                    \
          Concat,                                                              \
          params_str(                                                          \
            param_str(                                                         \
              append(                                                          \
                CLASS_IDENTIFIER_STR,                                          \
                string(" *const")                                              \
              ),                                                               \
              string(nameof(this))                                             \
            )                                                                  \
          ),                                                                   \
          param_clusters                                                       \
        ),                                                                     \
        __VA_ARGS__                                                            \
      )                                                                        \
    )

# define method(                                                               \
    access_literal,                                                            \
    returning_type_literal,                                                    \
    identifier_literal,                                                        \
    param_clusters,                                                            \
    ...                                                                        \
  )                                                                            \
  (call(                                                                       \
    Class,                                                                     \
    AddMethod,                                                                 \
    this,                                                                      \
    _create_method(access_literal, returning_type_literal, identifier_literal, param_clusters, __VA_ARGS__)\
  ));

Method *Method_Create(
  const Access access,
  String *const class_identifier,
  Function *const function
);
Method *Method_CopyOf(Method *const other);
void Method_Delete(Method *const inst);
boolean Method_Equals(Method *const obj1, Method *const obj2);
Access Method_GetAccess(const Method *const inst);
Function *Method_GetFunction(const Method *const inst);
String *Method_GetIdentifier(Method *const inst);

#endif  /* COMPOUND_METHOD_H */
