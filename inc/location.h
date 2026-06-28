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

/** @file location.h */

#ifndef COMPOUND_LOCATION_H
# define COMPOUND_LOCATION_H

# include <string.h>

# include "boolean.h"

/* Marks the location of occurrence for Status. */
typedef struct {
  char *file;
  char *func;
  int line;
} Location;

/* The in-line-ment of creating Location instances. */
# define __HERE__\
  ((Location) {\
    .file = __FILE__,\
    .func = (char *)__func__,\
    .line = __LINE__\
  })

/* The in-line-ment of creating global Location instances. */
# define __GLOBAL__\
  ((Location) {\
    .file = __FILE__,\
    .func = "(global)",\
    .line = __LINE__\
  })

# define LOCATION_DESCRIPTOR  "in file %s, line %d, function %s"

boolean Location_Equals(const Location location1, const Location location2);

#endif  /* COMPOUND_LOCATION_H */
