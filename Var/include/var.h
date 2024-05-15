#ifndef COMPOUND_VAR
# define COMPOUND_VAR

# include <stdio.h>

# include <Compound/common.h>
# include <Compound/status.h>

# define VAR_IDENTITY_LENGTH    64
# define VAR_LITERALISE_LENGTH  (VAR_IDENTITY_LENGTH + 16 + 9 + 10)
# define VAR_LITERALISE_FORMAT  ("%s  @[%p]: %ld")
# define VAR_IDENTITY_ILLEGAL_CHAR  "!@#$%^*()-=+;\'\"\\|,./<>?[]{}`~ "

static Status IllegalVarIdentity = {
  .description = "Given identity does not fit the standard of Var Naming "
                 "convension.",
  .characteristic = STATUS_ERROR,
  .prev = &InvalidParameter
};

// static Status VarIdentityTooLong = {
//   .description = "Given identity has longer length that the maximum length "
//                  "limitation.",
//   .characteristic = STATUS_ERROR,
//   .prev = &IllegalVarIdentity
// };

typedef struct {

  /* Data */
  void *addr;
  size_t size;
  
  /* Identification */
  char *identity;  // Maximum up to VAR_IDENTITY_LENGTH
  
} Var;

// typedef struct {
//   union {
//     /* Self Pointing (Var) */
//     const Var *__this__;
//   };
// } _Var;

Status Var_Create(Var *inst, void *addr, size_t size, char *identity);
Status Var_CopyOf(Var *inst, Var *other);
Status Var_Literalise(Var *inst, char *buff);
bool   Var_Equal(Var *a, Var *b);
void   Var_Delete(Var *inst);

void   VarUtils_Swap(Var *v1, Var *v2);
bool   VarUtils_IsIdentityLegal(char *identity);

#endif  /* COMPOUND_VAR */
