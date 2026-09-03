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

#include <stdio.h>

#include "../inc/body.h"
#include "../inc/class.h"
#include "../inc/constructor.h"
#include "../inc/destructor.h"
#include "../inc/entry.h"
#include "../inc/field.h"
#include "../inc/function.h"
#include "../inc/preprocessor.h"
#include "../inc/regex.h"
#include "../inc/stream.h"

#define HEADER  "user/header.h"
#define SOURCE  "user/source.c"

/* This header includes everything generated.
 * Before the generation, it is suppose to be empty, making no difference.
 */
#include "../user/header.h"

typedef struct Variable Variable;

ARRAY(Variable)
LITERALISE(Variable)

Variable *Variable_Create(String *const appearance, String *const value);
Variable *Variable_CopyOf(Variable *const other);
void Variable_Delete(Variable *const inst);
boolean Variable_Equals(Variable *const obj1, Variable *const obj2);
String *Variable_Literalise(Variable *const inst);

struct Variable {
  String *appearance;
  String *value;
};

Variable *Variable_Create(String *const appearance, String *const value)
{
  if (!appearance) {
    return nll;
  }

  Variable *const inst = Allocate(1, sizeof(Variable));
  if (!inst) {
    return nll;
  }

  inst->appearance = appearance;
  inst->value = value;

  return inst;
}

Variable *Variable_CopyOf(Variable *const other)
{
  if (!other) {
    return nll;
  }

  return Create(Variable, CopyOf(String, other->appearance), CopyOf(String, other->value));
}

void Variable_Delete(Variable *const inst)
{
  if (!inst) {
    return;
  }

  Delete(String, inst->value);
  Delete(String, inst->appearance);
  Deallocate(inst);
}

boolean Variable_Equals(Variable *const obj1, Variable *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return Equals(String, obj1->appearance, obj2->appearance)
      && Equals(String, obj1->value, obj2->value);
}

String *Variable_Literalise(Variable *const inst)
{
  if (!inst) {
    return nll;
  }

  return append(inst->appearance, string("  -> "), inst->value);
}

IMPL_ARRAY(Variable)
IMPL_ARRAY_LITERALISE(Variable)

String *ParseBetween(
  String *const content,
  String *const left,
  String *const right,
  llong *const offset
) {
  if (!content || !left || !right || !offset) {
    return nll;
  }

  /* Find the left delimiter starting from the current offset. */
  const llong leftwhence = whence(content, left, *offset);
  /* No more matches exist. */
  if (leftwhence < 0) {
    return nll;
  }

  /* Find the right delimiter starting AFTER the left delimiter. */
  const llong start_idx = leftwhence + Length(String, left);
  const llong rightwhence = whence(content, right, start_idx);
  if (rightwhence < 0) {
    return nll;
  }

  /* Advance the tracked offset past the right delimiter for the next iteration. */
  *offset = rightwhence + Length(String, right);

  /* Extract exactly the content between the delimiters. */
  return substr(content, start_idx, rightwhence - start_idx);
}

Array(Variable) *ParseVariables(String *const line)
{
  if (!line) {
    return nll;
  }

  Array(Variable) *vars = array(Variable, 0);

  String *str_left = string("${");
  String *str_right = string("}");
  String *str_assign = string("$=");

  llong offset = 0;

  while (true) {
    String *appearance = ParseBetween(line, str_left, str_right, &offset);
    /* End of line reached. */
    if (!appearance) {
      break;
    }

    llong assign_idx = whence(line, str_assign, offset);
    /* Malformed line; abort. */
    if (assign_idx < 0) {
      break;
    }
    offset = assign_idx + Length(String, str_assign);

    String *value = ParseBetween(line, str_left, str_right, &offset);
    /* Malformed line; abort. */
    if (!value) {
      break;
    }

    Variable *const var = Create(Variable, appearance, value);
    vars = call(Array(Variable), Insert, vars, -1, var);
  }

  return vars;
}

int Main()
{
  Stream *const stream = stream("mapping", "r");
  if (!stream) {
    return 1;
  }

  Open(stream);

  String *line = nll;
  while ((line = ReadLine(stream, 0))) {
    Regex *const expr = regex(line, "\$\{(\w*)\}");
    Array(String) *const found = extract(expr, 1);
    outln(lit(Array(String), found, nll, string(NL), nll))
  }

  Close(stream);

  return 0;
}
