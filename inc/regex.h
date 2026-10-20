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

/** @file regex.h */

#ifndef COMPOUND_REGEX_H
# define COMPOUND_REGEX_H

/* TODO(william): Please extend the configuration for users. */
# define PCRE2_CODE_UNIT_WIDTH  8

# include "../dep/pcre2/interface/pcre2.h"

# include "match.h"

typedef struct Regex Regex;

ARRAY(Regex)
LITERALISE(Regex)

# define regex(string_ptr, ...)                                                \
  (Create(Regex, string_ptr, string(#__VA_ARGS__)))

/* Provide indices to extract; provide 0 to extract them all. */
# define extract(regex_ptr, ...)                                               \
  (Regex_Extract(regex_ptr, Compose(Array(int), __VA_ARGS__)))

# define substit(regex_ptr, raw_replacement_expression_cstr)                   \
  (Regex_Substitute(regex_ptr, raw(raw_replacement_expression_cstr)))

void InitialiseRegex(void);
void DeinitialiseRegex(void);

Regex *Regex_Create(String *const original, String *const expression);
Regex *Regex_CopyOf(Regex *const other);
void Regex_Delete(Regex *const inst);
boolean Regex_Equals(Regex *const inst, Regex *const other);
Array(String) *Regex_Extract(Regex *const inst, Array(int) *const indices);
Array(String) *Regex_ExtractAll(Regex *const inst);
String *Regex_Substitute(Regex *const inst, const String *const replacement);

#endif  /* COMPOUND_REGEX_H */
