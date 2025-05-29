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

#include <Compound/stacks.h>

IMPL_STACK(FILE,,);
IMPL_STACK(boolean,,);
IMPL_STACK(float,,);
IMPL_STACK(double,,);
IMPL_STACK(char,,);
IMPL_STACK(short,,);
IMPL_STACK(int,,);
IMPL_STACK(long,,);
IMPL_STACK(llong,,);
IMPL_STACK(int8_t,,);
IMPL_STACK(int16_t,,);
IMPL_STACK(int32_t,,);
IMPL_STACK(int64_t,,);
IMPL_STACK(int_fast8_t,,);
IMPL_STACK(int_fast16_t,,);
IMPL_STACK(int_fast32_t,,);
IMPL_STACK(int_fast64_t,,);
IMPL_STACK(intptr_t,,);
IMPL_STACK(intmax_t,,);
IMPL_STACK(uchar,,);
IMPL_STACK(ushort,,);
IMPL_STACK(uint,,);
IMPL_STACK(ulong,,);
IMPL_STACK(ullong,,);
IMPL_STACK(uint8_t,,);
IMPL_STACK(uint16_t,,);
IMPL_STACK(uint32_t,,);
IMPL_STACK(uint64_t,,);
IMPL_STACK(uint_fast8_t,,);
IMPL_STACK(uint_fast16_t,,);
IMPL_STACK(uint_fast32_t,,);
IMPL_STACK(uint_fast64_t,,);
IMPL_STACK(uintptr_t,,);
IMPL_STACK(uintmax_t,,);

IMPL_STACK(Status,, {
  handle(*(const Status *)inst, {
    nbelong(NormalStatus, {
      PrintStatus(_);
    })
  })
});
