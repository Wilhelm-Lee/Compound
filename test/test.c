#include "../inc/entry.h"

int Main(Array(String) *const args, Array(String) *const envs)
{
  ignore args, ignore envs;

  Array(String) *parsed_functions = array(String, 0);
  refeach (String, env, envs, {
    if (contains(env, string("."))) {
      parsed_functions = call(Array(String), Insert, parsed_functions, -1, env);
    }
  })

  refeach (String, func, parsed_functions, {
    printf("%s"NEWLINE, (char *)refbyte(func, 0));
  })

  ig parsed_functions;
  
  return 0;
}
