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

/** @file compiler.h */

#ifndef COMPOUND_COMPILER_H
# define COMPOUND_COMPILER_H

/* Compiler specifications. */
# ifdef _MSC_VER
#  define __COMPOUND_BY_MSVC__
/* Detect Clang first since it also defines __GNUC__. */
# elif defined __clang__
#  define __COMPOUND_BY_CLANG__
# elif defined __GNUC__
#  define __COMPOUND_BY_GCC__
# elif defined __TINYC__
#  define __COMPOUND_BY_TCC__
# elif defined __INTEL_COMPILER
#  define __COMPOUND_BY_INTEL__
# elif defined __MINGW32__
#  define __COMPOUND_BY_MINGW__
# else
#  define __COMPOUND_BY_MISCELLANEOUS_COMPILER__
# endif

#endif  /* COMPOUND_COMPILER_H */
