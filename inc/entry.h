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

/** @file entry.h */

#ifndef COMPOUND_ENTRY_H
# define COMPOUND_ENTRY_H

# include "init.h"

# if defined (__COMPOUND_FEATURE_ARGUMENT__) &&                                \
     defined (__COMPOUND_FEATURE_ENVIRONMENT__)
#  define IMPL_MAIN                                                            \
   int main(                                                                   \
     const int argc,                                                           \
     const char *const *const argv,                                            \
     const char *const *const envp                                             \
   ) {                                                                         \
     Array(String) *args = null;                                               \
     Array(String) *envs = null;                                               \
     InitialiseMain(argc, argv, envp, &args, &envs);                           \
                                                                               \
     const int retval = _Main(args, envs);                                     \
                                                                               \
     DeinitialiseMain(&args, &envs);                                           \
     return retval;                                                            \
   }
# elif defined (__COMPOUND_FEATURE_ARGUMENT__)
#  define IMPL_MAIN                                                            \
   int main(                                                                   \
     const int argc,                                                           \
     const char *const *const argv                                             \
   ) {                                                                         \
     Array(String) *args = null;                                               \
     InitialiseMain(argc, argv, null, &args, null);                            \
                                                                               \
     const int retval = _Main(args);                                           \
                                                                               \
     DeinitialiseMain(&args, null);                                            \
     return retval;                                                            \
   }
# else
#  define IMPL_MAIN                                                            \
   int main(void)                                                              \
   {                                                                           \
     InitialiseMain(0, null, null, null, null);                                \
                                                                               \
     const int retval = _Main();                                               \
                                                                               \
     DeinitialiseMain(null, null);                                             \
     return retval;                                                            \
   }
# endif

# ifdef __COMPOUND_FEATURE_CLASS__
static const boolean _UseContext = true;
# else
static const boolean _UseContext = false;
# endif

/* Prevent undefined error -- user doesn't necessarily write @Context. */
# if defined (__COMPOUND_FEATURE_ARGUMENT__) &&                                \
      defined (__COMPOUND_FEATURE_ENVIRONMENT__)
static int _Context(Array(String) *const args, Array(String) *const envs)
{
  ig args, ig envs;
  return 0;
}
# elif defined (__COMPOUND_FEATURE_ARGUMENT__)
static int _Context(Array(String) *const args)
{
  ig args;
  return 0;
}
# else
static int _Context(void)
{
  return 0;
}
# endif

/* @Main is the mask of @_Main. */
/* @_Main is the actual entrance of Compound. */
/* @main is the entrance of C. */
# if defined (__COMPOUND_FEATURE_ARGUMENT__) &&                                \
     defined (__COMPOUND_FEATURE_ENVIRONMENT__)
#  define Main(_args, _envs)                                                   \
   _MainUser(_args, _envs);                                                    \
   int _Main(_args, _envs);                                                    \
   extern int Context(_args, _envs);                                           \
   IMPL_MAIN                                                                   \
   int _Main(_args, _envs)                                                     \
   {                                                                           \
     const int returncode = _UseContext ? Context(args, envs)                  \
                                        : _Context(args, envs);                \
     if (returncode != EXIT_SUCCESS) {                                         \
       return returncode;                                                      \
     }                                                                         \
                                                                               \
     return _MainUser(args, envs);                                             \
   }                                                                           \
   int _MainUser(_args, _envs)
# elif defined (__COMPOUND_FEATURE_ARGUMENT__)
#  define Main(_args)                                                          \
   _MainUser(_args);                                                           \
   int _Main(_args);                                                           \
   extern int Context(_args);                                                  \
   IMPL_MAIN                                                                   \
   int _Main(_args)                                                            \
   {                                                                           \
     const int returncode = _UseContext ? Context(args) : _Context(args);      \
     if (returncode != EXIT_SUCCESS) {                                         \
       return returncode;                                                      \
     }                                                                         \
                                                                               \
     return _MainUser(args);                                                   \
   }                                                                           \
   int _MainUser(_args)
# else
/* Synchronise the "void" parameter to ISO-C99 requirement. */
#  define Main(placeholder_void)                                               \
   _MainUser(placeholder_void);                                                \
   int _Main(placeholder_void);                                                \
   extern int Context(placeholder_void);                                       \
   IMPL_MAIN                                                                   \
   int _Main(placeholder_void)                                                 \
   {                                                                           \
     const int returncode = _UseContext ? Context() : _Context();              \
     if (returncode != EXIT_SUCCESS) {                                         \
       return returncode;                                                      \
     }                                                                         \
                                                                               \
     return _MainUser();                                                       \
   }                                                                           \
   int _MainUser(placeholder_void)
# endif

# undef Context

#endif  /* COMPOUND_ENTRY_H */
