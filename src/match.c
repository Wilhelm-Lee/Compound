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

/** @file match.c */

#include "../inc/match.h"

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

/** @file match.c */

#include "../inc/match.h"

struct Match {
  Array(int) *bounds;  /* Even index = start, Odd index = end */
};

Match *Match_Create(Array(int) *const bounds)
{
  Match *const inst = Allocate(sizeof(Match));
  if (!inst) return nll;

  inst->bounds = bounds;
  return inst;
}

Match *Match_CopyOf(Match *const other)
{
  if (!other) return nll;
  return Create(Match, CopyOf(Array(int), other->bounds));
}

void Match_Delete(Match *const inst)
{
  if (!inst) return;
  Delete(Array(int), inst->bounds);
  Deallocate(inst);
}

boolean Match_Equals(Match *const inst, Match *const other)
{
  if (!inst || !other) {
    return false;
  }

  if (inst == other) {
    return true;
  }
  
  return Equals(Array(int), inst->bounds, other->bounds, null);
}

String *Match_Literalise(Match *const inst)
{
  if (!inst) {
    return nll;
  }

  return lit(Array(int), inst->bounds, nll, string(NL), nll);
}

int Match_GetStart(const Match *const inst, const int group_idx)
{
  if (!inst || !inst->bounds) {
    return -1;
  }

  /* Every group takes 2 elements (start and end) */
  const int total_groups = capacity(Array(int), inst->bounds) / 2;

  const int norm_idx = group_idx < 0 ? group_idx + total_groups : group_idx;

  if (norm_idx < 0 || norm_idx >= total_groups) {
    return -1;
  }

  return get(Array(int), inst->bounds, norm_idx * 2);
}

int Match_GetEnd(const Match *const inst, const int group_idx)
{
  if (!inst || !inst->bounds) {
    return -1;
  }

  const int total_groups = capacity(Array(int), inst->bounds) / 2;
  const int norm_idx = group_idx < 0 ? group_idx + total_groups : group_idx;

  if (norm_idx < 0 || norm_idx >= total_groups) {
    return -1;
  }

  return get(Array(int), inst->bounds, norm_idx * 2 + 1);
}

IMPL_ARRAY(Match)
IMPL_ARRAY_LITERALISE(Match)
