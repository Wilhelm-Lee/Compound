/*
 * This file is part of Compound library.
 * Copyright (C) 2024-TODAY  William Lee
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

#ifndef COMPOUND_CLASS_METHOD_H
# define COMPOUND_CLASS_METHOD_H

# include <stdarg.h>

# include "../inc/access_permission_qualifier.h"
# include "../inc/lifespan_qualifier.h"
# include "../inc/status.h"
# include "../inc/signature.h"

//  rtn id signature
# define CLASS_METHOD_DECLARATION_LITERALISE_FORMAT        \
    "%s %s(%s);"

//  rtn id signa content
# define CLASS_METHOD_DEFINITION_LITERALISE_FORMAT         \
    "%s %s(%s) { %s }"

# define CLASS_METHOD_SIGNATURE_LENGTH_MAXIMUM  1024
    
// https://edurev.in/question/1999712/
//   How-many-max-number-of-arguments-can-present-in-function-in-c99-compiler-
//   a-99b-90c-102d-127Correct-a
# define CLASS_METHOD_ARGUMENTS_LENGTH_MAXIMUM  127

typedef struct {
  AccessPermissionQualifier perm;  // Public, Protected, Package or Private.
  LifespanQualifier lfspn;         // Static or Dynamic.
  boolean virtual;                    // Virtual method should not have @content.
  boolean override;                   // The one that this method overrides.
  char *identity;                  // Method identity.
  char *content;                   // Only when non-virtual to apply.
  char *signature;                 // See doc/SIGNATURE.md for more.
  int argc;                        // Count for args.  ranged: [0, 127]
  char **args;                     // The identifiers of args.
} ClassMethod;

Status ClassMethod_Create(ClassMethod *inst,
                          const AccessPermissionQualifier perm,
                          const LifespanQualifier lfspn, const boolean virtual,
                          const boolean override, const char *identity,
                          const char *content, const char *signature, ...);
Status ClassMethod_CopyOf(ClassMethod *inst, const ClassMethod other);
Status ClassMethod_Delete(ClassMethod *inst);
boolean   ClassMethod_Equals(const ClassMethod method1, const ClassMethod method2);
Status ClassMethod_Override(ClassMethod *inst, const char *content);
size_t ClassMethod_Export(const ClassMethod method, FILE *dst);
size_t ClassMethod_Literalise(const ClassMethod method, char *buff);

STATUS(InvalidClassMethodIdentity, 1,
  "Class method identity cannot be null or empty.",
  STATUS_ERROR, &InvalidObject);

STATUS(InvalidClassMethodContent, 1,
  "Class method content cannot be null.",
  STATUS_ERROR, &InvalidObject);

STATUS(InvalidClassMethodSignature, 1,
  "Class method signature cannot be null.",
  STATUS_ERROR, &InvalidObject);

STATUS(InvalidClassMethodParameterItemName, 1,
  "Class method parameter item name cannot be null.",
  STATUS_ERROR, &InvalidObject);

STATUS(VirtualClassMethodCannotHaveContent, 1,
  "Any virtual class method cannot have content.",
  STATUS_ERROR, &ErrorStatus);

#endif  /* COMPOUND_CLASS_METHOD_H */
