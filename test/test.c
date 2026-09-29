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
#include "../inc/enums.h"
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

int Context(Array(String) *const args)
{
  ig args;

  class (public, Variable, {
    class (public, Inner, {
      field (private, int, value, 0);

      constructor (public, (int value), {
        this->value = value;
      })
    })

    record (public, OperationCode, (int code), {
      enums (NOP, 0);
      enums (ADDITION, 1);
      enums (SUBTRACTION, 2);
      enums (MULTIPLICATION, 3);
      enums (DIVISION, 4);
    });

    field (private, String *, identifier, nll);
    field (private, String *, value, nll);

    constructor (public, (String *const identifier, String *const value), {
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

    method (public, String *, GetIdentifier, (void), {
      return this->identifier;
    })

    method (public, String *, GetValue, (void), {
      return this->value;
    })

    virtual (public, Variable *, Operate, (int operation_code, Variable *const other));

    override (public, boolean, Equals, (Variable *const other), {
      if (!other) {
        return false;
      }

      return Equals(String, this->identifier, other->identifier)
          && Equals(String, this->value, other->value);
    })

    override (public, String *, Literalise, (void), {
      return append(nll, this->identifier, string(" = "), this->value, string(";"));
    })
  })

  class (public, Integer, {
    inherit (Variable);

    constructor (public, (int value), {
      this->identifier = string("Integer");
      this->value = format("%d", value);

      return this;
    })

    constructor (public, (Integer *const other), {
      if (!other) {
        return nll;
      }

      this->identifier = string("Integer");
      this->value = format("%d", other->value);

      return this;
    })

    override (public, Variable *, Operate, (int operation_code, Variable *const other), {
      if (!other) {
        return this;
      }

      if (operation_code == OperationCode.ADDITION) {
        this->value = Concat(String, this->value, other->value);
      }

      return this;
    })

    override (public, boolean, Equals, (Integer *const other), {
      if (!other) {
        return false;
      }

      return Equals(String, this->value, other->value);
    })
  })

  return 0;
}

int Main(Array(String) *const args)
{
  ig args;

  outln(format("Hello, Compound!"NL"The time is %lld.", time(nll)));

  return 0;
}
