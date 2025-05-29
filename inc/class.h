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

#ifndef COMPOUND_CLASS_H
# define COMPOUND_CLASS_H

# include "access_qualifier.h"
# include "array.h"
# include "class_constructor.h"
# include "class_field.h"
# include "class_method.h"
# include "common.h"
# include "lifespan_qualifier.h"
# include "literalisation.h"
# include "stack.h"
# include "status.h"

/* Inline current class in discussion (stack top). */
# define this  (call(Stack, Class, GetTop) with (GlobalClassStack))

# define __method(perm, lfspn, virtual, identity, content, signature, ...)     \
  fail(Class_AddMethod(this, ({                                                \
    ClassMethod m_##identity = EMPTY;                                          \
    fail(ClassMethod_Create(&m_##identity, perm, lfspn, virtual,               \
                            nameof(identity), content, signature, __VA_ARGS__))\
    m_##identity;                                                              \
  })))

# define __constructor(perm, content, signature, ...)                          \
  fail(Class_AddConstructor(this, ({                                           \
    ClassConstructor t_##identity = EMPTY;                                     \
    fail(ClassConstructor_Create(&t_##identity, perm, content,                 \
                                 "v:"signature, __VA_ARGS__));                 \
    t_##identity;                                                              \
  })))

/*
Status ClassArray_Create(ClassArray *inst, const size_t length);
*/
# define class(perm, lfspn, identity, content)                                 \
  {                                                                            \
    Class c_##identity = EMPTY;                                                \
    fail(call(Stack, Class, Push) with (GlobalClassStack, c_##identity));      \
    int c_##identity##_inner_count = 0;                                        \
    Array(Class) c_##identity##_inner_classes = EMPTY;                         \
    content                                                                    \
    fail(call(Array, Class, Create)                                            \
         with (&c_##identity##_inner_classes, c_##identity##_inner_count));    \
    fail(Class_Create(&c_##identity, perm, lfspn, nameof(identity)));          \
    fail(call(Array, Class, Insert)                                            \
         with (GlobalClassArray, GlobalClassArray->length, *this));            \
    fail(call(Stack, Class, Pop) with (GlobalClassStack));                     \
  }

# define field(perm, lfspn, signature, identity, value)                        \
  fail(Class_AddField(this, ({                                                 \
    ClassField f_##identity = EMPTY;                                           \
    fail(ClassField_Create(&f_##identity, perm, lfspn,                         \
                           nameof(signature), nameof(identity), value));       \
    f_##identity;                                                              \
  })))

# define method(perm, lfspn, identity, content, signature, ...)                \
  __method(perm, lfspn, false, identity, nameof(content), nameof(signature),   \
           __VA_ARGS__)

/* When a class inherits another class, calling a constructor from it that is
   already defined in that super class, would always perform the one from super
   class, then the content from this constructor.
   
   Similarly, when in Java, we have super() method to explicitly indicate the
   calling sequence which also, must be written on the first line of the content
   from that constructor.
   
   However, in Compound, the approach is implicit.  Meaning the calling to a
   constructor from super class is all done automatically without the need
   to write super(), or things as such. */
# define constructor(perm, content, signature, ...)                            \
  __constructor(perm, nameof(content), nameof(signature), __VA_ARGS__)

# define virtual(perm, lfspn, identity, signature, ...)                        \
  __method(perm, lfspn, true, identity, "",nameof(signature),__VA_ARGS__)

/* A method overrides another is not virtual for sure.

   Therefor, it is never true to have both "virtual" and "override"
   switches on, at the same time. */
# define override(perm, lfspn, identity, content, signature, ...)              \
  __method(perm, lfspn, false, identity, nameof(content), signature,__VA_ARGS__)

# define record(incompleted)                               \
  fail(Class_RecordIncompleted(this, nameof(incompleted)))

# define implement(class)                                  \
  fail(Class_ImplementByName(this, nameof(class)))

# define inherit(class)                                    \
  fail(Class_ExtendByName(this, nameof(class)))

/* Inline the class *object* in class pool. */
# define of(class)                                         \
  ({                                                       \
    Class __of_##class = EMPTY;                            \
    fail(call(Array, Class, GetIdx)                        \
         with(*GlobalClassArray,                           \
              call(Array, Class, Search)                   \
              with(*GlobalClassArray, nameof(class)),      \
                   &__of_##class));                        \
    __of_##class;                                          \
  })

// /*  */
// # define thatof(class, member)                             \
//   ({                                                       \
//     Class __thatof_##class = of(class);                    \
    
//   })

/* Inline the available members of the class. */
# define thatof(class, member)

typedef struct Class {
  String identity;

  Array(ClassField) fields;
  Array(ClassMethod) methods;
  Array(ClassConstructor) constructors;

  struct Class *super;
  struct Class *self;

  AccessQualifier perm;
  LifespanQualifier lfspn;
} Class;

ARRAY(Class);
STACK(Class);

Status Class_Create(Class *inst, const AccessQualifier perm,
                    const LifespanQualifier lfspn, const char *identity);
Status Class_CopyOf(Class *inst, const Class other);
Status Class_Delete(Class *inst);
boolean Class_Equals(const Class class1, const Class class2);

Status Class_Extend(Class *inst, const Class other);
Status Class_Implement(Class *inst, const Class other);
Status Class_AddField(Class *inst, const ClassField field);
Status Class_AddMethod(Class *inst, const ClassMethod method);
Status Class_AddConstructor(Class *inst, const ClassConstructor constructor);

Status Class_CallMethod(Class *inst, const ClassMethod method);
Status Class_CallConstructor(Class *inst, const ClassConstructor constructor);

Status Class_GetField(Class *inst, ClassField *field);
Status Class_GetMethod(Class *inst, ClassMethod *method);
Status Class_GetConstructor(Class *inst, ClassConstructor *constructor);

Status Class_RecordIncompleted(Class *inst, const char *incompleted);

STATUS(InvalidClassIdentity, 1,
          "Given class identity is invalid.",
          STATUS_ERROR, &InvalidObject);

#endif  /* COMPOUND_CLASS_H */
