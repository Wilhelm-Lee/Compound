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

#ifndef COMPOUND_CLASS_CONSTRUCTOR_H
# define COMPOUND_CLASS_CONSTRUCTOR_H

# include "Compound/access_permission_qualifier.h"
# include "Compound/lifespan_qualifier.h"
# include "Compound/status.h"

# define CLASS_CONSTRUCTOR_IDENTITY_LENGTH_MAXIMUM  64

typedef struct {
  AccessPermissionQualifier perm;
  LifespanQualifier lfspn;
  char identity[CLASS_CONSTRUCTOR_IDENTITY_LENGTH_MAXIMUM];
} ClassConstructor;

Status ClassConstructor_Create(ClassConstructor *inst,
                               const AccessPermissionQualifier perm,
                               const char *content, const char *signature, ...);
Status ClassConstructor_CopyOf(ClassConstructor *inst,
                               const ClassConstructor other);
Status ClassConstructor_Delete(ClassConstructor *inst);
boolean   ClassConstructor_Equals(const ClassConstructor constr1,
                               const ClassConstructor constr2);

/* Based on given @identity, writes processed string into @buff. */
Status ClassConstructor_GenerateIdentity(char *buff, const char *signature);

#endif  /* COMPOUND_CLASS_CONSTRUCTOR_H */
