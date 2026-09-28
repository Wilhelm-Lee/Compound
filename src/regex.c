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
  pcre2_code *compiled_expression_pointer;
  Array(Match) *matches;
  int compile_return_code;
  int execute_return_code;
};

static void _Regex_RemoveQuotePair(String **const expression)
{
  if (!expression || !*expression) {
    return;
  }

  if (blank(*expression)) {
    return;
  }

  const int length = Length(String, *expression);
  if (length < 2) {
    return;
  }

  *expression = substr(*expression, 1, length - 1);
  setbyte(*expression, length - 2, 0);
}

Regex *Regex_Create(String *const original, String *const expression)
{
  Regex *const inst = Allocate(sizeof(Regex));
  if (!inst) {
    return nll;
  }

  inst->original = CopyOf(String, original);
  inst->expression = CopyOf(String, expression);
  inst->matches = array(Match, 0);
  inst->compiled_expression_pointer = null;
  inst->compile_return_code = -1;
  inst->execute_return_code = -1;

  /* By allowing user to directly input the Regex Expression in code,
   * we used quoting in C Preprocessor that quotes the literal; rendering
   * the result requires removal for those additional characters.
   */
  _Regex_RemoveQuotePair(&inst->expression);

  return inst;
}

Regex *Regex_CopyOf(Regex *const other)
{
  if (!other) {
    return nll;
  }

  Regex *const inst = Allocate(sizeof(Regex));
  if (!inst) {
    return nll;
  }

  inst->original = CopyOf(String, other->original);
  inst->expression = CopyOf(String, other->expression);
  inst->matches = CopyOf(Array(Match), other->matches);
  inst->compiled_expression_pointer = null;
  inst->compile_return_code = -1;
  inst->execute_return_code = -1;

  return inst;
}

void Regex_Delete(Regex *const inst)
{
  if (!inst) {
    return;
  }

  if (inst->compiled_expression_pointer) {
    pcre2_code_free(inst->compiled_expression_pointer);
  }

  Delete(String, inst->original);
  Delete(String, inst->expression);
  erase(Array(Match), inst->matches);
  Delete(Array(Match), inst->matches);
  Deallocate(inst);
}

boolean Regex_Equals(Regex *const inst, Regex *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }

  return Equals(String, inst->original, other->original)
      && Equals(String, inst->expression, other->expression)
      && Equals(Array(Match), inst->matches, other->matches, Match_Equals);
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

  if (inst->compiled_expression_pointer) {
    pcre2_code_free(inst->compiled_expression_pointer);
    inst->compiled_expression_pointer = null;
  }

  int errornumber;
  PCRE2_SIZE erroroffset;

  inst->compiled_expression_pointer = pcre2_compile(
    (PCRE2_SPTR)expr_cstr,
    PCRE2_ZERO_TERMINATED,
    0, /* Default options */
    &errornumber,
    &erroroffset,
    NULL
  );

  Deallocate(expr_cstr);

  if (!inst->compiled_expression_pointer) {
    inst->compile_return_code = -1;
    return false;
  }

  inst->compile_return_code = 0;

  if (!inst->original) {
    /* Prevent the exit memory leak identified earlier */
    pcre2_code_free(inst->compiled_expression_pointer);
    inst->compiled_expression_pointer = null;
    inst->compile_return_code = -1;
    return false;
  }

  char *orig_cstr = flatten(char, inst->original);
  if (!orig_cstr) {
    return false;
  }

  const int orig_len = Length(String, inst->original);
  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(inst->compiled_expression_pointer, NULL);
  PCRE2_SIZE offset = 0;

  /* Execute PCRE2 match block */
  while ((inst->execute_return_code = pcre2_match(
             inst->compiled_expression_pointer,
             (PCRE2_SPTR)orig_cstr,
             orig_len,
             offset,
             0,
             match_data,
             NULL)) > 0)
  {
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);

    /* Dynamically allocate an array to hold all bounds for this match */
    Array(int) *bounds = array(int, inst->execute_return_code * 2);
    for (register int i = 0; i < inst->execute_return_code; i++) {
      set(Array(int), bounds, i * 2, (int)ovector[i * 2]);
      set(Array(int), bounds, i * 2 + 1, (int)ovector[i * 2 + 1]);
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
Array(String) *Regex_Extract(Regex *const inst, Array(int) *const indices)
{
  if (!inst) {
    return nll;
  }

  if (!inst->original || !inst->matches) {
    return array(String, 0);
  }

  Regex_Compile(inst);

  const int group_count = Length(Array(int), indices);

  /* Default to fetching group 0 (full match) if no arguments provided */
  const int actual_count = Length(Array(int), indices);
  Array(int) *groups = array(int, actual_count);

  if (group_count > 0) {
    loop (i, group_count) {
      /* C varargs promote standard integer literals to 'int' */
      set(Array(int), groups, i, get(Array(int), indices, i));
    }
  } else {
    set(Array(int), groups, 0, 0);
  }

  const int match_count = Length(Array(Match), inst->matches);
  Array(String) *const extracted = array(String, match_count * actual_count);

  if (!extracted) {
    Delete(Array(int), groups);
    return nll;
  }

  register int write_idx = 0;
  refeach (Match, match, inst->matches, {
    if (!match) {
      continue;
    }

    refeach (int, group, groups, {
      const int start = Match_GetStart(match, *group);
      const int end = Match_GetEnd(match, *group);

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

  Delete(Array(int), groups);

  if (inst->compiled_expression_pointer) {
    pcre2_code_free(inst->compiled_expression_pointer);
    inst->compiled_expression_pointer = null;
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
