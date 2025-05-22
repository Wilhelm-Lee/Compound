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

#ifndef COMPOUND_PLATFORM_H
# define COMPOUND_PLATFORM_H

/* Environmental Constants. */
# define __COMPOUND_STACK_MAXIMUM__  4096L  // 4096 * 64 B = 262,144 B
//                                                         = 256 KB

/* Architecture. */
# if defined __x86_64__ || defined __x86_64
#  define __COMPOUND_64__
#  define __COMPOUND_PRODUCT__  "compound64"
#  define __COMPOUND_CLASS_MAXIMUM__  UINT64_MAX
# elif defined __i386__ || __i486__ || __i586__ || __i686__ || _X86_ || __X86__
#  define __COMPOUND_32__
#  define __COMPOUND_PRODUCT__  "compound32"
#  define __COMPOUND_CLASS_MAXIMUM__  UINT32_MAX
# else
#  error Architecture not yet supported.  Please put up relevant proposals \
if necessary.
# endif

/* The byte width. */
# define __WIDTH__  sizeof(void *)

/* Platform-specific control characters. */
# define LF  "\n"
# define CR  "\r"
# define CRLF  "\r\n"

# ifdef __APPLE__
#  define NEWLINE  CR
# elif defined __linux__
#  define NEWLINE  LF
# else
#  define NEWLINE  CRLF
# endif

# ifdef __APPLE__
#  define __COMPOUND_ON_APPLE__
# elif defined __linux__ || defined __unix__
#  define __COMPOUND_ON_LINUX__
# elif defined _WIN32 || defined _WIN64 || defined __MINGW32__ || defined __MINGW64__
#  define __COMPOUND_ON_WINDOWS__
# endif

# define CLASS_IDENTITY_LENGTH_MAXIMUM  64

#endif  /* COMPOUND_PLATFORM_H */
