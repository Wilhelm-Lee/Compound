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

#ifndef COMPOUND_CATLOG_H
# define COMPOUND_CATLOG_H

# include <stdarg.h>

# include <Compound/common.h>
# include <Compound/stacks.h>
# include <Compound/status.h>
# include <Compound/types.h>

# define CATLOG_MESSAGE_CONTENT_LENGTH_MAXIMUM  1024
# define CATLOG_MESSAGE_INITIATOR_LENGTH_MAXIMUM  64
//                                      time lvl  init
# define CATLOG_MESSAGE_HEADER_FORMAT  "[%s] (%s) @%s:\t"

typedef enum {
  CATLOG_LEVEL_ALL,  // Least the value, most the information.
  CATLOG_LEVEL_INFO,
  CATLOG_LEVEL_WARN,
  CATLOG_LEVEL_ERROR,
  CATLOG_LEVEL_FATAL,
  CATLOG_LEVEL_DEBUG,
  CATLOG_LEVEL_OFF,
  CATLOG_LEVEL_TRACE
} CatlogLevel;

/* Converting CatlogLevel into an identical char string. */
static inline const char *strlvl(const CatlogLevel lvl)
{
  switch (lvl) {
  case CATLOG_LEVEL_ALL:
    return "  ALL";
  case CATLOG_LEVEL_DEBUG:
    return "DEBUG";
  case CATLOG_LEVEL_INFO:
    return " INFO";
  case CATLOG_LEVEL_WARN:
    return " WARN";
  case CATLOG_LEVEL_ERROR:
    return "ERROR";
  case CATLOG_LEVEL_FATAL:
    return "FATAL";
  case CATLOG_LEVEL_OFF:
    return "  OFF";
  case CATLOG_LEVEL_TRACE:
  default:
    return "TRACE";
  }
}

typedef struct {
  time_t time;
  CatlogLevel level;
  char initiator[CATLOG_MESSAGE_INITIATOR_LENGTH_MAXIMUM];
  char content[CATLOG_MESSAGE_CONTENT_LENGTH_MAXIMUM];
} CatlogMessage;

Status CatlogMessage_Create(CatlogMessage *inst, const CatlogLevel level,
                            const char *initiator, const char *fmt, ...);
Status CatlogMessage_CopyOf(CatlogMessage *inst, const CatlogMessage other);
boolean CatlogMessage_Equals(const CatlogMessage msg1, const CatlogMessage msg2);

typedef struct {
  CatlogMessage msg;
  FILE *dst;
  boolean successful;
  struct timespec elapsed;
  CatlogLevel filter;
} CatlogSender;

static CatlogSender *GlobalCatlogSender = NULL;

static inline void CatlogSender_SetGlobal(CatlogSender *inst)
{
  if (!inst)  return;

  GlobalCatlogSender = inst;
}

/* Shorthand for declaring CatlogSender @cs in function parameters.
   "CATS" stands for "Catlog Sender". */
# define CATS  CatlogSender *cs

Status CatlogSender_Create(CatlogSender *inst, const CatlogMessage msg,
                           FILE *dst);
Status CatlogSender_CopyOf(CatlogSender *inst, const CatlogSender other);
Status CatlogSender_Delete(CatlogSender *inst);
boolean CatlogSender_Equals(const CatlogSender sender1,
                           const CatlogSender sender2);
Status CatlogSender_Send(const CatlogSender sender);
Status cat(CatlogSender *cs, const CatlogLevel lvl, const char *initiator,
           const char *fmt, ...);

/*
  CATLOG_LEVEL_ALL,  // Least the value, most the information.
  CATLOG_LEVEL_INFO,
  CATLOG_LEVEL_WARN,
  CATLOG_LEVEL_ERROR,
  CATLOG_LEVEL_FATAL,
  CATLOG_LEVEL_DEBUG,
  CATLOG_LEVEL_OFF,
  CATLOG_LEVEL_TRACE
*/

/* Preset cats. */
# define all(fmt, ...)  cat(cs, CATLOG_LEVEL_ALL, __func__, fmt, __VA_ARGS__)
# define info(fmt, ...)  cat(cs, CATLOG_LEVEL_INFO, __func__, fmt, __VA_ARGS__)
# define warning(fmt, ...)  cat(cs, CATLOG_LEVEL_WARNING, __func__, fmt, __VA_ARGS__)
# define error(fmt, ...)  cat(cs, CATLOG_LEVEL_DEBUG, __func__, fmt, __VA_ARGS__)
# define fatal(fmt, ...)  cat(cs, CATLOG_LEVEL_DEBUG, __func__, fmt, __VA_ARGS__)
# define debug(fmt, ...)  cat(cs, CATLOG_LEVEL_DEBUG, __func__, fmt, __VA_ARGS__)
# define off(fmt, ...)  cat(cs, CATLOG_LEVEL_DEBUG, __func__, fmt, __VA_ARGS__)
# define trace(fmt, ...)  cat(cs, CATLOG_LEVEL_DEBUG, __func__, fmt, __VA_ARGS__)

# define redir(file)  cs->dst = (file)
# define filter(lvl)  cs->filter = (lvl)

STATUS(CatCannotSendMessage, 1,
          "Cat had trouble while sending your message.",
          STATUS_ERROR, &ReadWriteError);

STATUS(CatMessageContentTooLong, 1,
          "The message content for logging is too long.",
          STATUS_ERROR, &MaximumLengthExceeded);

STATUS(CatMessageInitiatorTooLong, 1,
          "The message initiator for logging is too long.",
          STATUS_ERROR, &MaximumLengthExceeded);

STATUS(CatCannotFindFile, 1,
          "Cat cannot have your file found.",
          STATUS_ERROR, &FileNotFound);

STATUS(CatHasInsufficientPermission, 1,
          "Cat cannot access your file with its given permission.",
          STATUS_ERROR, &InsufficientAccessPermission);

STATUS(CatCannotOpenFile, 1,
          "Cat cannot open your file because fopen returned NULL.",
          STATUS_ERROR, &ReadWriteError);

STATUS(CatMessageSuppressed, 0,
          "The message is suppressed.",
          STATUS_NORMAL, &NormalStatus);

#endif  /* COMPOUND_CATLOG_H */
