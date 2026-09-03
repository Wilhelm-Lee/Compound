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
  Array(llong) *bounds;  /* Even index = start, Odd index = end */
};

Match *Match_Create(Array(llong) *const bounds)
{
  Match *const inst = Allocate(1, sizeof(Match));
  if (!inst) return nll;

  inst->bounds = bounds;
  return inst;
}

Match *Match_CopyOf(Match *const other)
{
  if (!other) return nll;
  return Create(Match, CopyOf(Array(llong), other->bounds));
}

void Match_Delete(Match *const inst)
{
  if (!inst) return;
  Delete(Array(llong), inst->bounds);
  Deallocate(inst);
}

boolean Match_Equals(Match *const obj1, Match *const obj2)
{
  if (!obj1 || !obj2) return false;
  if (obj1 == obj2) return true;
  return Equals(Array(llong), obj1->bounds, obj2->bounds, null);
}

String *Match_Literalise(Match *const inst)
{
  if (!inst) {
    return nll;
  }

  return lit(Array(llong), inst->bounds, nll, string(NL), nll);
}

llong Match_GetStart(const Match *const inst, const llong group_idx)
{
  if (!inst || !inst->bounds) return -1;

  /* Every group takes 2 elements (start and end) */
  const llong total_groups = capacity(Array(llong), inst->bounds) / 2;

  const llong norm_idx = group_idx < 0 ? group_idx + total_groups : group_idx;

  if (norm_idx < 0 || norm_idx >= total_groups) return -1;

  return get(Array(llong), inst->bounds, norm_idx * 2);
}

llong Match_GetEnd(const Match *const inst, const llong group_idx)
{
  if (!inst || !inst->bounds) return -1;

  const llong total_groups = capacity(Array(llong), inst->bounds) / 2;
  const llong norm_idx = group_idx < 0 ? group_idx + total_groups : group_idx;

  if (norm_idx < 0 || norm_idx >= total_groups) return -1;

  return get(Array(llong), inst->bounds, norm_idx * 2 + 1);
}

IMPL_ARRAY(Match)
IMPL_ARRAY_LITERALISE(Match)
