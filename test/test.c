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

// # define __COMPOUND_FEATURE_ARGUMENT__
// # define __COMPOUND_FEATURE_ENVIRONMENT__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../inc/entry.h"
#include "../inc/literalisation.h"
#include "../inc/platform.h"

/* Array Tests:
 * [ ] Insert
 * [ ] Remove
 * [ ] Assign
 * [ ] Clone
 * [ ] Erase
 * [ ] erase
 * [ ] Fill
 */

/* Link Tests:
 * [ ] Create
 * [ ] CopyOf
 * [ ] Delete
 * [ ] Count
 * [ ] Insert
 * [ ] Remove
 * [ ] Equals
 * [ ] GetData
 * [ ] GetPrev
 * [ ] GetNext
 */

/* init Tests:
 * [ ] _Main
 */

void PrintIntArray(const Array(int) *const iarr)
{
  printf("[");
  const llong capacity = call(Array(int), GetCapacity, iarr);
  iterate (int, i, iarr, {
    if (i < capacity - 1) {
      printf("%d, ", *ref(Array(int), iarr, i));
      continue;
    }

    printf("%d]"NEWLINE, *ref(Array(int), iarr, i));
  })
}

int ArrayTest(void)
{
  Array(int) *iarr = array(int, 5);
  if (!iarr) {
    return 1;
  }

  PrintIntArray(iarr);

  int ten = 10;
  iarr = call(Array(int), Insert, iarr, 3, &ten);

  PrintIntArray(iarr);

  iarr = call(Array(int), Insert, iarr, 3, &ten);

  Delete(Array(int), iarr);

  return 0;
}

// int LinkTest(void)
// {
//   Link(int) *link = Create(Link(int), 10);
//   if (!link) {
//     return 1;
//   }

//   Link(int) *current = link;
//   while (current) {
//     current->data = rand() % 10;
//     printf("%d ", current->data);
//     current = current->next;
//   }
//   puts("");

//   return 0;
// }

# define fatal(...)                                                            \
  fprintf(stderr, __VA_ARGS__)

void VerifyRefRefReferencingIntegrity(const Array(String) *const inst);
String *GenerateRandomDataString(const llong length);
String *String_Literalise(const String *const inst);
void PrintStringArray(const Array(String) *const inst);

int Main(const Array(String) *const args)
{
  refeach (String, arg, args, {
    char *const arg_flatten = flatten(char, arg);
    fatal("%s ", arg_flatten);
    Deallocate(arg_flatten);
  })
  // puts("");
  fatal("%s", NEWLINE);

  return 0;
}

// int main(int argc, const char *const *const argv)
// {
//   ignore argc;
//   ignore argv;

//   // todo(william): Remember to add Signal handling, e.g. SIGTERM.

//   if (argc <= 1) {
//     fatal("No args provided."NEWLINE);
//     return 1;
//   }

//   Array(String) *args = array(String, argc - 1);
//   if (!args) {
//     fatal("Failed to construct args."NEWLINE);
//     return 1;
//   }

//   register llong i = 1;
//   refrefeach (String, it, args, {
//     *it = string(argv[i]);
//     i++;
//   })

//   String *const programme_name = string(argv[0]);
//   if (!programme_name) {
//     fatal("Failed to create a String instance."NEWLINE);
//     return 1;
//   }

//   char *const flatten = flatten(char, programme_name);
//   fatal("%s", flatten);
//   Deallocate(flatten);

//   refeach (String, arg, args, {
//     char *const arg_flatten = flatten(char, arg);
//     fatal(" %s", arg_flatten);
//     Deallocate(arg_flatten);
//   })
//   puts("");

//   Delete(String, programme_name);
//   erase(Array(String), args);
//   Delete(Array(String), args);

//   return 0;
// }

String *GenerateRandomDataString(const llong length)
{
  if (length < 0) {
    return NULL;
  }

  if (!length) {
    return string("");
  }

  String *const retval = Create(String, length, sizeof(byte));
  if (!retval) {
    return NULL;
  }

  const llong capa = Getter(Array(byte), Capacity, Getter(String, Data, retval)) - 1;
  loop (i, capa) {
    *refbyte(retval, i) = rand() % 26 + 'A';
  }

  return retval;
}

// clear
inline String *String_Literalise(const String *const inst)
{
  if (!inst) {
    return string("(nil)");
  }

  char *flatten = flatten(char, inst);
  if (!flatten) {
    flatten = "(nil:flatten error)";
  }

  String *const lit = format(
      "String = {"NEWLINE
      "  .data = \"%s\","NEWLINE
      "  .width = %lld,"NEWLINE
      "  .breaks = %p"NEWLINE
      "}",
      flatten,
      Getter(String, Width, inst),
      Getter(String, Breaks, inst)
  );

  free(flatten);

  return lit;
}

void PrintStringArray(const Array(String) *const inst)
{
  if (!inst) {
    fatal("(nil)"NEWLINE);
    return;
  }

  refeach (String, string, inst, {
    if (!string) {
      fatal("(nil)"NEWLINE);
      continue;
    }

    String *const lit = lit(String, string);
    char *const flatten = flatten(char, lit);

    fatal("%s"NEWLINE, flatten);

    free(flatten);
    Delete(String, lit);
  })
}

// Next feature: lit
