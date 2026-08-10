#include "../inc/entry.h"

int Main(void)
{
  ignore args;
  ignore envs;

  Array(String) *fruits = Compose(
    Array(String),
    string("apple"),
    string("banana"),
    string("coconut"),
    string("durian"),
    string("easter-egg"),
    string("f"),
    string("gummy, fruit flavoured"),
    string("helium"),
    string("itsy bitsy apple"),
    string("john walker"),
    string("kake"),
    string("labubu"),
    string("mother's pride (R)")
  );
  if (!fruits) {
    return 1;
  }

  refeach (String, fruit, fruits, {
    printf("%s, ", (char *)refbyte(fruit, 0));
  })
  puts("");

  erase(Array(String), fruits);
  Delete(Array(String), fruits);

  return 0;
}
