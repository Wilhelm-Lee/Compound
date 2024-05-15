#include <Compound/utils.h>

int Utils_CalcDigits(long long n)
{
  if (n == 0) {
    return 1;
  }
  
  n = llabs(n);
  
  /* Accumulate. */
  register int i;
  for (i = 0; n; i++)  n /= 10;
  
  return i;
}
