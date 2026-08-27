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

/** @file platform.h */

#ifndef COMPOUND_PLATFORM_H
# define COMPOUND_PLATFORM_H

# define __COMPOUND__  1

# if defined __x86_64__ || defined __x86_64 || __WORDSIZE == 64
#  define __COMPOUND_64__
#  define __COMPOUND_PRODUCT__  "compound64"
# else
#  define __COMPOUND_32__
#  define __COMPOUND_PRODUCT__  "compound32"
# endif

# define CR  "\r"
# define LF  "\n"
# define CRLF  CR LF

# if defined(__APPLE__) && defined(__MACH__)
#  define __COMPOUND_ON_APPLE__
# elif defined(_WIN32) || defined(_WIN64)
#  define __COMPOUND_ON_WINDOWS__
# else
/* Declared separately is purely for *your* personal preference. */
#  define __COMPOUND_ON_UNIX__
#  define __COMPOUND_ON_LINUX__

/* For unifications: */
#  define __COMPOUND_ON_UNIX_OR_LINUX__\
       (__COMPOUND_ON_UNIX__ || __COMPOUND_ON_LINUX__)
# endif

# ifdef __COMPOUND_ON_APPLE__
#  define NEWLINE  CR
# elif defined __COMPOUND_ON_WINDOWS__
#  define NEWLINE  CRLF
# else
#  define NEWLINE  LF
# endif

# define NL  NEWLINE

#endif  /* COMPOUND_PLATFORM_H */
