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

#include "../inc/memory.h"

inline void *Allocate(const size_t nmemb, const size_t size)
{
  void *inst = NULL;

  if (nmemb == 1) {
    inst = malloc(size);
    memset(inst, 0, size);
  } else {
    inst = calloc(nmemb, size);
  }

  if (!inst) {
    return NULL;
  }

  return inst;
}

inline void *Reallocate(void *inst, const size_t size)
{
  if (!inst) {
    return inst;
  };

  inst = realloc(inst, size);

  if (!inst) {
    return inst;
  }

  return inst;
}

inline void Deallocate(void *const inst)
{
  if (inst) {
    free(inst);
  }
}


// void *_Allocate(const size_t nmemb, const size_t size,
//                 const char *restrict const file,
//                 const char *restrict const func,
//                 const int line)
// {
//   void *inst = NULL;

//   if (nmemb == 1) {
//     inst = malloc(size);
//     memset(inst, 0, size);
//   } else {
//     inst = calloc(nmemb, size);
//   }

//   if (!inst) {
//     return NULL;
//   }

//   /* Register the memory onto the heap stack. */
//   Memory mem = (Memory) {
//     .allocation = (Location) {
//       .file = (char *)file,
//       .func = (char *)func,
//       .line = line
//     },
//     .reallocation = EMPTY,
//     .freeing = EMPTY,
//     .data = inst,
//     .size = (nmemb * size),
//     .has_reallocated = false,
//     .has_freed = false
//   };
//   mpush(mem);

//   return inst;
// }

// void *_Reallocate(Memory *const inst, const size_t size,
//                   const char *restrict const file,
//                   const char *restrict const func,
//                   const int line)
// {
//   if (!inst) state (RuntimeError,
//     "Attempting to reallocate unallocated memory address (inst = NULL)."
//   ) {
//     return inst;
//   };

//   inst = realloc(inst, size);

//   if (!inst) state (InsufficientMemory,
//     "Failed to re-allocate."
//   ) {
//     return inst;
//   }

//   return inst;
// }

// void Deallocate(void *const *inst)
// {
//   if (inst && *inst) {
//     free(*inst);
//   }

//   inst = NULL;
// }
