#include <Compound/var.h>

Status Var_Create(Var *inst, size_t size)
{
  *inst = (Var) {
    .addr = malloc(size),
    .size = size
  };
  
  if (inst->addr == NULL) {
    return InsufficientMemory;
  }
  
  return NormalStatus;
}

// Status Var_Create(Var *inst, void *addr, size_t size, char *identity)
// {
//   /* Skip when inst is unavailable. */
//   fails(inst, UnavailableInstance);
//   /* Skip when identity is unavailable. */
//   fails(identity, NullPointerAccounted);
//   /* Skip when identity does not pass the examine. */
//   state(!VarUtils_IsIdentityLegal(identity), IllegalVarIdentity);
    
//   inst->addr = addr;
//   inst->size = size;
//   *inst->identity = *identity;
  
//   return NormalStatus;
// }

Status Var_CopyOf(Var *inst, Var *other)
{
  /* Skip when inst or other is unavailable. */
  fails(inst, UnavailableInstance);
  fails(other, InvalidParameter);
  
  /* Copy members from other.  Only has to apply size, no addr is needed. */
  inst->addr = malloc(other->size);
  inst->size = other->size;
  
  return NormalStatus;
}

void Var_Delete(Var *inst)
{
  if (inst == NULL)  return;
  
  free(inst->addr);
  inst->addr = NULL;
  inst->size = 0;
}

// void Var_Delete(Var *inst)
// {
//   /* Skip when inst or inst->addr is unavailable. */
//   svoid(inst == NULL || inst->addr == NULL);

//   inst->addr = NULL;
//   inst->size = 0;
//   *inst->identity = 0;
// }

void VarUtils_Swap(Var *v1, Var *v2)
{
  /* Skip when v1 or v2 is unavailable. */
  svoid(v1 == NULL || v2 == NULL);
  
  Var v3 = *v1;
  *v1 = *v2;
  *v2 = v3;
}

Status Var_Literalise(Var *inst, char *buff)
{
  /* Skip when inst is unavailable. */
  state(inst == NULL, UnavailableInstance);
  
  /* Write into buffer. */
  state(!sprintf(buff, VAR_LITERALISE_FORMAT"\n", inst->addr, inst->size),
        error(RuntimeError, "Sprintf returned 0 where it should never do."));
  
  return NormalStatus;
}

bool Var_Equals(Var *a, Var *b)
{
  /* Skip unavailable inst and invalid param. */
  state((a == NULL || b == NULL), false);
  
  return (a->addr == b->addr && a->size == b->size);
}

// bool VarUtils_IsIdentityLegal(char *identity)
// {
//   /* Skip when identity is unavailable. */
//   state(identity == NULL, false);
  
//   const int len = strlen(identity);
  
//   /* Skip when identity is empty. */
//   state(len == 0, false);

//   /* Skip when the first char is not within alphabet. */
//   state(ATRANGE('a', 'z', identity[0])
//         || ATRANGE('A', 'Z', identity[0]), false);
  
//   /* Skip when the length of identity is greater that VAR_IDENTITY_LENGTH. */
//   state(len > VAR_IDENTITY_LENGTH, false);
  
//   /* Skip when identity has space and illegal characters in it. */
//   const int illegal_len = strlen(VAR_IDENTITY_ILLEGAL_CHAR);
//   for (register int i = 0; i < len; i++) {
//     for (register int j = 0; j < illegal_len; j++) {
//       if (identity[i] == VAR_IDENTITY_ILLEGAL_CHAR[j]) {
//         return false;
//       }
//     }
//   }
  
//   return true;
// }
