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

#include <Compound/common.h>
#include <Compound/memory.h>
#include <Compound/platform.h>
#include <Compound/status.h>
#include <Compound/string.h>
#include <Compound/types.h>

Status Main(void)
{
  String msg1 = string("Apples ");
  String msg2 = string("are sometimes red.");

  int versus = compare(msg1, msg2);
  String merge = concat(msg1, msg2);
  String subseq = substr(msg2, 4, 9);  // Requires deallocation afterwards.
  
  ig printf("%d"NEWLINE, versus);
  
  stringing (ch, merge, {
    ig putchar(ch);
  })
  ig printf(NEWLINE);
  
  stringing (ch, subseq, {
    ig putchar(ch);
  })
  ig printf(NEWLINE);
  
  ig call(String,, Delete) with (&subseq);
  ig call(String,, Delete) with (&msg2);
  ig call(String,, Delete) with (&msg1);
  
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
