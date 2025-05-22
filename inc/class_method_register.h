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

#ifndef COMPOUND_CLASS_METHOD_REGISTER_H
# define COMPOUND_CLASS_METHOD_REGISTER_H

# include "../inc/class_method.h"

typedef struct {
  ClassMethod *data;
  size_t length;
} ClassMethodRegister;

#endif  /* COMPOUND_CLASS_METHOD_REGISTER_H */
