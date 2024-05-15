#include <Compound/catlog.h>
#include <Compound/status.h>

Status CatlogMsg_Create(CatlogMsg *inst, CatlogLevel level,
       char const *originator, char const *msg)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, UnavailableInstance);
  state((originator == NULL || msg == NULL), InvalidParameter);

	inst->time = time(NULL);
	inst->level = level;
	*inst->originator = *originator;
	*inst->content = *msg;

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

bool CatlogMsg_Equal(CatlogMsg *inst, CatlogMsg *other)
{
  /* Skip unavailable instances and parameters. */
  state((inst == NULL || other == NULL), false);

	return (
		inst->time == other->time &&
		inst->level == other->level &&
		(!strcmp(inst->originator, other->originator)) &&
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

bool CatlogSender_Equal(CatlogSender *inst, CatlogSender *other)
{
  /* Skip unavailable instances and parameters. */
  state((inst == NULL || other == NULL), false);

  return (
    CatlogMsg_Equal(&inst->msg, &other->msg) &&
    inst->dst == other->dst &&
    inst->successful == other->successful &&
    ((inst->elapsed.tv_sec == other->elapsed.tv_sec) &&
    (inst->elapsed.tv_nsec == other->elapsed.tv_nsec))
  );
}

Status CatlogSender_Send(CatlogSender *inst, int *store, bool append)
       throws(ReadWriteError)
{
  /* Skip unavailable instances and parameters. */
  fails(inst, UnavailableInstance);
  fails(store, InvalidParameter);

  /* Open file. */
  ensure(CatlogUtils_OpenFile(inst->dst, (append ? "a" : "w")),
         "Unable to open file.");

  /* Write msg. */
  *store = fprintf(inst->dst, "%s", inst->msg.content);
  
  return NormalStatus;
}

Status CatlogUtils_CalcElapsed(struct timespec t1, struct timespec t2);

Status CatlogUtils_OpenFile(FILE *store, const char const *__restrict mode);
