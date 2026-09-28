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

/** @file access.c */

#include "../inc/access.h"

/* READ WRITE EXECUTE */
/*  7     7     7     */

#define ACCESS_PERMISSION_MAXIMUM  (0777)
#define ACCESS_PERMISSION_MINIMUM  (0)

struct Access {
  AccessPermission permission;
  AccessVisibility visibility;
};

// static inline byte GetPermissionRead(const AccessPermission permission)
// {
//   return permission & 0700;
// }

// static inline byte GetPermissionWrite(const AccessPermission permission)
// {
//   return permission & 0070;
// }

// static inline byte GetPermissionExecute(const AccessPermission permission)
// {
//   /* Double-O Seven?!  No wonder it's called "Execute". */
//   return permission & 0007;
// }

static boolean IsValidPermission(const AccessPermission permission)
{
  return permission > ACCESS_VISIBILITY_MINIMUM
      && permission <= ACCESS_PERMISSION_MAXIMUM;
}

/* Visibility is a signed integer. */
static boolean IsValidVisibility(const AccessVisibility visibility)
{
  return visibility > ACCESS_VISIBILITY_MINIMUM
      && visibility <= ACCESS_VISIBILITY_MAXIMUM;
}

Access *Access_Create(
  const AccessPermission permission,
  const AccessVisibility visibility
) {
  if (!IsValidPermission(permission) || !IsValidVisibility(visibility)) {
    return nll;
  }

  Access *const inst = Allocate(sizeof(Access));
  if (!inst) {
    return nll;
  }

  inst->permission = permission;
  inst->visibility = visibility;

  return inst;
}

Access *Access_CopyOf(Access *const other)
{
  if (!other) {
    return nll;
  }

  return Create(Access, other->permission, other->visibility);
}

void Access_Delete(Access *const inst)
{
  if (!inst) {
    return;
  }

  Deallocate(inst);
}

boolean Access_Equals(Access *const inst, Access *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  return inst->permission == other->permission
      && inst->visibility == other->visibility;
}

boolean Access_IsAccessible(
  Access *const accessee,
  Access *const accesser
) {
  if (!accessee || !accesser) {
    return false;
  }

  if (!IsValidVisibility(accessee->visibility)
   || !IsValidVisibility(accessee->visibility)) {
    return false;
  }

  if (accessee->visibility == ACCESS_VISIBILITY_PRIVATE) {
    return false;
  }

  if (accessee->visibility == ACCESS_VISIBILITY_PUBLIC) {
    return true;
  }

  /* Protected. */
  return accessee->visibility == accesser->visibility;
}

String *Access_Literalise(Access *const inst)
{
  if (!inst) {
    return nll;
  }

  if (!IsValidVisibility(inst->visibility)) {
    return string("(invalid AccessVisibility)");
  }

  switch (inst->visibility) {
  case ACCESS_VISIBILITY_PUBLIC:
    return string("public");
    break;
  case ACCESS_VISIBILITY_PRIVATE:
    return string("private");
    break;
  default:
    return string("protected");
    break;
  }
}
