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

#include "Compound/arrays.h"
#include "Compound/common.h"
#include "Compound/memory.h"
#include "Compound/platform.h"
#include "Compound/stacks.h"
#include "Compound/status.h"
#include "Compound/string.h"
#include "Compound/types.h"

Status Main(void)
{
  String message = string("Delightful encountering, Compound!");
  iterate (i, message.data) {
    ig putchar(getbyte(message, i));
  }
  ig printf(NEWLINE);
  
  byte *buff = fallback(message);
  ig printf("%s"NEWLINE, buff);
  
  ig Deallocate(buff);
  ig call(String,, Delete) with (&message);
  
  RETURN(NormalStatus);
}

int main(void)
{
  handle(Main(), {
    nbelong(NormalStatus, {
      PrintStatus(_);
    })
    
    return _.value;
  })
}
