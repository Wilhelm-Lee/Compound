#include "../inc/entry.h"

int main(void)
{
  // ignore args;
  // ignore envs;

  InitialiseMemoryStack(&MEMORY_STACK);

  String *content = string("This is not an apple.");
  if (!content) {
    return 1;
  }

  // const llong count = count(content, )

  DeinitialiseMemoryStack(&MEMORY_STACK);

  return 0;
}
