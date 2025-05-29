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

# include "access_qualifier.h"
# include "array.h"
# include "lifespan_qualifier.h"
# include "signature.h"
# include "status.h"

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
  AccessQualifier perm;     // Public, Protected, Package or Private.
  LifespanQualifier lfspn;  // Static or Dynamic.
  boolean virtual;          // Virtual method always return @NOCONTENT.
  String identity;
  String content;           // Method body.
  String signature;         // See doc/SIGNATURE.md.
  Array(String) args;       // Arguments passed in.
} ClassMethod;

ARRAY(ClassMethod);

Status ClassMethod_Create(ClassMethod *inst,
                          const AccessQualifier perm,
                          const LifespanQualifier lfspn, const boolean virtual,
                          const String identity, const String content,
                          const String signature, ...);
Status ClassMethod_CopyOf(ClassMethod *inst, const ClassMethod other);
Status ClassMethod_Delete(ClassMethod *inst);
boolean ClassMethod_Equals(const ClassMethod method1, const ClassMethod method2);
Status ClassMethod_Override(ClassMethod *inst, const String content);

STATUS(InvalidMethodIdentity, 1,
       "Method identity cannot be empty.",
       STATUS_ERROR, &InvalidObject);

STATUS(InvalidMethodSignature, 1,
       "Method signature cannot be empty.",
       STATUS_ERROR, &InvalidObject);

#endif  /* COMPOUND_CLASS_METHOD_H */
