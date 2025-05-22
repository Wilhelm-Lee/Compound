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

#ifndef COMPOUND_LITERALISATION_H
# define COMPOUND_LITERALISATION_H

# define LITERALISATION_LENGTH_MAXIMUM  4096
# define LITERALISATION_COUNTDOWN  65536

# define DEFINE_LITERALISATION(type)                       \
  typedef struct type##_lit_callback {                     \
    /* Offsetting to match buffer indexer's position. */   \
    size_t offset;                                         \
    size_t (*Literalise)(char *buff, const type object,    \
                         int countdown,                    \
                         struct type##_lit_callback *callback,\
                         const char *format, ...);         \
    /* The literalisation format.  e.g. "@%p+%lu". */      \
    char *format;                                          \
                                                           \
    /* The next callback of one deeper recursion. */       \
    struct LiteralisationCallback *next;                   \
  } type##_lit_callback;

// # define lit(buff, type, object, format, ...)              \
//   type##_Literalise((buff), (object),                      \
//                    LITERALISATION_COUNTDOWN,               \
//                    &type##_lit_callback,                   \
//                    format, __VA_ARGS__)

# define lit(buff, type, object)                           \
  type##_Literalise((buff), (object), LITERALISATION_COUNTDOWN,\
                    LITERALISATION_CALLBACK(type), /* YOU LEFT HERE */)

# define FUNC_LITERALISATION(type)                         \
  /* Uses @callback to process literalisation according to \
   specific requirements.                                  \
                                                           \
   This function terminates whenever (@countdown <= 0).    \
                                                           \
   @countdown updates by (-1) every time @Literalise       \
   is called. */                                           \
  size_t type##_Literalise(                                \
      char *buff, const type object, int countdown,        \
      struct type##_lit_callback *callback,                \
      const char *format, ...);                            \
  void type##Literalise_SetFormat(const char *format);

/* Get responding format of @type. */
# define LITERALISATION_FORMAT(type)                       \
  type##_LITERALISATION_FORMAT

/* Get responding callback of @type. (variable) */
# define LITERALISATION_CALLBACK(type)                     \
  type##_lit_callback

/* Get responding callback of @type. (type) */
# define LITERALISATION_CALLBACK_TYPE(type)                \
  type##LiteralisationCallback

# define DEFAULT_LITERALISATION_FORMAT  "%p"

# define LITERALISATION(type, fmt)                         \
  DEFINE_LITERALISATION(type)                              \
  FUNC_LITERALISATION(type)                                \
  static char *LITERALISATION_FORMAT(type) = (fmt);        \
  static LITERALISATION_CALLBACK_TYPE(type) LITERALISATION_CALLBACK(type) = {\
    .offset = 0,                                           \
    .Literalise = type##_Literalise,                       \
    .format = (fmt),                                       \
    .next = NULL                                           \
  };                                                       \

# define IMPL_LITERALISATION(type)                         \
  size_t type##_Literalise(char *buff, const type object,  \
                           int countdown,                  \
                           struct type##_lit_callback *callback,\
                           const char *format, ...)        \
  {                                                        \
    if (!buff || !object || countdown <= 0) {              \
      return 0;                                            \
    }                                                      \
                                                           \
    va_list ap;                                            \
    va_start(ap, format);                                  \
                                                           \
    size_t written = 0;                                    \
                                                           \
    /* Perform extra actions when @callback is provided. */\
    if (callback) {                                        \
      /* Continue from previous position in @buff. */      \
      written = callback->offset;                          \
                                                           \
      written += callback->Literalise(                     \
          buff, object, --countdown, callback, format      \
      );                                                   \
                                                           \
      /* Update @callback. */                              \
      callback->offset = written;                          \
                                                           \
      return written;                                      \
    }                                                      \
                                                           \
    /* Perform default action instead. */                  \
    return snprintf(buff, LITERALISATION_LENGTH_MAXIMUM,   \
                    DEFAULT_LITERALISATION_FORMAT, object);\
  }                                                        \
                                                           \
  void type##_SetFormat(const char *format)                \
  {                                                        \
    if (!format)  return;                                  \
                                                           \
    LITERALISATION_FORMAT(type) = (char *)format;          \
  }

#endif  /* COMPOUND_LITERALISATION_H */
