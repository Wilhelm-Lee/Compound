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

/** @file language.h */

#ifndef COMPOUND_LANGUAGE_H
# define COMPOUND_LANGUAGE_H

# include <stdlib.h>

# include "common.h"
# include "platform.h"

/* PROFILES. */
# ifdef __COMPOUND_PROFILE_EVERYTHING__
#  define __COMPOUND_FEATURE_ALL__
#  define __COMPOUND_KEYWORD_ALL__
# endif

# ifdef __COMPOUND_PROFILE_STANDALONE__
#  define __COMPOUND_FEATURE_BOOLEAN__
#  define __COMPOUND_FEATURE_HEAP__
#  define __COMPOUND_FEATURE_STATUS__
#  define __COMPOUND_KEYWORD_ALL__
# endif

# ifdef __COMPOUND_PROFILE_NATIVE__
#  define __COMPOUND_FEATURE_STATUS__
#  define __COMPOUND_KEYWORD_ALL__
# endif

# ifdef __COMPOUND_PROFILE_PERFORMANCE__
#  define __COMPOUND_FEATURE_BOOLEAN__
#  define __COMPOUND_FEATURE_HEAP__
#  define __COMPOUND_KEYWORD_ALL__
# endif

/* FEATURES & KEYWORDS. */
# ifdef __COMPOUND_FEATURE_ALL__
#  define __COMPOUND_FEATURE_ARGUMENT__
#  define __COMPOUND_FEATURE_BACKTRACING__
#  define __COMPOUND_FEATURE_BOOLEAN__
#  define __COMPOUND_FEATURE_ENVIRONMENT__
#  define __COMPOUND_FEATURE_HEAP__
#  define __COMPOUND_FEATURE_MEMORY__
#  define __COMPOUND_FEATURE_STATUS__
# endif

# ifdef __COMPOUND_KEYWORD_ALL__
#  define __COMPOUND_KEYWORD_AND_OR_NOT__
#  define __COMPOUND_KEYWORD_CONST__
#  define __COMPOUND_KEYWORD_INLINE__
#  define __COMPOUND_KEYWORD_REGISTER__
#  define __COMPOUND_KEYWORD_RESTRICT__
# endif

# define __COMPOUND_FEATURE__(FEATURE)\
  __COMPOUND_FEATURE_##FEATURE##__

# define __COMPOUND_KEYWORD__(KEYWORD)\
  __COMPOUND_KEYWORD_##KEYWORD##__

/* Singular selection. */
static const char *restrict const ENABLED_PROFILE =
# if defined(__COMPOUND_PROFILE_EVERYTHING__)
  "EVERYTHING"
# elif defined(__COMPOUND_PROFILE_STANDALONE__)
  "STANDALONE"
# elif defined(__COMPOUND_PROFILE_NATIVE__)
  "NATIVE"
# elif defined(__COMPOUND_PROFILE_PERFORMANCE__)
  "PERFORMANCE"
# else
  NULL
# endif
;

/* Multiple selections. */
static const char *restrict const ENABLED_FEATURES[] = {
# if defined(__COMPOUND_FEATURE_ARGUMENT__)
  "ARGUMENT",
# endif
# if defined(__COMPOUND_FEATURE_BACKTRACING__)
  "BACKTRACING",
# endif
# if defined(__COMPOUND_FEATURE_BOOLEAN__)
  "BOOLEAN",
# endif
# if defined(__COMPOUND_FEATURE_ENVIRONMENT__)
  "ENVIRONMENT",
# endif
# if defined(__COMPOUND_FEATURE_HEAP__)
  "HEAP",
# endif
# if defined(__COMPOUND_FEATURE_MEMORY__)
  "MEMORY",
# endif
# if defined(__COMPOUND_FEATURE_STATUS__)
  "STATUS",
# endif
  NULL
};

/* Multiple selections. */
static const char *restrict const ENABLED_KEYWORDS[] = {
# if defined(__COMPOUND_KEYWORD_AND_OR_NOT__)
  "AND_OR_NOT",
# endif
# if defined(__COMPOUND_KEYWORD_CONST__)
  "CONST",
# endif
# if defined(__COMPOUND_KEYWORD_INLINE__)
  "INLINE",
# endif
# if defined(__COMPOUND_KEYWORD_REGISTER__)
  "REGISTER",
# endif
# if defined(__COMPOUND_KEYWORD_RESTRICT__)
  "RESTRICT",
# endif
  NULL
};

/* goto is not allowed to prevent conflicting with Status. */
/* goto is an excellent solution for handling errors in C
   natively, by intruding control flows.

   That is, when there is the availability to avoid using
   goto with another handling of errors, goto is no longer
   valid. */
# define goto  @"GOTO IS NOT ALLOWED IN COMPOUND; CONSIDER STATUS INSTEAD."

/* Avoid implications of defining macro __COMPOUND_FEATURE_ARGUMENT__ in ifdef. */
# if defined(__COMPOUND_FEATURE_ENVIRONMENT__) &&\
     !defined(__COMPOUND_FEATURE_ARGUMENT__)
#  define __COMPOUND_FEATURE_ARGUMENT__
# endif

# ifndef __COMPOUND_KEYWORD_REGISTER__
#  define register
# endif

# ifndef __COMPOUND_KEYWORD_CONST__
#  define const
# endif

# ifndef __COMPOUND_KEYWORD_INLINE__
#  define inline
# endif

# ifdef __COMPOUND_KEYWORD_AND_OR_NOT__
#  define and  &&
#  define or   ||
#  define not  !
# endif

#endif  /* COMPOUND_LANGUAGE_H */
