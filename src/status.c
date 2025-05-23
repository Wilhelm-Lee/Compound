/*
 * This file is part of Compound library.
 * Copyright (C) 2024-TODAY  William Lee
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

#include <Compound/status.h>
#include <Compound/literalisation.h>

// //   id     description          prev.id    value               char   loc_buff
// size_t Location_Literalise(char *buff, const void *object, const char *format,
//                            int countdown, LiteralisationCallback *callback,
//                            ...) {
//   if (!buff || !object || countdown <= 0) {
//     return 0;
//   }
//   size_t written = 0;
//   if (callback) {
//     written = callback->offset;
//     written += callback->_Literalise((buff), Location_LITERALISATION_FORMAT,
//                                      65536, LocationLiteralisationCallback);
//     callback->offset = written;
//     return written;
//   }
//   return snprintf(buff, 8192, "%p", object);
// };

size_t Location_Literalise(char *buff, const Location loc)
{
  if (!buff)  return 0;
  
  return snprintf(buff, LITERALISATION_LENGTH_MAXIMUM,
                  LOCATION_LITERALISE_FORMAT, loc.file, loc.line, loc.func);
}

boolean Location_Equal(const Location loc1, const Location loc2)
{
  if (!loc1.file || !loc2.file
      || !loc1.func || !loc2.func)  return false;

  return (!strcmp(loc1.file, loc2.file)
          && (loc1.line == loc2.line)
          && (!strcmp(loc1.func, loc2.func)));
}

boolean Status_Equal(const Status stat1, const Status stat2)
{
  if (!stat1.identity || !stat2.identity ||
      !stat1.description || !stat2.description)  return false;

  return (!strcmp(stat1.identity, stat2.identity)
          && stat1.value == stat2.value
          && !strcmp(stat1.description, stat2.description)
          && stat1.characteristic == stat2.characteristic
          && Location_Equal(stat1.location, stat2.location)
          && stat1.prev == stat2.prev);
}

boolean Status_Match(const Status stat1, const Status stat2)
{
  if (!stat1.identity || !stat2.identity)  return false;

  return (!strcmp(stat1.identity, stat2.identity));
}

boolean Status_Belong(const Status stat1, const Status stat2)
{
  /* Get last Status from @stat. */
  Status last = stat1;
  while (Status_HasPrev(last)) {
    last = *last.prev;
  }

  return Status_Match(stat2, last);
}

// size_t Status_Literalise(const Status stat, char *buff)
// {
//   if (!buff)  return 0;

//   char loc_buff[LITERALISATION_LENGTH_MAXIMUM] = EMPTY;
//   return (Location_Literalise(stat.loc, loc_buff) +
//           snprintf(buff, LITERALISATION_LENGTH_MAXIMUM,
//                    STATUS_LITERALISE_FORMAT,
//                    strnil(stat.identity),
//                    strnil(stat.description),
//                    stat.prev,
//                    stat.value, stat.characteristic, loc_buff));
// }

// IMPL_LITERALISATION(Status, strnil(stat.identity),
//                     strnil(stat.description),
//                     stat.prev,
//                     stat.value, stat.characteristic, loc_buff);

boolean Status_HasPrev(Status stat)
{
  return (stat.prev != NULL && !Status_IsRecursive(stat));
}

boolean Status_IsOkay(Status stat)
{
  return (!stat.characteristic);
}

boolean Status_IsRecursive(Status stat)
{
  return (stat.prev
          && Status_Equal(stat, (!stat.prev ? (Status)EMPTY : *stat.prev)));
}

void Status_Dump(const Status inst, Status *store)
{
  /* Skip when store is unavailable, or, inst is unavailable,
     recursive or does not have any predecessor. */
  if (!store || Status_IsRecursive(inst) || !Status_HasPrev(inst))  return;

  *store = *inst.prev;
}

int Status_Depth(const Status stat)
{
  /* Set up counter. */
  int cnt = 1;
  /* Set up current status indication representor. */
  Status current = stat;
  /* Iterate to accumulate. */
  while (current.prev) {
    /* Skip recursive status. */
    if (Status_IsRecursive(current))  break;

    current = *current.prev;
    cnt += 1;
  }

  return cnt;
}

// IMPL_STACK(Status);

// IMPL_LITERALISATION(Status);
