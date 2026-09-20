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

/** @file allocator.h */

#ifndef COMPOUND_ALLOCATOR_H
# define COMPOUND_ALLOCATOR_H

# include <stdlib.h>

# include "common.h"
# include "types.h"

/*  *** Hint ********************************  */
/*   Users call "Allocate" and "Deallocate".   */
/*  ******************************** Hint ***  */

# ifdef __COMPOUND_FEATURE_HEAP__
#  define Allocate(requirement)  _Allocate(requirement, 1)
#  ifdef __COMPOUND_FEATURE_RECYCLER__
#   define Deallocate(address_on_heap)
#  else
#   define Deallocate(address_on_heap)  _Deallocate(address_on_heap)
#  endif
# else
#  define Allocate(requirement)        Origin_Allocate(requirement)
#  define Deallocate(address_on_heap)  Origin_Deallocate(address_on_heap)
# endif

/* Process' data-area simulated heap. */
extern void *Origin_Allocate(const llong requirement);
extern void Origin_Deallocate(void *address_on_heap);

/* Operating System provided heap. */
void *_Allocate(const size_t nmemb, const size_t size);
void _Deallocate(void *const ptr);

#endif  /* COMPOUND_ALLOCATOR_H */
