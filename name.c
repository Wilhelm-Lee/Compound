#include <Compound/name.h>

Status NameSpace_Create(NameSpace *inst)
{
  fails(inst, UnavailableInstance);
  
  /* Create instances for members from inst. */
  state(status_isokay(NameSpace_EmptyName(&inst->latest)),
        error(RuntimeError, "Failed to initialise latest from NameSpace"));
  state(status_isokay(NameSpace_EmptyName(&inst->idx)),
        error(RuntimeError, "Failed to initialise idx from NameSpace"));
  state(status_isokay(NameSpace_EmptyName(inst->occupied)),
        error(RuntimeError, "Failed to initialise occupied from NameSpace"));
  
  return NormalStatus;
}

Status NameSpace_CopyOf(NameSpace *inst, NameSpace *other)
{
  fails(inst, UnavailableInstance);
  fails(other, UnavailableParameter);
  
  /* Copy and assign. */
  other->latest = inst->latest;
  other->idx = inst->idx;
  
  const Name len = NameSpace_CalcNameArrayLength(&other->occupied);
  for (Name i = (Name){0}; (NameSpace_CompareName(i, len) < 0);) {

    // TODO(william):  HERE
    
    /* i++ */
    state((!status_isokay(NameSpace_CountUp(&i))),
          error(RuntimeError, "Error occurred during calculations of Name."));
  }
}

Status NameSpace_CreateName(NameSpace *inst, Name *buff);

Status NameSpace_RemoveName(NameSpace *inst, Name idx);

Status NameSpace_EmptyName(Name *inst);

Status NameSpace_CountUp(Name *inst);

Status NameSpace_CountDown(Name *inst);

Status NameSpace_CountUpFor(Name *inst, Name amount);

Status NameSpace_CountDownFor(Name *inst, Name amount);

Status NameSpace_UpdateLatest(NameSpace *inst, Name idx);

Status NameSpace_FormatTrim(Name *inst);

Status NameSpace_FormatInflate(Name *inst);

Name NameSpace_CalcNameArrayLength(Name **arr);

bool NameSpace_IsAvailable(NameSpace *inst, Name idx);

bool NameSpace_IsValidName(Name *inst);

int NameSpace_CompareName(Name *a, Name *b)
{
  /* Validation comes the first.  --William */
  if (!NameSpace_IsValidName(a) || !NameSpace_IsValidName(b)) {
    Report e; ArgueStartParam h;
    throw(stamp(&InvalidName));
  }
}
