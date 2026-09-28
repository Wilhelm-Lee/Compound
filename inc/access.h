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

/** @file access.h */

#ifndef COMPOUND_ACCESS_H
# define COMPOUND_ACCESS_H

# include "string.h"
# include "literalise.h"

# define ACCESS_VISIBILITY_MAXIMUM  (INT32_MAX)
# define ACCESS_VISIBILITY_MINIMUM  (0)

typedef uint16_t AccessPermission;
/* Defines access permissions for both ingress and egress
 * the privilege of a class to access others or be accessed by them. */
typedef enum {
  /* Unrestricted: Can access and be accessed by any class. */
  ACCESS_VISIBILITY_PUBLIC = ACCESS_VISIBILITY_MAXIMUM,
  ACCESS_VISIBILITY_public = ACCESS_VISIBILITY_MAXIMUM,

  /* Restricted: Can only access and be accessed by classes with the
   * exact same access level.
   *
   * Note: Protected access is not limited strictly to the value 1.
   * Any value greater than ACCESS_VISIBILITY_PRIVATE and less than ACCESS_VISIBILITY_PUBLIC
   * acts as a unique protected tier. This provides a broad range of
   * custom, user-defined access levels.
   */
  ACCESS_VISIBILITY_PROTECTED = 1,
  ACCESS_VISIBILITY_protected = 1,

  /* Isolated: Cannot access other classes and cannot be accessed by them. */
  ACCESS_VISIBILITY_PRIVATE = ACCESS_VISIBILITY_MINIMUM,
  ACCESS_VISIBILITY_private = ACCESS_VISIBILITY_MINIMUM
} AccessVisibility;

typedef struct Access Access;

ARRAY(Access)
LITERALISE(Access)

Access *Access_Create(
  const AccessPermission permission,
  const AccessVisibility visibility
);
Access *Access_CopyOf(Access *const other);
void Access_Delete(Access *const inst);
boolean Access_Equals(Access *const inst, Access *const other);
/* @accesser accesses @accessee. */
boolean Access_IsAccessible(
  Access *const accessee,
  Access *const accesser
);

#endif  /* COMPOUND_ACCESS_H */
