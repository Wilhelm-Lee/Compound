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

/** @file memory_internal.h */

#ifndef COMPOUND_MEMORY_INTERNAL_H
# define COMPOUND_MEMORY_INTERNAL_H

# include "common.h"
// # include "frame.h"
# include "types.h"

# define MEMORY_FRAME_ID_NULL  0
# define MEMORY_FRAME_ID_MAXIMUM  INT32_MAX

typedef struct Header {
  /* The actual pointer towards defragmented address in heap. */
  void *actual;
  void *user;
  llong size;
} Header;

typedef struct Memory {
  Header header;
  llong index_on_meta;
# ifdef __COMPOUND_FEATURE_RECOLLECTOR__
  /* @pinned declares what is kept after the clean-up from current @frame. */
  boolean pinned : 1;
  /* @frame declares what is allocated within a function. */
  uint32_t frame_id : 31;
# endif
} Memory;

#endif  /* COMPOUND_MEMORY_INTERNAL_H */
