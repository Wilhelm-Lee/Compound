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

extern void RemoveQuotePair(String **const expression);

pcre2_general_context *gcontext = nll;

struct Regex {
  String *original;
  String *expression;
  pcre2_code *compiled_expression_pointer;
  Array(Match) *matches;
  int compile_return_code;
  int execute_return_code;
};

static Array(int) *GetGroups(Array(int) *const indices)
{
  const int group_count = indices ? Length(Array(int), indices) : 0;

  /* Default to fetching group 0 (full match) if no arguments provided */
  const int actual_count = group_count > 0 ? group_count : 1;
  Array(int) *groups = array(int, actual_count);
  if (!groups) {
    return nll;
  }

  if (!group_count) {
    set(Array(int), groups, 0, 0);
    return groups;
  }

  loop (i, group_count) {
    /* C varargs promote standard integer literals to 'int' */
    set(Array(int), groups, i, get(Array(int), indices, i));
  }

  return groups;
}

static String *ExtractSingular(
  Array(int) *const groups,
  Match *const match,
  String *const original
) {
  if (!groups || !match || !original) {
    return nll;
  }

  refeach (int, group, groups, {
    const int start = Match_GetStart(match, *group);
    const int end = Match_GetEnd(match, *group);

    if (start >= 0 && end >= 0 && end >= start) {
      /* Early return replaces variable mutation + break */
      return substr(original, start, end - start);
    }
  })

  return nll;
}

static boolean CompileCore(Regex *const inst)
{
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
    0,
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
  return true;
}

static int CountOccurrences(
  Regex *const inst,
  const char *const orig_cstr,
  const int orig_len
) {
  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(
    inst->compiled_expression_pointer,
    null
  );

  if (!match_data) {
    return 0;
  }

  int count = 0;
  PCRE2_SIZE offset = 0;

  while (pcre2_match(
           inst->compiled_expression_pointer,
           (PCRE2_SPTR)orig_cstr,
           orig_len,
           offset,
           0,
           match_data,
           NULL) > 0)
  {
    count++;
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
    PCRE2_SIZE step = ovector[1];

    /* Zero-length match guardrail */
    if (ovector[0] == ovector[1]) {
      step++;
    }

    offset = step;
    if (offset >= (PCRE2_SIZE)orig_len) {
      break;
    }
  }

  pcre2_match_data_free(match_data);
  return count;
}

static void ExtractMatches(
  Regex *const inst,
  const char *const orig_cstr,
  const int orig_len
) {
  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(
    inst->compiled_expression_pointer,
    null
  );

  if (!match_data) {
    return;
  }

  PCRE2_SIZE offset = 0;

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
    if (offset >= (PCRE2_SIZE)orig_len) {
      break;
    }
  }

  pcre2_match_data_free(match_data);
}

static String *SubstituteCore(
  Regex *const inst,
  const char *const subject_cstr,
  const int subject_len,
  const char *const replace_cstr,
  const int replace_len
) {
  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(
    inst->compiled_expression_pointer,
    null
  );

  if (!match_data) {
    return nll;
  }

  /* Heuristic initial buffer size: Subject length + replacement length margin */
  PCRE2_SIZE out_len = subject_len + (replace_len * 2) + 128;
  PCRE2_UCHAR *out_buffer = Allocate(out_len * sizeof(PCRE2_UCHAR));

  int rc = PCRE2_ERROR_NOMEMORY;

  /* Loop cleanly handles the OVERFLOW_LENGTH reallocation pass */
  while (rc == PCRE2_ERROR_NOMEMORY) {
    if (!out_buffer) {
      break;
    }

    rc = pcre2_substitute(
      inst->compiled_expression_pointer,
      (PCRE2_SPTR)subject_cstr, subject_len,
      0,
      PCRE2_SUBSTITUTE_GLOBAL | PCRE2_SUBSTITUTE_OVERFLOW_LENGTH,
      match_data,
      null,
      (PCRE2_SPTR)replace_cstr, replace_len,
      out_buffer, &out_len
    );

    /* Handle buffer overflow gracefully by reallocating to the exact required size */
    if (rc == PCRE2_ERROR_NOMEMORY) {
      Deallocate(out_buffer);
      out_buffer = Allocate(out_len * sizeof(PCRE2_UCHAR));
    }
  }

  String *result = nll;
  if (rc >= 0 && out_buffer) {
    out_buffer[out_len] = '\0';
    result = string((char *)out_buffer);
  }

  if (out_buffer) {
    Deallocate(out_buffer);
  }

  pcre2_match_data_free(match_data);

  return result;
}

