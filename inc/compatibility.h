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

/** @file compatibility.h */

#ifndef COMPOUND_COMPATIBILITY_H
# define COMPOUND_COMPATIBILITY_H

/* ISSUE:  Y2038 */
# if !defined(__COMPOUND_OMIT_Y2038__) && (_TIME_BITS < 64) ||\
     (defined(__GLIBC__) && !defined(__USE_TIME_BITS64)) ||\
     (defined(_WIN32) && defined(_USE_32BIT_TIME_T) && (_USE_32BIT_TIME_T != 0))
#  error "Compound Compatibility:  You are compiling with non-64 bits configuration."
#  error "Compound Compatibility:  Your binary is vulnerable to the Y2038 problem."
#  error "Compound Compatibility:  Compile with -D_FILE_OFFSET_BITS=64 and -D_TIME_BITS=64"
#  error "Compound Compatibility:  or compile with _D__COMPOUND_OMIT_Y2038__ to disable error."
# endif

/* Omitting any of the issues above without solving them
 * causes the untrusting through out the framework.
 *
 * This "UNSTABLE FOUNDATION" mark is used mostly by users
 * who seek security over portability for their works.
 */
# if defined(__COMPOUND_OMIT_Y2038__)
#  define __COMPOUND_UNSTABLE_FOUNDATION__
# endif

#endif  /* COMPOUND_COMPATIBILITY_H */
