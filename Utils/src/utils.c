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

int Utils_CalcDateTimeLiteralisationLength(char *buff)
{
  const time_t now = time(NULL);
  
  (void)strftime(buff, 28, DATETIME_FORMAT, localtime(&now));
  
  return strlen(buff);
}
