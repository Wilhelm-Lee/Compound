#ifndef COMPOUND_VAR
# define COMPOUND_VAR

# include <stdio.h>
# include <stdlib.h>
# include <wchar.h>
# include <stdint.h>
# include <Compound/common.h>

typedef struct {
  void *addr;
  size_t sz;
} Var;

void var_swap(Var *v1, Var *v2);

int var_literalise(Var v, wchar_t **wbuff);

#endif  /* COMPOUND_VAR */
