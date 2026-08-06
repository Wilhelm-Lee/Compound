#include "../inc/entry.h"

int Main(const Array(String) *const args)
{
  ignore args;

  Array(String) *const strings = array(String, 10);
  refrefeach (String, itptr, strings, {
    *itptr = string("content");
  })

  erase(Array(String), strings);
  Delete(Array(String), strings);

  return 0;
}
