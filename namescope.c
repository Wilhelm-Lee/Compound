#include <Compound/namescope.h>

Status NameScope_Create(NameScope *inst)
{
  nonull(inst, apply(UnavailableInstance));
  
  /* Create instances for members from inst. */
  state(StatusUtils_IsOkay(NameScope_EmptyName(&inst->latest)),
        apply(error(RuntimeError, "Failed to initialise latest from NameScope")));
  state(StatusUtils_IsOkay(NameScope_EmptyName(&inst->idx)),
        apply(error(RuntimeError, "Failed to initialise idx from NameScope")));
  state(StatusUtils_IsOkay(NameScope_EmptyName(inst->occupied)),
        apply(error(RuntimeError, "Failed to initialise occupied from NameScope")));
  
  return apply(NormalStatus);
}

Status NameScope_CopyOf(NameScope *inst, NameScope *other)
{
  nonull(inst, apply(UnavailableInstance));
  nonull(other, apply(UnavailableParameter));
  
  /* Copy and assign. */
  other->latest = inst->latest;
  other->idx = inst->idx;
  
  const Name len = {};

  NameScopeUtils_CalcNameArrayLength(&other->occupied);
  for (Name i = (Name){0}; (NameScope_CompareName(i, len) < 0);) {

    // TODO(william):  HERE
    
    /* i++ */
    state((!StatusUtils_IsOkay(NameScope_CountUp(&i))),
      apply(error(RuntimeError, "Error occurred during calculations of Name.")));
  }
}

Status NameScope_CreateName(NameScope *inst, Name *buff);

Status NameScope_RemoveName(NameScope *inst, Name idx);

Status NameScope_EmptyName(Name *inst);

Status NameScope_UpdateLatest(NameScope *inst, Name idx);

Status NameScope_FormatTrim(Name *inst);

Status NameScope_FormatInflate(Name *inst);

// Status NameScopeUtils_CalcNameArrayLength(Name **arr);

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
