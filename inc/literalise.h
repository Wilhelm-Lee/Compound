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

/** @file literalise.h */

#ifndef COMPOUND_LITERALISE_H
# define COMPOUND_LITERALISE_H

# include "array.h"

# define lit(type, ...)                                                        \
  call(type, Literalise, __VA_ARGS__)

/* Ellipsis is for configurations. */
# define litarr(elem_type, ...)                                                \
  call(Array(elem_type), LiteraliseWrapper, __VA_ARGS__)

# define LITERALISE(type)                                                      \
  String *type##_Literalise(type *const inst);                                 \
  FUNC_ARRAY_LITERALISE(type)

# define LITERALISE_ARGS(type, ...)                                            \
  String *type##_Literalise(type *const inst, __VA_ARGS__);                    \
  FUNC_ARRAY_LITERALISE_CONFIGS(type, __VA_ARGS__)

# define _ARRAY_LITERALISE_PARAMS(elem_type)                                   \
  Array(elem_type) *const inst,                                                \
  String *const prefix,                                                        \
  String *const separator,                                                     \
  String *const suffix

# define _IMPL_ARRAY_LITERALISE_BODY(elem_type, lit_elem, lit_last)            \
  if (!inst) {                                                                 \
    return null;                                                               \
  }                                                                            \
                                                                               \
  String *str_empty = string("");                                              \
                                                                               \
  String *const final_prefix = prefix ? prefix : str_empty;                    \
  String *const final_separator = separator ? separator : str_empty;           \
  String *const final_suffix = suffix ? suffix : str_empty;                    \
                                                                               \
  const llong arraylen = Length(Array(elem_type), inst);                       \
                                                                               \
  String *rtn = CopyOf(String, final_prefix);                                  \
  if (!rtn) {                                                                  \
    return null;                                                               \
  }                                                                            \
                                                                               \
  refeach (elem_type, elem, inst, {                                            \
    if (!elem) {                                                               \
      continue;                                                                \
    }                                                                          \
                                                                               \
    /* Skip inserting a separator for the last element. */                     \
    if (_refeach_idx_elem == arraylen - 1) {                                   \
      continue;                                                                \
    }                                                                          \
                                                                               \
    rtn = append(rtn, lit_elem, final_separator);                              \
  })                                                                           \
                                                                               \
  rtn = append(                                                                \
    rtn,                                                                       \
    lit_last,                                                                  \
    final_suffix                                                               \
  );                                                                           \
                                                                               \
  Delete(String, str_empty);                                                   \
                                                                               \
  return rtn;

# define FUNC_ARRAY_BASICTYPE_LITERALISE(elem_type)                            \
  String *elem_type##Array_Literalise(_ARRAY_LITERALISE_PARAMS(elem_type));

# define IMPL_ARRAY_BASICTYPE_LITERALISE(elem_type, format_str)                \
String *elem_type##Array_Literalise(_ARRAY_LITERALISE_PARAMS(elem_type))       \
{                                                                              \
  if (!inst) {                                                                 \
    return null;                                                               \
  }                                                                            \
                                                                               \
  String *str_empty = string("");                                              \
  String *const final_prefix = prefix ? prefix : str_empty;                    \
  String *const final_separator = separator ? separator : str_empty;           \
  String *const final_suffix = suffix ? suffix : str_empty;                    \
                                                                               \
  const llong arraylen = Length(Array(elem_type), inst);                       \
  String *rtn = CopyOf(String, final_prefix);                                  \
  if (!rtn) {                                                                  \
    return null;                                                               \
  }                                                                            \
                                                                               \
  for (register llong i = 0; i < arraylen; i++) {                              \
    elem_type *elem = ref(Array(elem_type), inst, i);                          \
    if (!elem) {                                                               \
      continue;                                                                \
    }                                                                          \
                                                                               \
    /* format() returns a new string which append() takes ownership of. */     \
    String *val_str = format(format_str, *elem);                               \
                                                                               \
    /* Use CopyOf to avoid double-free during append's erase(). */             \
    if (i == arraylen - 1) {                                                   \
      rtn = append(rtn, val_str, CopyOf(String, final_suffix));                \
    } else {                                                                   \
      rtn = append(rtn, val_str, CopyOf(String, final_separator));             \
    }                                                                          \
  }                                                                            \
                                                                               \
  if (arraylen == 0) {                                                         \
    rtn = append(rtn, CopyOf(String, final_suffix));                           \
  }                                                                            \
                                                                               \
  Delete(String, str_empty);                                                   \
                                                                               \
  return rtn;                                                                  \
}

# define FUNC_ARRAY_LITERALISE(elem_type)                                      \
String *elem_type##Array_Literalise(_ARRAY_LITERALISE_PARAMS(elem_type));

# define FUNC_ARRAY_LITERALISE_CONFIGS(elem_type, ...)                         \
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
);

# define IMPL_ARRAY_LITERALISE(elem_type)                                      \
String *elem_type##Array_Literalise(_ARRAY_LITERALISE_PARAMS(elem_type))       \
{                                                                              \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem),                                                      \
    lit(elem_type, ref(Array(elem_type), inst, -1))                            \
  )                                                                            \
}

# define IMPL_ARRAY_LITERALISE_CONFIGS(...)                                    \
  CONCAT(IMPL_ARRAY_LITERALISE_CONFIGS_, arglen(__VA_ARGS__))(__VA_ARGS__)

/* 1 Config Parameter (3 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_3(elem_type, var1, ...)                 \
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1),                                                \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1)                      \
  )                                                                            \
}

/* 2 Config Parameters (5 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_5(elem_type, var1, var2, ...)           \
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1, var2),                                          \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1, var2)                \
  )                                                                            \
}

/* 3 Config Parameters (7 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_7(elem_type, var1, var2, var3, ...)     \
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1, var2, var3),                                    \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1, var2, var3)          \
  )                                                                            \
}

/* 4 Config Parameters (9 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_9(elem_type, var1, var2, var3, var4, ...)\
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1, var2, var3, var4),                              \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1, var2, var3, var4)    \
  )                                                                            \
}

/* 5 Config Parameters (11 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_11(elem_type, var1, var2, var3, var4, var5, ...)\
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1, var2, var3, var4, var5),                        \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1, var2, var3, var4, var5)\
  )                                                                            \
}

/* 6 Config Parameters (13 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_13(elem_type, var1, var2, var3, var4, var5, var6, ...)\
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1, var2, var3, var4, var5, var6),                  \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1, var2, var3, var4, var5, var6)\
  )                                                                            \
}

/* 7 Config Parameters (15 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_15(elem_type, var1, var2, var3, var4, var5, var6, var7, ...)\
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1, var2, var3, var4, var5, var6, var7),                  \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1, var2, var3, var4, var5, var6, var7)\
  )                                                                            \
}

/* 8 Config Parameters (17 Total Arguments) */
# define IMPL_ARRAY_LITERALISE_CONFIGS_17(elem_type, var1, var2, var3, var4, var5, var6, var7, var8, ...)\
String *elem_type##Array_Literalise(                                           \
  _ARRAY_LITERALISE_PARAMS(elem_type),                                         \
  __VA_ARGS__                                                                  \
) {                                                                            \
  _IMPL_ARRAY_LITERALISE_BODY(                                                 \
    elem_type,                                                                 \
    lit(elem_type, elem, var1, var2, var3, var4, var5, var6, var7, var8),      \
    lit(elem_type, ref(Array(elem_type), inst, -1), var1, var2, var3, var4, var5, var6, var7, var8)\
  )                                                                            \
}

#endif  /* COMPOUND_LITERALISE_H */
