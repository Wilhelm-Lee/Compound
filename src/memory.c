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

#include "../inc/memory.h"

Status _Allocate(void **inst, const size_t nmemb, const size_t size)
{
  if (nmemb == 1) {
    *inst = malloc(size);
  } else {
    *inst = calloc(nmemb, size);
  }
 
  state(!*inst, InsufficientMemory);
  
  RETURN(NormalStatus);
}

Status _Reallocate(void **inst, const size_t size)
{
  avail(inst);
  state(!size, InvalidSize);
  
  *inst = realloc(*inst, size);
  
  state(!*inst, InsufficientMemory);
  
  RETURN(NormalStatus);
}

void Deallocate(void *inst)
{
  if (inst) {
    free(inst);
  }
  
  inst = NULL;
}
