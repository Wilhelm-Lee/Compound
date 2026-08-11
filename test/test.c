#include "../inc/entry.h"

int Main(void)
{
  ignore args;
  ignore envs;

  Array(String) *fruits = Compose(
    Array(String),
    NULL,
    NULL,
    string("apple"),
    NULL,
    NULL,
    string("banana"),
    NULL,
    string("coconut"),
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );
  if (!fruits) {
    return 1;
  }

  const llong length = Length(Array(String), fruits);
  printf("%lld"NEWLINE, length);

  refeach (String, fruit, fruits, {
    printf("%s, %lld"NEWLINE, (char *)refbyte(fruit, 0), Length(String, fruit));
  })
  puts("");

  erase(Array(String), fruits);
  Delete(Array(String), fruits);

  return 0;
}
