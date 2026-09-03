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

# if defined (__COMPOUND_FEATURE_ARGUMENT__) &&\
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

/* @Main is the mask of @_Main. */
/* @_Main is the actual entrance of Compound. */
/* @main is the entrance of C. */
# if defined (__COMPOUND_FEATURE_ARGUMENT__) &&\
     defined (__COMPOUND_FEATURE_ENVIRONMENT__)
#  define Main(args, envs)\
   _Main(args, envs);\
   IMPL_MAIN\
   int _Main(args, envs)
# elif defined (__COMPOUND_FEATURE_ARGUMENT__)
#  define Main(args)\
   _Main(args);\
   IMPL_MAIN\
   int _Main(args)
# else
#  define Main(...)\
   _Main(void);\
   IMPL_MAIN\
   int _Main(void)
# endif

#endif  /* COMPOUND_ENTRY_H */
