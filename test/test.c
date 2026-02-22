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

#include "../inc/string.h"
#include "../inc/memory_stack.h"

/* String Tests:
 * [X] Create
 * [X] CopyOf
 * [X] Delete
 * [X] Equals
 * [X] Update
 * [X] Compare
 * [X] Concat
 * [X] Length
 *
 * [X] Substr
 * [X] Tokens
 * [X] Breaks
 * [X] Gather
 * [X] Whence
 *
 * [X] Blank
 * [X] Trim
 * [X] RemoveLeadingWhitespace
 * [X] RemoveTrailingWhitespace
 * [X] FirstAt
 * [X] LastAt
 * [X] StrCut
 *
 * [X] Insert
 * [X] Flatten
 * [X] Contains
 *
 * [X] Remove
 * [X] CountOccurrences
 * [X] Occurrences
 * [X] ReplaceFirst
 * [X] ReplaceAll
 */

inline size_t strout(const String *const string)
{
  if (!string) {
    return 0;
  }

  return printf("%s", fallback(string));
}

inline size_t stroutln(const String *const string)
{
  if (!string) {
    return printf(NEWLINE);
  }

  return printf("%s"NEWLINE, fallback(string));
}

inline void PrintLlongArrayElement(const Array(llong) *const array)
{
  if (!array) {
    return;
  }

  iterate(i, array) {
    printf("%lld ", get(llong, array, i));
  }
  puts("");
}

inline void PrintTokens(const String *const inst)
{
  if (!inst) {
    return;
  }

  const llong count = String_CountTokens(inst);
  if (!count) {
    return;
  }

  for (register llong i = 0; i < count; i++) {
    const String *const token = breaks(inst, i);
    stroutln(token);
    Delete(String, &token);
  }
}

# define sout  stroutln

int CompoundTest(void)
{
  String *content = string("#This is(not, an) apple");

  Array(ptr) *tokens = tokenise(content, PUNCTUATION WHITESPACE);
  if (!tokens) {
    return 1;
  }

  foreach (ptr, token, tokens, {
    sout(token);
  })
  puts(".");

  Array(ptr) *string_ptrs = array(ptr, 1000);
  refeach (ptr, string_ptr, string_ptrs, {
    *string_ptr = String_Create(rand() % 50, sizeof(byte));
  })

  return 0;
}

int main(void)
{
  InitialiseMemoryStack(&MEMORY_STACK);

  const int retval = CompoundTest();

  DeinitialiseMemoryStack(&MEMORY_STACK);

  return retval;
}





