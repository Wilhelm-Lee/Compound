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

/** @file const.h */

/* This file includes definitions of constant expressions/values. */

#ifndef COMPOUND_CONST_H
# define COMPOUND_CONST_H

# define EMPTY  {0}

/* To distinguish from Unicode tabulation/space variants. */
# define TABULATION  "\t"
# define SPACE " "
# define WHITESPACE  TABULATION"\n\v\f\r"SPACE

# define PUNCTUATION  "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"

#endif  /* COMPOUND_CONST_H */
