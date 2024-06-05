#include <Compound/utils.h>

int Utils_CalcDigits(long long int n)
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

int Utils_LiteraliseInteger(long long int n, char *buff)
{
  /* Invalid buffer was presented. */
  if (strlen(buff) != LITERALISATION_LENGTH_MAXIMUM) {
    buff = NULL;
    return 0;
  }
  
  if (!n) {
    buff = "0";
    return 1;
  }
  
  const int literalising_len = Utils_CalcDigits(n);
  if (literalising_len >= LITERALISATION_LENGTH_MAXIMUM) {
    buff = NULL;
    return 0;
  }
  
  char literalising[literalising_len];
  for (register int i = 0; i < literalising_len; i++) {
    literalising[i] = (n / (int)pow(10, i));
  }
  
  return literalising_len;
}

int Utils_DateTimeLiteralise(time_t t, char *buff)
{
  return 0;
}

