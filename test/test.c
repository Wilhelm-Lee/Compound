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

/* Required on some systems to expose POSIX features. */
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../inc/allocator.h"
#include "../inc/body.h"
#include "../inc/class.h"
#include "../inc/constructor.h"
#include "../inc/destructor.h"
#include "../inc/entry.h"
#include "../inc/field.h"
#include "../inc/function.h"
#include "../inc/memory_stack.h"
#include "../inc/origin.h"
#include "../inc/preprocessor.h"
// #include "../inc/recollector.h"
#include "../inc/regex.h"
#include "../inc/stream.h"

#define HEADER  "usr/header.h"
#define SOURCE  "usr/source.c"

/* This header includes everything generated.
 * Before the generation, it is suppose to be empty, making no difference.
 */
#include "../usr/header.h"

int Main(void)
{
  class (public, Variable, {
    field(private, String *, identifier, nll);
    field(private, String *, value, nll);

    constructor (params(param(String *const, identifier), param(String *const, value)), {
      if (!identifier) {
        return nll;
      }

      this->identifier = CopyOf(String, identifier);
      this->value = CopyOf(String, value);

      return this;
    })

    destructor ({
      Delete(String, this->identifier);
      Delete(String, this->value);
    })

    method (public, String *, GetIdentifier, noparam, {
      return this->identifier;
    })

    method (public, String *, GetValue, noparam, {
      return this->value;
    })

    override (Literalise, {
      return append(nll, this->identifer, string(" = "), this->value, string(";"));
    })

    override (Equals, {
      return Equals(String, this->identifer, other->identifier)
          && Equals(String, this->value, other->value);
    })
  })

  Class *const copyof = CopyOf(Class, c_Variable);
  ig copyof;

  // DumpHeap("");
  // DumpHeapOccupations();

  return 0;
}
