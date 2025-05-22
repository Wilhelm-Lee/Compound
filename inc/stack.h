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

#ifndef COMPOUND_STACK_H
# define COMPOUND_STACK_H

# define Stack(name)                                       \
  name##Stack

# define DEFINE_STACK(name)                                \
  struct name##Stack {                                     \
    name *data;                                            \
    size_t length;                                         \
    size_t offset;  /* The top of the stack. */            \
  }

# define DEFINE_STACK_T(name, type)                        \
  struct name##Stack {                                     \
    type *data;                                            \
    size_t length;                                         \
    size_t offset;  /* The top of the stack. */            \
  }

# define TYPEDEF_STACK(name)                               \
  typedef DEFINE_STACK(name) name##Stack

# define TYPEDEF_STACK_T(name, type)                       \
  typedef DEFINE_STACK_T(name, type) name##Stack

# define FUNC_STACK(name)                                  \
  FUNC_STACK_T(name, name)

# define FUNC_STACK_T(name, type)                          \
  Status name##Stack_Create(name##Stack *inst, const size_t length);\
  Status name##Stack_CopyOf(name##Stack *inst, const name##Stack other);\
  Status name##Stack_Delete(name##Stack *inst);            \
  Status name##Stack_Push(name##Stack *inst, const name elem);\
  Status name##Stack_Pop(name##Stack *inst);               \
  void   name##Stack_PopAll(name##Stack *inst);            \
  inline type *name##Stack_GetTop(name##Stack *inst);

# define GLOBAL_STACK(name)                                \
  static name##Stack *Global##name##Stack = NULL;          \
  static inline void name##Stack_SetGlobal(name##Stack *stk)\
  {                                                        \
    Global##name##Stack = stk;                             \
  }

// # define LITERALISATION_STACK(type)                        \
//   LITERALISATION(type##Stack, STACK_LITERALISATION_FORMAT);

# define STACK_LITERALISATION_FORMAT  "[>%s]"

# define STACK(type)                                       \
  TYPEDEF_STACK(type);                                     \
  FUNC_STACK(type);                                        \
  GLOBAL_STACK(type);

/* Based on @STACK, but use provided @type to define members in struct. */
# define STACK_T(name, type)                               \
  TYPEDEF_STACK_T(name, type);                             \
  FUNC_STACK_T(name);                                      \
  GLOBAL_STACK(name);

# define IMPL_STACK(type, push_action, pop_action)         \
Status type##Stack_Create(type##Stack *inst, const size_t length)\
{\
  avail(inst);\
  state(!length, InvalidStackLength);\
\
  state(!(inst->data = calloc(length, sizeof(type))), InsufficientMemory);\
\
  (void)memset(inst->data, 0, inst->length);\
\
  inst->length = length;\
  inst->offset = 0;\
\
  RETURN(NormalStatus);\
}\
\
Status type##Stack_CopyOf(type##Stack *inst, const type##Stack other)\
{\
  avail(inst);\
  state(!other.length, (InvalidStackLength));\
\
  state(!(inst->data = calloc(other.length, sizeof(type))), InsufficientMemory);\
  inst->length = other.length;\
  inst->offset = 0;\
\
  RETURN(NormalStatus);\
}\
\
Status type##Stack_Delete(type##Stack *inst)\
{\
  avail(inst);\
  state(!inst->data, InstanceNotAlive);\
\
  type##Stack_PopAll(inst);\
\
  free(inst->data);\
  inst->length = 0;\
  inst->offset = 0;\
\
  RETURN(NormalStatus);\
}\
\
Status type##Stack_Push(type##Stack *inst, const type elem)\
{\
  avail(inst);\
  state(isfull(*inst), StackFull);\
\
  inst->offset++;\
\
  inst->data[inst->offset - 1] = elem;\
\
  push_action\
\
  RETURN(NormalStatus);\
}\
\
Status type##Stack_Pop(type##Stack *inst)\
{\
  avail(inst);\
  state(isempty(*inst), StackEmpty);\
\
  inst->offset--;\
\
  pop_action\
\
  RETURN(NormalStatus);\
}\
\
void type##Stack_PopAll(type##Stack *inst)\
{\
  if (!inst || !inst->length)  return;\
\
  inst->offset = 0;\
}\
inline type *type##Stack_GetTop(type##Stack *inst)\
{\
  if (!inst || !inst->length)  return NULL;\
\
  return &inst->data[inst->offset];\
}

# define isfull(stk)  ((stk).offset == (stk).length)
# define isempty(stk)  ((stk).offset == 0)

#endif  /* COMPOUND_STACK_H */
