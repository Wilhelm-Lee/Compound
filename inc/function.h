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

#ifndef COMPOUND_FUNCTION_H
# define COMPOUND_FUNCTION_H

# include "array.h"
# include "signature.h"
# include "status.h"
# include "stack.h"
# include "string.h"
# include "literalisation.h"

typedef struct Function {
  /* Types and parametres. */
  String signature;
  
  /* Function name. */
  String identity;
  
  /* Function body. */
  String content;
} Function;

Status Function_Create(Function *inst, void *funcptr);
Status Function_CopyOf(Function *inst, const Function other);
Status Function_Delete(Function *inst);
boolean   Function_Equals(const Function func1, const Function func2);
/* Returns total bytes written into stream. */
size_t Function_Print(FILE *stream, const Function func);

# define funcdecl(rtn, name, param)                        \
  nameof(rtn name param)

# define function(rtn, name, param, body)                  \
  funcdecl(rtn, name, param) nameof(body)

# define funcptr(rtn, name, ...)                           \
  (rtn)(*name)(__VA_ARGS__)

// LITERALISATION(Function);
// ARRAY(Function);
// STACK(Function);

#endif  /* COMPOUND_FUNCTION_H */
