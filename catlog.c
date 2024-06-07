#include "Status/include/status.h"
#include <Compound/common.h>
#include <Compound/status.h>
#include <Compound/catlog.h>

Status CatlogMsg_Create(CatlogMsg *inst, CatlogLevel level,
       char *initiator, char *msg)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, apply(UnavailableInstance));
  state((initiator == NULL || msg == NULL), apply(InvalidParameter));

	inst->time = time(NULL);
	inst->level = level;
  inst->initiator = initiator;
  inst->content = msg;

	return apply(NormalStatus);
}

Status CatlogMsg_CopyOf(CatlogMsg *inst, CatlogMsg *other)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, apply(UnavailableInstance));
  fails(other, apply(InvalidParameter));

	*inst = *other;

	return apply(NormalStatus);
}

bool CatlogMsg_Equals(CatlogMsg *inst, CatlogMsg *other)
{
  /* Skip unavailable instances and parameters. */
  state((!inst || other == NULL), false);

	return (
		inst->time == other->time &&
		inst->level == other->level &&
		(!strcmp(inst->initiator, other->initiator)) &&
		(!strcmp(inst->content, other->content))
	);
}

Status CatlogSender_Create(CatlogSender *inst, CatlogMsg *msg, FILE *dst)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, apply(UnavailableInstance));
  fails(msg, apply(InvalidParameter));

  /* Copy and assign. */
  inst->msg = *msg;
  inst->dst = (!dst ? stdout : dst);
  inst->successful = false;
  inst->elapsed = (struct timespec){ .tv_sec = 0, .tv_nsec = 0 };

  return apply(NormalStatus);
}

Status CatlogSender_CopyOf(CatlogSender *inst, CatlogSender *other)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, apply(UnavailableInstance));
  fails(other, apply(InvalidParameter));

  /* Copy and assign */
  inst->msg = other->msg;
  inst->dst = other->dst;
  inst->successful = other->successful;
  inst->elapsed = other->elapsed;

  return apply(NormalStatus);
}

bool CatlogSender_Equals(CatlogSender *inst, CatlogSender *other)
{
  /* Skip unavailable instances and parameters. */
  state((!inst || other == NULL), false);

  return (
    CatlogMsg_Equals(&inst->msg, &other->msg) &&
    inst->dst == other->dst &&
    inst->successful == other->successful &&
    ((inst->elapsed.tv_sec == other->elapsed.tv_sec) &&
    (inst->elapsed.tv_nsec == other->elapsed.tv_nsec))
  );
}

Status CatlogSender_Send(CatlogSender *inst)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, apply(UnavailableInstance));

  const int written = fprintf(inst->dst, "%s", inst->msg.content);

  /* Write msg. */
  state(!written, error(ReadWriteError, "No bytes were written into buffer."));

  return apply(NormalStatus);
}

Status CatlogUtils_OpenFile(FILE **fileptr, const char *filepath,
                            const char const *restrict mode)
{
  /* Skip unavailable instances and parameters. */
  fails(fileptr, apply(UnavailableBuffer));
  fails(filepath, apply(UnavailableFileName));
  fails(mode, apply(UnavailableFileAccessMode));

  /* Open the file.  Return CatCannotOpenFile once failed. */
  state(!(*fileptr = fopen(filepath, mode)), apply(CatCannotOpenFile));

  return apply(NormalStatus);
}

Status CatlogUtils_CloseFile(FILE **fileptr)
{
  /* Skip if either the fileptr or the *fileptr is unavailable. */
  state(!fileptr || !*fileptr, apply(UnavailableParameter));

  /* Return returning code of fclose, sealed with "value". */
  return apply(value(UnknownStatus, fclose(*fileptr)));
}
