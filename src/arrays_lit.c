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

/** @file arrays_lit.c */

#include "../inc/arrays_lit.h"

IMPL_ARRAY_BASICTYPE_LITERALISE(boolean, "%d")
IMPL_ARRAY_BASICTYPE_LITERALISE(byte, "%u")
IMPL_ARRAY_BASICTYPE_LITERALISE(char, "%c")
IMPL_ARRAY_BASICTYPE_LITERALISE(double, "%f")
IMPL_ARRAY_BASICTYPE_LITERALISE(float, "%f")
IMPL_ARRAY_BASICTYPE_LITERALISE(int, "%d")
IMPL_ARRAY_BASICTYPE_LITERALISE(int16_t, "%" PRId16)
IMPL_ARRAY_BASICTYPE_LITERALISE(int32_t, "%" PRId32)
IMPL_ARRAY_BASICTYPE_LITERALISE(int64_t, "%" PRId64)
IMPL_ARRAY_BASICTYPE_LITERALISE(int8_t, "%" PRId8)
IMPL_ARRAY_BASICTYPE_LITERALISE(int_fast16_t, "%" PRIdFAST16)
IMPL_ARRAY_BASICTYPE_LITERALISE(int_fast32_t, "%" PRIdFAST32)
IMPL_ARRAY_BASICTYPE_LITERALISE(int_fast64_t, "%" PRIdFAST64)
IMPL_ARRAY_BASICTYPE_LITERALISE(int_fast8_t, "%" PRIdFAST8)
IMPL_ARRAY_BASICTYPE_LITERALISE(intmax_t, "%jd")
IMPL_ARRAY_BASICTYPE_LITERALISE(intptr_t, "%" PRIdPTR)
IMPL_ARRAY_BASICTYPE_LITERALISE(llong, "%lld")
IMPL_ARRAY_BASICTYPE_LITERALISE(long, "%ld")
IMPL_ARRAY_BASICTYPE_LITERALISE(ptr, "%p")
IMPL_ARRAY_BASICTYPE_LITERALISE(short, "%hd")
IMPL_ARRAY_BASICTYPE_LITERALISE(size_t, "%zu")
IMPL_ARRAY_BASICTYPE_LITERALISE(uchar, "%u")
IMPL_ARRAY_BASICTYPE_LITERALISE(uint, "%u")
IMPL_ARRAY_BASICTYPE_LITERALISE(uint16_t, "%" PRIu16)
IMPL_ARRAY_BASICTYPE_LITERALISE(uint32_t, "%" PRIu32)
IMPL_ARRAY_BASICTYPE_LITERALISE(uint64_t, "%" PRIu64)
IMPL_ARRAY_BASICTYPE_LITERALISE(uint8_t, "%" PRIu8)
IMPL_ARRAY_BASICTYPE_LITERALISE(uint_fast16_t, "%" PRIuFAST16)
IMPL_ARRAY_BASICTYPE_LITERALISE(uint_fast32_t, "%" PRIuFAST32)
IMPL_ARRAY_BASICTYPE_LITERALISE(uint_fast64_t, "%" PRIuFAST64)
IMPL_ARRAY_BASICTYPE_LITERALISE(uint_fast8_t, "%" PRIuFAST8)
IMPL_ARRAY_BASICTYPE_LITERALISE(uintmax_t, "%ju")
IMPL_ARRAY_BASICTYPE_LITERALISE(uintptr_t, "%" PRIuPTR)
IMPL_ARRAY_BASICTYPE_LITERALISE(ullong, "%llu")
IMPL_ARRAY_BASICTYPE_LITERALISE(ulong, "%lu")
IMPL_ARRAY_BASICTYPE_LITERALISE(ushort, "%hu")
