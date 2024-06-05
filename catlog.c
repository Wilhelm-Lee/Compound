#include <Compound/catlog.h>
#include <Compound/status.h>

Status CatlogMsg_Create(CatlogMsg *inst, CatlogLevel level,
       char *initiator, char *msg)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, UnavailableInstance);
  state((initiator == NULL || msg == NULL), InvalidParameter);

	inst->time = time(NULL);
	inst->level = level;
  inst->initiator = initiator;
  inst->content = msg;

	return NormalStatus;
}

Status CatlogMsg_CopyOf(CatlogMsg *inst, CatlogMsg *other)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, UnavailableInstance);
  fails(other, InvalidParameter);

	*inst = *other;

	return NormalStatus;
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
  fails(inst, UnavailableInstance);
  fails(msg, InvalidParameter);

  /* Copy and assign, with detections. */
  inst->msg = *msg;
  inst->dst = (dst == NULL ? (stdout) : dst);
  inst->successful = false;
  inst->elapsed = (struct timespec){.tv_sec = 0, .tv_nsec = 0};

  return NormalStatus;
}

Status CatlogSender_CopyOf(CatlogSender *inst, CatlogSender *other)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, UnavailableInstance);
  fails(other, InvalidParameter);

  /* Copy and assign */
  inst->msg = other->msg;
  inst->dst = other->dst;
  inst->successful = other->successful;
  inst->elapsed = other->elapsed;

  return NormalStatus;
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

Status CatlogSender_Send(CatlogSender *inst, char *filepath, bool append)
       throws(ReadWriteError)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, UnavailableInstance);
  fails(filepath, UnavailableFileName);

  /* Open file. */
  // ensure(CatlogUtils_OpenFile(inst->dst, (append ? "a" : "w")),
  //        "Unable to open file.");
  (void)CatlogUtils_OpenFile(inst->dst, filepath, (append ? "a" : "w"));

  /* Write msg. */
  return unknown(NormalStatus, "", !fprintf(inst->dst, "%s", inst->msg.content));
}

Status CatlogUtils_CalcElapsed(struct timespec t1, struct timespec t2);

Status CatlogUtils_OpenFile(FILE *store, char *filepath,
                            const char const *__restrict mode)
{
  /* No need to open a system output stream. */
  if (!strcmp(filepath, "stdin") ||
      !strcmp(filepath, "stdout") ||
      !strcmp(filepath, "stderr")) {
    return NormalStatus;
  }
  
  store = fopen(filepath, mode);
  
  return NormalStatus;
}
