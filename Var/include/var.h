#ifndef COMPOUND_VAR
# define COMPOUND_VAR

# include <stdio.h>

# include <Compound/common.h>
# include <Compound/status.h>
# include <Compound/catlog.h>

// # define VAR_IDENTITY_LENGTH    64
// # define VAR_LITERALISE_LENGTH  (VAR_IDENTITY_LENGTH + 16 + 9 + 10)
# define VAR_LITERALISE_LENGTH  (16 + 9 + 10)
// # define VAR_LITERALISE_FORMAT  ("%s  @[%p]: %ld")
# define VAR_LITERALISE_FORMAT  "@[%p]: %ld"
// # define VAR_IDENTITY_ILLEGAL_CHAR  "!@#$%^*()-=+;\'\"\\|,./<>?[]{}`~ "

// static Status IllegalVarIdentity = {
//   .value = 1,
//   .description = "Given identity does not fit the standard of Var Naming "
//                  "convention.",
//   .characteristic = STATUS_ERROR,
//   .prev = &InvalidParameter
// };

// static Status VarIdentityTooLong = {
//   .value = 1,
//   .description = "Given identity has longer length that the maximum length "
//                  "limitation.",
//   .characteristic = STATUS_ERROR,
//   .prev = &IllegalVarIdentity
// };

typedef struct {

  /* Data */
  void *addr;
  size_t size;
  
  bool alive;
  
  // /* Identification */
  // char *identity;  // Maximum up to VAR_IDENTITY_LENGTH
  
} Var;

// typedef struct {
//   union {
//     /* Self Pointing (Var) */
//     const Var *__this__;
//   };
// } _Var;

Status Var_Create(Var *inst, size_t size)  throws(InsufficientMemory);
Status Var_CopyOf(Var *inst, Var *other);
Status Var_Literalise(Var *inst, char *buff);
bool   Var_Equals(Var *a, Var *b);
void   Var_Delete(Var *inst);

void   VarUtils_Swap(Var *v1, Var *v2);
// bool   VarUtils_IsIdentityLegal(char *identity);

#endif  /* COMPOUND_VAR */
