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

#ifndef COMPOUND_OUTPUT_H
# define COMPOUND_OUTPUT_H

# include <stdio.h>

# include "stack.h"
# include "status.h"

typedef FILE *Output;

// The global definition of the object.
static FILE *GlobalOutput = NULL;
static inline void Output_SetGlobal(Output stream)
{
  GlobalOutput = stream;
};

STACK(Output)

/* Get current output stream. */
# define this_output  ({                                   \
  Output output = NULL;                                    \
  fail(call(Stack, FILE, GetTop) with (&GlobalFILEStack)); \
})

# define output(filename, block)                           \
  local(Output _output_out = fopen((filename), "w+"), ig fclose(out), {\
    state(!_output_out, CannotOpenFile);                   \
    block                                                  \
  })

# define OUTPUT_TUPLE  "out/tuples.c"

#endif  /* COMPOUND_OUTPUT_H */
