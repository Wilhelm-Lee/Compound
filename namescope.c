#include <Compound/namescope.h>

Status NameScope_Create(NameScope *inst)
{
  fails(inst, UnavailableInstance);
  
  /* Create instances for members from inst. */
  state(StatusUtils_IsOkay(NameScope_EmptyName(&inst->latest)),
        error(RuntimeError, "Failed to initialise latest from NameScope"));
  state(StatusUtils_IsOkay(NameScope_EmptyName(&inst->idx)),
        error(RuntimeError, "Failed to initialise idx from NameScope"));
  state(StatusUtils_IsOkay(NameScope_EmptyName(inst->occupied)),
        error(RuntimeError, "Failed to initialise occupied from NameScope"));
  
  return NormalStatus;
}

Status NameScope_CopyOf(NameScope *inst, NameScope *other)
{
  fails(inst, UnavailableInstance);
  fails(other, UnavailableParameter);
  
  /* Copy and assign. */
  other->latest = inst->latest;
  other->idx = inst->idx;
  
  const Name len = NameScope_CalcNameArrayLength(&other->occupied);
  for (Name i = (Name){0}; (NameScope_CompareName(i, len) < 0);) {

    // TODO(william):  HERE
    
    /* i++ */
    state((!StatusUtils_IsOkay(NameScope_CountUp(&i))),
          error(RuntimeError, "Error occurred during calculations of Name."));
  }
}

Status NameScope_CreateName(NameScope *inst, Name *buff);

Status NameScope_RemoveName(NameScope *inst, Name idx);

Status NameScope_EmptyName(Name *inst);

Status NameScope_CountUp(Name *inst);

Status NameScope_CountDown(Name *inst);

Status NameScope_CountUpFor(Name *inst, Name amount);

Status NameScope_CountDownFor(Name *inst, Name amount);

Status NameScope_UpdateLatest(NameScope *inst, Name idx);

Status NameScope_FormatTrim(Name *inst);

Status NameScope_FormatInflate(Name *inst);

Name NameScope_CalcNameArrayLength(Name **arr);

bool NameScope_IsAvailable(NameScope *inst, Name idx);

bool NameScope_IsValidName(Name *inst);

int NameScope_CompareName(Name *a, Name *b)
{
  /* Validation comes the first.  --William */
  if (!NameScope_IsValidName(a) || !NameScope_IsValidName(b)) {
    Report e; ArgueStartParam h;
    throw(stamp(&InvalidName));
  }
}
