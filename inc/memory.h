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

#ifndef COMPOUND_MEMORY_H
# define COMPOUND_MEMORY_H

# include "status.h"
# include "literalisation.h"

//                                       data size signature
# define MEMORY_LITERALISATION_FORMAT  "@%p+%lu<%s>"

# define MEMORY_LITERALISATION_LENGTH_MAXIMUM  64

/* The calloc() function allocates memory for an array of
   nmemb elements of size bytes each and returns a pointer
   to the allocated memory. The memory is set to zero.

   If nmemb or size is 0, then calloc() returns either NULL,
   or a unique pointer value that can later be successfully
   passed to free().

   https://linux.die.net/man/3/calloc

   Pass on null to @stat when trying to ignore the status. */
Status _Allocate(void **inst, const size_t nmemb, const size_t size);

# define Allocate(inst, nmemb, size)                       \
  _Allocate((void **)(inst), (nmemb), (size))

/* Uses @realloc. */
Status _Reallocate(void **inst, const size_t size);

# define Reallocate(inst, nmemb, size)                     \
  _Reallocate((void **)(inst), (size))

/* Uses @free. */
void Deallocate(void *inst);

/*
  int
  char[]
  void *
*/
// # define new(type)                                         \
//   ({                                                       \
//     type *_new_object = NULL;                              \
//     int _new_array_level = 0;                              \
//     parse(nameof(type), {                                  \
//                                                            \
//     })
//   })

// # define delete(obj)

# define FUNC_MEMORY(type)                                                     \
  Status type##Memory_Create(type##Memory *inst, const char *signature);       \
  Status type##Memory_CopyOf(type##Memory *inst, const type##Memory other);    \
    void type##Memory_Delete(type##Memory *inst);                              \
    boolean type##Memory_Equals(const type##Memory mem1, const type##Memory mem2);\
  size_t type##Memory_Literalise(char *buff, const void *object, const char *format,\
                                 int countdown, LiteralisationCallback *callback);

# define DEFINE_MEMORY(type)                               \
  struct _##type##Memory {                                 \
    type *data;                                            \
    size_t size;                                           \
    char *signature;                                       \
  }

# define DEFINE_MEMORY_T(name, type)                       \
  struct _##name##Memory {                                 \
    type *data;                                            \
    size_t size;                                           \
    char *signature;                                       \
  }

# define TYPEDEF_MEMORY(type)                              \
  typedef DEFINE_MEMORY(type) type##Memory;

# define TYPEDEF_MEMORY_T(name, type)                      \
  typedef DEFINE_MEMORY_T(name, type) name##Memory;

# define GLOBAL_MEMORY(type)                               \
  static type##Memory *Global##type##Memory = NULL;        \
  static inline void type##Memory_SetGlobal(type##Memory *arr)\
  {                                                        \
    Global##type##Memory = arr;                            \
  }

# define LITERALISATION_MEMORY(type)                                \
  static LiteralisationCallback type##MemoryLiteralisationCallback = {\
    .offset = 0,                                           \
    .Literalise = type##Memory_Literalise,                 \
    .format = MEMORY_LITERALISATION_FORMAT                 \
  };

# define MEMORY(type)                                      \
  TYPEDEF_MEMORY(type)                                     \
  FUNC_MEMORY(type)                                        \
  GLOBAL_MEMORY(type)                                      \
  LITERALISATION_MEMORY(type)

/* Based on @MEMORY, but use provided @type to define members in struct. */
# define MEMORY_T(name, type)                              \
  TYPEDEF_MEMORY_T(name, type)                             \
  FUNC_MEMORY(name)                                        \
  GLOBAL_MEMORY(name)

# define Memory(type)                                      \
  type##Memory

# define IMPL_MEMORY(type)                                 \
Status Allocate(type##Memory *inst, const size_t nmemb)    \
{\
  state(!size, InvalidSize);\
\
  inst->data = calloc(nmemb, size);\
\
  if (!inst->data) {\
    RETURN(InsufficientMemory);\
  }\
  inst->size = size;\
\
  RETURN(NormalStatus);\
}\
\
Status Reallocate(type##MemoryMemory *inst, const size_t size)\
{\
  state(!size, InvalidSize);\
\
  if (!(inst->data = realloc(&inst->data, size))) {\
    RETURN(InsufficientMemory);\
  }\
  inst->size = size;\
\
  RETURN(NormalStatus);\
}\
\
void Deallocate(Memory *inst)\
{\
  if (inst->data) {\
    free(inst->data);\
  }\
  inst->data = NULL;\
  inst->size = 0;\
}\
\
Status Memory_Create(Memory *inst, const size_t size)\
{\
  fail(Allocate(inst, 0, size));\
\
  RETURN(NormalStatus);\
}\
\
Status Memory_CopyOf(Memory *inst, const Memory other)\
{\
  fail(Allocate(inst, 0, other.size));\
\
  RETURN(NormalStatus);\
}\
\
void Memory_Delete(Memory *inst)\
{\
  Deallocate(inst);\
\
  RETURN(NormalStatus);\
}\
\
size_t Memory_Literalise(char *buff, const void *object, const char *format,\
                         int countdown, LiteralisationCallback *callback)\
{\
  /* @(!format[0]) indicates whether @format is empty. */\
  if (!buff || !object || !format || !format[0] || countdown <= 0) {\
    return 0;\
  }\
\
  const Memory *inst = object;\
\
  return snprintf(buff, MEMORY_LITERALISATION_LENGTH_MAXIMUM + strlen(inst->signature),\
                  (format ? format : callback->format), inst->data, inst->size, inst->signature);\
}

#endif  /* COMPOUND_MEMORY_H */
