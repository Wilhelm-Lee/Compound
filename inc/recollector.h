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

#ifndef COMPOUND_RECOLLECTOR_H
# define COMPOUND_RECOLLECTOR_H

# include "arrays.h"
# include "boolean.h"
# include "status.h"

typedef struct {
  void *obj;
  boolean inuse;
} Mark;

ARRAY(Mark)

typedef struct {
  Array(Mark) marks;
  
} Recollector;



#endif  /* COMPOUND_RECOLLECTOR_H */
