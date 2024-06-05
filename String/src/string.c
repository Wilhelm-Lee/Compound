#include <Compound/string.h>

Status String_Create(String *inst, int len)
{
  /* Create an array has length len + 1, for termination character in string. */  
  ensure(Array_Create(inst, len + 1, sizeof(int)), "Failed to create a string.");
 
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

Status String_GetAt(String *inst, Char *store, int index)
{
  fails(inst, UnavailableInstance);
}

Status String_SetAt(String *inst, Char *source, int index)
{
  fails(inst, UnavailableInstance);
}

Status String_Literalise(String *inst, String *store)
{
  fails(inst, UnavailableInstance);
}

int StringUtils_Compare(String *a, String *b)
{
  /* Both being null is counted as equal. */
  state(a == NULL && b == NULL, 0);

  /* Compare by iterating through each element to calculate differing
     values between a[i] and b[i]. */  
  const int len = max(a->len, b->len);
  for (register int i = 0; i < len; i++) {
    const int comp =
        (*(char *)(a->members[i].addr) - *(char *)(b->members[i].addr));

    /*
      If values from both side does not equal, return the differential value.
    */
    if (!comp) {
      return comp;
    }
  }
  
  /* Even the previous elements are same, but the length must be compared
     because we haven't really done this.
     The reason that we don't compare it earlier is though we can know whether
     the lengths are the same or not, we, however, cannot give a precise value
     telling which content from string is "greater" or "less". */
  return (a->len != b->len);
}
