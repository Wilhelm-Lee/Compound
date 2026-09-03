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

/** @file match.h */

#ifndef COMPOUND_MATCH_H
# define COMPOUND_MATCH_H

# include "arrays_lit.h"
# include "string.h"

typedef struct Match Match;

ARRAY(Match)
LITERALISE(Match)

Match *Match_Create(Array(llong) *const bounds);
Match *Match_CopyOf(Match *const other);
void Match_Delete(Match *const inst);
boolean Match_Equals(Match *const obj1, Match *const obj2);
llong Match_GetStart(const Match *const inst, const llong group_idx);
llong Match_GetEnd(const Match *const inst, const llong group_idx);

#endif  /* COMPOUND_MATCH_H */
