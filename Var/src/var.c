#include <Compound/var.h>

void var_swap(Var *v1, Var *v2)
{
  Var v3 = {
    .addr = v1->addr,
    .sz = v1->sz
  };
  
  *v1 = *v2;
  *v2 = v3;
}

int var_literalise(Var v, wchar_t **wbuff)
{
  return swprintf(*wbuff, (2*8 + 6), L"@[%p] +%u", v.addr, v.sz);
}
