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
# include "literalisation.h"

/* Defines access permissions for both ingress and egress
 * the privilege of a class to access others or be accessed by them. */
typedef enum {
  /* Unrestricted: Can access and be accessed by any class. */
  ACCESS_PUBLIC = UINT32_MAX,

  /* Restricted: Can only access and be accessed by classes with the
   * exact same access level.
   *
   * Note: Protected access is not limited strictly to the value 1.
   * Any value greater than ACCESS_PRIVATE and less than ACCESS_PUBLIC
   * acts as a unique protected tier. This provides a broad range of
   * custom, user-defined access levels.
   */
  ACCESS_PROTECTED = 1,

  /* Isolated: Cannot access other classes and cannot be accessed by them. */
  ACCESS_PRIVATE = 0
} Access;

/* @accesser accesses @accessee. */
boolean Access_IsAccessible(const Access accessee, const Access accesser);

#endif  /* COMPOUND_ACCESS_H */
