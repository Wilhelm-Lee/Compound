#include <Compound/string.h>

Status String_Create(String *inst, int len)
{
  Status create = Array_Create(inst, len);
  solve(!(StatusUtils_IsOkay(create)), {
    Report e = stamp(error(create, "Failed to create a string."),
                     nameof(Array_Create));
    (void)_throw(e);
    return ReportThrown;
  })
  
  return NormalStatus;
}

Status String_CopyOf(String *inst, String *other)
{
  fails(inst, UnavailableInstance);
}

Status String_Delete(String *inst)
{
  fails(inst, UnavailableInstance);
}

Status String_GetIdx(String *inst, Char *store, int index)
{
  fails(inst, UnavailableInstance);
}

Status String_SetIdx(String *inst, Char *source, int index)
{
  fails(inst, UnavailableInstance);
}

Status String_Literalise(String *inst, String *store)
{
  fails(inst, UnavailableInstance);
}

bool   String_Equals(String *arr1, String *arr2)
{
  fails(inst, UnavailableInstance);
}

