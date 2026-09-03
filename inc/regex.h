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

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

# include "match.h"

typedef struct Regex Regex;

ARRAY(Regex)
LITERALISE(Regex)

# define regex(string_ptr, ...)\
  (Create(Regex, string_ptr, string(#__VA_ARGS__)))

/**
 * @brief Extracts matched substrings from a compiled Regex instance.
 *
 * This macro acts as an elegant and highly expressive wrapper around
 * Regex_Extract. By leveraging Compound's arglen() macro, it accepts a
 * variable number of numerical indices, granting you granular control over
 * exactly which capture groups are pulled from the text.
 *
 * @complexity & @potential:
 * - **Specific Group Targeting**: Pass indices to extract specific groups
 *   (e.g., `1` for $1). If no extra arguments are provided, it safely defaults
 *   to extracting the full match (Group 0).
 * - **On-the-Fly Reordering**: The order of the provided indices dictates the
 *   exact sequence of the output. Calling `extract(expr, 2, 1)` will append
 *   Group 2 followed by Group 1 to the result array, allowing you to restructure
 *   data instantly without manual array manipulation.
 * - **Negative Navigations**: Supports Python-style negative indexing. Passing
 *   `-1` retrieves the very last capture group in the pattern, `-2` gets the
 *   second to last, etc.
 * - **Duplication**: Indices can be repeated if you need identical copies of
 *   a capture group (e.g., `extract(expr, 1, -1, 1)`).
 *
 * @note The returned Array(String) is a flattened 1D array. It contains the
 *       requested groups sequentially for *every* match found in the original
 *       string.
 *
 * @param regex_ptr A pointer to the compiled Regex instance.
 * @param ...       A comma-separated list of capture group indices.
 *                  Fill with 0 (zero) to indicate every group.
 * @return          A dynamically allocated Array(String) containing the
 *                  extracted substrings.
 */
# define extract(regex_ptr, ...)\
  Regex_Extract(regex_ptr, arglen(__VA_ARGS__), ##__VA_ARGS__)

Regex *Regex_Create(String *const original, String *const expression);
Regex *Regex_CopyOf(Regex *const other);
void Regex_Delete(Regex *const inst);
boolean Regex_Equals(Regex *const obj1, Regex *const obj2);
Array(String) *Regex_Extract(Regex *const inst, const llong group_count, ...);

#endif  /* COMPOUND_REGEX_H */
