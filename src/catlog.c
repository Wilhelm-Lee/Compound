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

#include "../inc/catlog.h"

Status CatlogMessage_Create(CatlogMessage *inst, const CatlogLevel level,
                            const char *initiator, const char *fmt, ...)
{
  avail(inst);
  state((!initiator), (UnavailableBuffer));
  state(strlen(initiator) > CATLOG_MESSAGE_INITIATOR_LENGTH_MAXIMUM,
        CatMessageInitiatorTooLong);

  inst->time = time(NULL);
  inst->level = level;
  (void)strncpy(inst->initiator, initiator,
                CATLOG_MESSAGE_INITIATOR_LENGTH_MAXIMUM);

  va_list ap;
  va_start(ap, fmt);
  const int rtncode =
    vsnprintf(inst->content, CATLOG_MESSAGE_CONTENT_LENGTH_MAXIMUM, fmt, ap);
  va_end(ap);

  if (rtncode < 0) {
    RETURN(value(annot(RuntimeError, "Failed on "nameof(vsnprintf)".  "
                       "Value preserved."), rtncode));
  }

  RETURN(NormalStatus);
}

Status CatlogMessage_CopyOf(CatlogMessage *inst, const CatlogMessage other)
{
  avail(inst);

  inst->time = other.time;
  inst->level = other.level;
  (void)strncpy(inst->initiator, other.initiator,
                CATLOG_MESSAGE_INITIATOR_LENGTH_MAXIMUM);
  (void)strncpy(inst->content, other.content,
                CATLOG_MESSAGE_CONTENT_LENGTH_MAXIMUM);

  RETURN(NormalStatus);
}

bool CatlogMessage_Equals(const CatlogMessage msg1, const CatlogMessage msg2)
{
  return (msg1.time == msg2.time && msg1.level == msg2.level &&
          (!strcmp(msg1.initiator, msg2.initiator)) &&
          (!strcmp(msg1.content, msg2.content)));
}

Status CatlogSender_Create(CatlogSender *inst, const CatlogMessage msg,
                           FILE *dst)
{
  nonull(inst, (UnavailableInstance));

  /* Copy and assign. */
  inst->msg = msg;
  inst->dst = (!dst ? stdout : dst);
  inst->successful = false;
  inst->elapsed = (struct timespec)EMPTY;

  RETURN(NormalStatus);
}

Status CatlogSender_CopyOf(CatlogSender *inst, const CatlogSender other)
{
  nonull(inst, (UnavailableInstance));

  /* Copy and assign */
  inst->msg = other.msg;
  inst->dst = other.dst;
  inst->successful = other.successful;
  inst->elapsed = other.elapsed;

  RETURN(NormalStatus);
}

Status CatlogSender_Delete(CatlogSender *inst)
{
  avail(inst);

  inst->msg = (CatlogMessage)EMPTY;
  inst->dst = NULL;
  inst->successful = false;
  inst->elapsed = (struct timespec)EMPTY;

  RETURN(NormalStatus);
}

bool CatlogSender_Equals(const CatlogSender sender1, const CatlogSender sender2)
{
  return (CatlogMessage_Equals(sender1.msg, sender2.msg) &&
          sender1.dst == sender2.dst &&
          sender1.successful == sender2.successful &&
          ((sender1.elapsed.tv_sec == sender2.elapsed.tv_sec) &&
           (sender1.elapsed.tv_nsec == sender2.elapsed.tv_nsec)));
}

Status CatlogSender_Send(const CatlogSender sender)
{
  /* Filtering. */
  state(sender.msg.level < sender.filter, (CatMessageSuppressed));

  /* Literalise time. */
  char tmbuff[30] = EMPTY;
  zero(strftime(tmbuff, CATLOG_MESSAGE_CONTENT_LENGTH_MAXIMUM, "%c",
                localtime(&sender.msg.time)),
       RETURN(annot(NoBytesWereWritten,
                    "Failed to write formatted time into buffer for CatLog.")));

  /* Literalise header. */
  char header_buffer[CATLOG_MESSAGE_CONTENT_LENGTH_MAXIMUM] = EMPTY;
  zero(sprintf(header_buffer, CATLOG_MESSAGE_HEADER_FORMAT, tmbuff,
               strlvl(sender.msg.level), sender.msg.initiator),
       RETURN(annot(NoBytesWereWritten,
                    "Failed to write content into buffer for CatLog.")));

  const size_t header_buffer_len = strlen(header_buffer);

  /* Tokenise with @NEWLINE. */
  char *tok = strtok(strptr(sender.msg.content), NEWLINE);
  while (tok) {
    zero(
      fwrite(header_buffer, sizeof(header_buffer[0]), header_buffer_len,
             sender.dst),
      RETURN(annot(ReadWriteError,
                   "No bytes were written into buffer for header_buffer.")););
    zero(fwrite(tok, sizeof(sender.msg.content[0]), strlen(sender.msg.content),
                sender.dst),
         RETURN(annot(ReadWriteError,
                      "No bytes were written into buffer for content.")););
    zero(
      fwrite(NEWLINE, sizeof(NEWLINE), strlen(NEWLINE), sender.dst),
      RETURN(annot(
               ReadWriteError,
               "No bytes were written into buffer for line wrapping character for "
               "content.")););
    tok = strtok(NULL, NEWLINE);
  }

  RETURN(NormalStatus);
}

Status cat(CatlogSender *cs, const CatlogLevel lvl, const char *initiator,
           const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  Status create = CatlogMessage_Create(&cs->msg, lvl, initiator, fmt, ap);
  va_end(ap);

  notok(create, {
    RETURN(_);
  })

  /* Send out the message. */
  fail(CatlogSender_Send(*cs));

  RETURN(NormalStatus);
}
