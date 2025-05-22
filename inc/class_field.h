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

#ifndef COMPOUND_CLASS_FIELD_H
# define COMPOUND_CLASS_FIELD_H

# include "../inc/access_permission_qualifier.h"
# include "../inc/lifespan_qualifier.h"
# include "../inc/status.h"

# define CLASS_FIELD_IDENTITY_LENGTH_MAX  64

typedef struct {
  AccessPermissionQualifier perm;
  LifespanQualifier lfspn;
  char *signature;
  char *identity;
  char *value;
} ClassField;

Status ClassField_Create(ClassField *inst, const AccessPermissionQualifier perm,
                         const LifespanQualifier lfspn, const char *signature,
                         const char *identity, const char *value);
Status ClassField_CopyOf(ClassField *inst, const ClassField other);
Status ClassField_Delete(ClassField *inst);
boolean   ClassField_Equals(const ClassField field1, const ClassField field2);

#endif  /* COMPOUND_CLASS_FIELD_H */
