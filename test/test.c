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

int Context(void)
{

  return 0;
}

// int StreamTest(void)
// {
//   Stream *const write = stream("/tmp/out", "a");
//   Stream *const read = stream("/tmp/out", "r");

//   soutln(write, string("0123456789012345678901"));

//   out(Read(read));

//   return 0;
// }

int Main(void)
{
  return !(
    sout(
      stream("content", "w"),
      reformat(
        nll, string(NL), string(NL),
        string("0123456789012345678901"),
        Concat(String, string("as;dlfkj"), string("zxcvnbm,.")),
        raw("\${\w*}\b?\UDEADBEEF"),
        raw("<a href="file">C:\Program Files\</a>"),
        format("1st Jun, 1980  08:00 + %lu seconds", time(nll))
      )
    ),
    out(
      Read(stream("content", "r"))
    )
  );

  // /* The definition represents an entire statement. */
  // String *definition = string("${Object}::${Function}(${Parameter1})");

  // Regex *const constants_reg = regex(definition, "(?<=\})(.*?)(?=\$\{)");
  // Regex *const identifiers_reg = regex(definition, "(\$\{)(\w*)(\})");

  // Array(String) *const constants = extract(constants_reg, 0);
  // refrefeach (String, constant, constants, {
  //   *constant = append(nll, string("("), CopyOf(String, *constant), string(")"));
  // })

  // Array(String) *const identifiers = extract(identifiers_reg, 0);
  // refrefeach (String, identifier, identifiers, {
  //   // *identifier = substit(regex(*identifier, "(?<=\})(.*?)(?=\$\{)"), "(\$1(\w*)$3)");
  //   *identifier = raw("\${\w*}");
  // })

  // String *constants_merged = String_Append(nll, constants);
  // String *identifiers_merged = String_Append(nll, identifiers);

  // // definition = substit(constants_reg, "($1)");
  // // definition = substit(identifiers_reg, "(\$1(\w*)$3)");

  // // outln(definition);

  // // Regex *const generated_regex =

  // return 0;
}
