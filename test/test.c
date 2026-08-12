#include "../inc/entry.h"

int Main(Array(String) *args, Array(String) *envs)
{
  ignore args, ignore envs;

  String *content = string("This is not an apple.");
  if (!content) {
    return 1;
  }

  Array(String) *tokens = tokenise(content, " ");
  if (!tokens) {
    return 1;
  }

  refeach (String, it, tokens, {
    printf("'%s'"NEWLINE, (char *)refbyte(it, 0));
  })

  return 0;
}