static void *PCRE2PrivateAllocator(size_t size, void *userdata)
{
  ig userdata;

  return Allocate(size);
}

static void PCRE2PrivateDeallocator(void *const addr, void *userdata)
{
  ig addr;
  ig userdata;

  Deallocate(addr);
}

void InitialiseRegex(void)
{
  gcontext = pcre2_general_context_create(
    PCRE2PrivateAllocator,
    PCRE2PrivateDeallocator,
    nll
  );

  if (!gcontext) {
    fprintf(
      stderr,
      "Failed to initialise Regex."NL
      "  pcre2_general_context_create returned NULL."NL
    );

    exit(1);
  }
}

void DeinitialiseRegex(void)
{
  pcre2_general_context_free(gcontext);
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
  RemoveQuotePair(&inst->expression);

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

  if (!CompileCore(inst)) {
    return false;
  }

  if (!inst->original) {
    /* Prevent memory leak on early exit */
    pcre2_code_free(inst->compiled_expression_pointer);
    inst->compiled_expression_pointer = null;
    inst->compile_return_code = -1;
    return false;
  }

  char *orig_cstr = flatten(char, inst->original);
  if (!orig_cstr) {
    pcre2_code_free(inst->compiled_expression_pointer);
    inst->compiled_expression_pointer = null;
    inst->compile_return_code = -1;
    return false;
  }

  const int orig_len = Length(String, inst->original);

  /* Count occurrences to determine if population is necessary */
  int match_count = CountOccurrences(inst, orig_cstr, orig_len);

  if (match_count > 0) {
    /* Optional: Pre-allocate Array(Match) capacity using match_count here */
    ExtractMatches(inst, orig_cstr, orig_len);
  }

  Deallocate(orig_cstr);

  return true;
}

Array(String) *Regex_Extract(Regex *const inst, Array(int) *const indices)
{
  if (!inst || !inst->original) {
    return nll;
  }

  /* Merge nested compilation guard into a single short-circuit condition */
  if (!inst->compiled_expression_pointer && !CompileCore(inst)) {
    return nll;
  }

  /* Reset internal matches array to prevent duplicate appendations */
  if (inst->matches) {
    erase(Array(Match), inst->matches);
    Delete(Array(Match), inst->matches);
  }
  inst->matches = array(Match, 0);

  char *orig_cstr = flatten(char, inst->original);
  if (!orig_cstr) {
    return nll;
  }

  const int orig_len = Length(String, inst->original);
  ExtractMatches(inst, orig_cstr, orig_len);
  Deallocate(orig_cstr);

  const int match_count = Length(Array(Match), inst->matches);
  Array(String) *results = array(String, match_count);
  if (!results) {
    return nll;
  }

  Array(int) *groups = GetGroups(indices);
  if (!groups) {
    Delete(Array(String), results);
    return nll;
  }

  refeach (Match, match, inst->matches, {
    String *extracted_str = ExtractSingular(groups, match, inst->original);
    set(Array(String), results, _refeach_idx_match, extracted_str);
  })

  Delete(Array(int), groups);
  return results;
}

String *Regex_Substitute(Regex *const inst, const String *const replacement)
{
  if (!inst || !replacement) {
    return nll;
  }

  if (!inst->original) {
    return CopyOf(String, replacement);
  }

  /* Use the subfunction to compile
   * without triggering the heavy Match extraction */
  if (!CompileCore(inst)) {
    return nll;
  }

  char *subject_cstr = flatten(char, inst->original);
  char *replace_cstr = flatten(char, replacement);

  if (!subject_cstr || !replace_cstr) {
    if (subject_cstr) {
      Deallocate(subject_cstr);
    }

    if (replace_cstr) {
      Deallocate(replace_cstr);
    }

    return nll;
  }

  const int subject_len = Length(String, inst->original);
  const int replace_len = Length(String, replacement);

  String *result = SubstituteCore(
    inst,
    subject_cstr, subject_len,
    replace_cstr, replace_len
  );

  Deallocate(subject_cstr);
  Deallocate(replace_cstr);

  /* Atomic cleanup operation for singular usage */
  if (inst->compiled_expression_pointer) {
    pcre2_code_free(inst->compiled_expression_pointer);
    inst->compiled_expression_pointer = null;
  }

  return result;
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
