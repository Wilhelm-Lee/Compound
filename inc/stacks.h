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

#ifndef COMPOUND_STACKS_H
# define COMPOUND_STACKS_H

# include "Compound/types.h"
# include "Compound/stack.h"
# include "Compound/status.h"

STACK(FILE);
STACK(Pointer);
STACK(boolean);
STACK(float);
STACK(double);

/* Signed. */
STACK(char);
STACK(short);
STACK(int);
STACK(long);
STACK(llong);
STACK(int8_t);
STACK(int16_t);
STACK(int32_t);
STACK(int64_t);
STACK(int_fast8_t);
STACK(int_fast16_t);
STACK(int_fast32_t);
STACK(int_fast64_t);
STACK(intptr_t);
STACK(intmax_t);

/* Unsigned. */
STACK(uchar);
STACK(ushort);
STACK(uint);
STACK(ulong);
STACK(ullong);
STACK(uint8_t);
STACK(uint16_t);
STACK(uint32_t);
STACK(uint64_t);
STACK(uint_fast8_t);
STACK(uint_fast16_t);
STACK(uint_fast32_t);
STACK(uint_fast64_t);
STACK(uintptr_t);
STACK(uintmax_t);

#endif  /* COMPOUND_STACKS_H */
