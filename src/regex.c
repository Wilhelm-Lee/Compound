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

/** @file regex.c */

/* Ensure PCRE2 unit width is defined BEFORE the header */
#include "../inc/regex.h"

struct Regex {
  String *original;
  String *expression;
  pcre2_code *re;  /* PCRE2 compiled expression pointer */
  Array(Match) *matches;
  int compile_return_code;
  int execute_return_code;
};

Regex *Regex_Create(String *const original, String *const expression)
{
  Regex *const inst = Allocate(1, sizeof(Regex));
  if (!inst) {
    retnll;
  }

  inst->original = CopyOf(String, original);
  inst->expression = substr(CopyOf(String, expression), 1, Length(String, expression) - 2);
  inst->matches = array(Match, 0);
  inst->re = null;
  inst->compile_return_code = -1;
  inst->execute_return_code = -1;

  return inst;
}

Regex *Regex_CopyOf(Regex *const other)
{
  if (!other) {
    return nll;
  }

  Regex *const inst = Allocate(1, sizeof(Regex));
  if (!inst) {
    return nll;
  }

  inst->original = CopyOf(String, other->original);
  inst->expression = CopyOf(String, other->expression);
  inst->matches = CopyOf(Array(Match), other->matches);
  inst->re = null;
  inst->compile_return_code = -1;
  inst->execute_return_code = -1;

  return inst;
}

void Regex_Delete(Regex *const inst)
{
  if (!inst) {
    return;
  }

  if (inst->re) {
    pcre2_code_free(inst->re);
  }

  Delete(String, inst->original);
  Delete(String, inst->expression);
  erase(Array(Match), inst->matches);
  Delete(Array(Match), inst->matches);
  Deallocate(inst);
}

boolean Regex_Equals(Regex *const obj1, Regex *const obj2)
{
  if (!obj1 || !obj2) {
    return false;
  }

  if (obj1 == obj2) {
    return true;
  }

  return Equals(String, obj1->original, obj2->original) &&
         Equals(String, obj1->expression, obj2->expression) &&
         Equals(Array(Match), obj1->matches, obj2->matches, Match_Equals);
}

boolean Regex_Compile(Regex *const inst)
{
  if (!inst || !inst->expression) {
    return false;
  }

  char *expr_cstr = flatten(char, inst->expression);
  if (!expr_cstr) {
    return false;
  }

  if (inst->re) {
    pcre2_code_free(inst->re);
    inst->re = null;
  }

  int errornumber;
  PCRE2_SIZE erroroffset;

  inst->re = pcre2_compile(
    (PCRE2_SPTR)expr_cstr,
    PCRE2_ZERO_TERMINATED,
    0, /* Default options */
    &errornumber,
    &erroroffset,
    NULL
  );

  Deallocate(expr_cstr);

  if (!inst->re) {
    inst->compile_return_code = -1;
    return false;
  }

  inst->compile_return_code = 0;

  if (!inst->original) {
    /* Prevent the exit memory leak identified earlier */
    pcre2_code_free(inst->re);
    inst->re = null;
    inst->compile_return_code = -1;
    return false;
  }

  char *orig_cstr = flatten(char, inst->original);
  if (!orig_cstr) {
    return false;
  }

  const llong orig_len = Length(String, inst->original);
  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(inst->re, NULL);
  PCRE2_SIZE offset = 0;

  /* Execute PCRE2 match block */
  while ((inst->execute_return_code = pcre2_match(
             inst->re,
             (PCRE2_SPTR)orig_cstr,
             orig_len,
             offset,
             0,
             match_data,
             NULL)) > 0)
  {
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);

    /* Dynamically allocate an array to hold all bounds for this match */
    Array(llong) *bounds = array(llong, inst->execute_return_code * 2);
    for (register llong i = 0; i < inst->execute_return_code; i++) {
      set(Array(llong), bounds, i * 2, (llong)ovector[i * 2]);
      set(Array(llong), bounds, i * 2 + 1, (llong)ovector[i * 2 + 1]);
    }

    Match *const match = Create(Match, bounds);
    inst->matches = call(Array(Match), Insert, inst->matches, -1, match);

    PCRE2_SIZE step = ovector[1];
    if (ovector[0] == ovector[1]) {
      step++;
    }
    offset = step;
    if (offset >= (PCRE2_SIZE)orig_len) break;
  }

  pcre2_match_data_free(match_data);
  Deallocate(orig_cstr);

  return true;
}

/* Update Regex_Extract to accept and process varargs */
Array(String) *Regex_Extract(Regex *const inst, const llong group_count, ...)
{
  if (!inst) {
    return nll;
  }

  if (!inst->original || !inst->matches) {
    return array(String, 0);
  }

  Regex_Compile(inst);

  /* Default to fetching group 0 (full match) if no arguments provided */
  const llong actual_count = group_count > 0 ? group_count : 1;
  Array(llong) *groups = array(llong, actual_count);

  if (group_count > 0) {
    va_list ap;
    va_start(ap, group_count);
    loop(i, group_count) {
      /* C varargs promote standard integer literals to 'int' */
      set(Array(llong), groups, i, (llong)va_arg(ap, int));
    }
    va_end(ap);
  } else {
    set(Array(llong), groups, 0, 0);
  }

  const llong match_count = Length(Array(Match), inst->matches);
  Array(String) *const extracted = array(String, match_count * actual_count);

  if (!extracted) {
    Delete(Array(llong), groups);
    return nll;
  }

  register llong write_idx = 0;
  refeach (Match, match, inst->matches, {
    if (!match) {
      continue;
    }

    refeach (llong, g, groups, {
      const llong start = Match_GetStart(match, *g);
      const llong end = Match_GetEnd(match, *g);

      if (start < 0 || end < 0 || end < start) {
        set(Array(String), extracted, write_idx, string(""));
      } else {
        set(
          Array(String),
          extracted,
          write_idx,
          substr(inst->original, start, end - start)
        );
      }
      write_idx++;
    })
  })

  Delete(Array(llong), groups);

  if (inst->re) {
    pcre2_code_free(inst->re);
    inst->re = null;
  }

  return extracted;
}

String *Regex_Literalise(Regex *const inst)
{
  if (!inst) {
    return nll;
  }

  String *str_regex = string("Regex(");
  String *str_comma = string(", ");
  String *str_end = string(")");

  String *rtn = CopyOf(String, str_regex);
  if (inst->expression) {
    rtn = append(rtn, inst->expression);
  }

  rtn = append(rtn, str_comma);

  if (inst->original) {
    rtn = append(rtn, inst->original);
  }

  rtn = append(rtn, str_end);

  Delete(String, str_regex);
  Delete(String, str_comma);
  Delete(String, str_end);

  return rtn;
}

IMPL_ARRAY(Regex)
IMPL_ARRAY_LITERALISE(Regex)
