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

#include <Compound/class.h>
#include <Compound/common.h>
#include <Compound/memory.h>
#include <Compound/platform.h>
#include <Compound/signature.h>
#include <Compound/status.h>
#include <Compound/string.h>
#include <Compound/types.h>

Status Main(void)
{
  String source = string("  This is not an   apple.");
  const String delim = string(" aeiou");
  const size_t count = tokens(&source, delim);

  for (register size_t i = 0; i < count; i++) {
    String token = breaks(source, i);
    
    ig putchar('\'');
    stringing (ch, token, {
      ig putchar(ch);
    })
    ig putchar('\'');
    ig printf(NEWLINE);
    
    ig call(String,, Delete) with (&token);
  }

  ig call(String,, Delete) with ((String *)&delim);
  ig call(String,, Delete) with ((String *)&source);
  
  RETURN(NormalStatus);
}

// void f(void)
// {
//   /* Sample To: Array_ComposeContent. */
//   iterate (i, source.breaks) {
//     const size_t current = get(source.breaks, size_t, i);
    
//     if (!i) {
//       ig printf("[%lu", current);
//       continue;
//     }
    
//     if (i == last(source.breaks) - 1) {
//       ig printf(", %lu]", current);
//       continue;
//     }
    
//     ig printf(", %lu", current);
//   }
//   ig printf(NEWLINE);
// }

int main(void)
{
  handle(Main(), {
    nbelong(NormalStatus, {
      PrintStatus(_);
    })
    
    return _.value;
  })
}
