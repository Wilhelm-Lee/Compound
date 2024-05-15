#include <Compound/array.h>

Status Array_Create(Array *inst, int len)
{
  /* Skip unavailable inst and invalid param. */
  fails(inst, UnavailableInstance);
  state((len < 0), InvalidArrayLength);
  solve((!len), { inst->len = 0; inst->members = NULL; return NormalStatus; })
  
  
}

Status Array_CopyOf(Array *inst, Array *other)
{
  /* Skip unavailable inst and invalid param. */
  fails(inst, UnavailableInstance);
  
}

Status Array_Delete(Array *inst)
{
  /* Skip unavailable inst and invalid param. */
  fails(inst, UnavailableInstance);
  
}

Status Array_GetIdx(Array *inst, Var *store, int index)
{
  /* Skip unavailable inst and invalid param. */
  fails(inst, UnavailableInstance);
  
}

Status Array_SetIdx(Array *inst, Var *source, int index)
{
  /* Skip unavailable inst and invalid param. */
  fails(inst, UnavailableInstance);
  
}

Status Array_Literalise(Array *inst, char const *store)
{
  /* Skip unavailable inst and invalid param. */
  fails(inst, UnavailableInstance);
  
}

bool   Array_Equal(Array *arr1, Array *arr2)
{
  /* Skip unavailable inst and invalid param. */
  state((arr1 == NULL || arr2 == NULL), false);
  
}




// Status Array_Create(Array *inst, int len)
// {
//   fails(inst, UnavailableInstance);
//   state(len < 0, InvalidArrayLength);
  
//   Var arr[len] = {};
//   *inst = (Array) {
//     .len = len,
//     .members = arr
//   };
  
//   /* Initialise members. */
//   for (register int i = 0; i < inst->len; i++) {
//     state(!(StatusUtils_IsOkay(Var_Create(&inst->members[i], NULL, 0, NULL))),
//       error(RuntimeError,
//             "Failed initialisations on var_create for array_create."));
//   }
  
//   return NormalStatus;
// }

// Status Array_Delete(Array *inst);

// bool ArrayUtils_IsEmpty(Array *inst);

// bool Array_Equal(Array *arr1, Array *arr2);
