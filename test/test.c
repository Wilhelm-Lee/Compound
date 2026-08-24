#include "../inc/body.h"
#include "../inc/class.h"
#include "../inc/constructor.h"
#include "../inc/destructor.h"
#include "../inc/entry.h"
#include "../inc/field.h"
#include "../inc/preprocessor.h"

// to test:
// String *String_Append(String *const inst, const Array(String) *const contents);

/* This header includes everything generated.
 * Before the generation, it is suppose to be empty, making no difference.
 */
#include "../user/header.h"

int Main(void)
{
  ignore args, ignore envs;

  Class *const c_Cat = Create(Class, ACCESS_PRIVATE, string("Cat"));
  if (!c_Cat) {
    return 1;
  }

  Class *const c_CopyCat = CopyOf(Class, c_Cat);
  if (!c_CopyCat) {
    Delete(Class, c_Cat);
    return 1;
  }

  Delete(Class, c_CopyCat);
  Delete(Class, c_Cat);



  // Signature *const signature = Create(Signature, string(""), string(""), Compose(Array(Parameter), Parameter))


  // Preprocessor *const pp_stdlib = preprocessor(
  //   include, <stdlib.h>
  // );
  // Preprocessor *const pp_time = preprocessor(
  //   include, <time.h>
  // );
  // Preprocessor *const pp_Compound_entry = preprocessor(
  //   include, "../inc/entry.h"
  // );

  // Function *const fn_Main = function(int, Main, body({
  //   ignore args, ignore envs;

  //   srand(time(NULL));

  //   Array(int) *const iarr = array(int, 10);
  //   refeach (int, it, iarr, {
  //     *it = rand() % INT32_MAX;
  //   })

  //   puts(flatten(char, lit(Array(int), iarr, string("["), string(","), string("]"))));

  //   return 0;
  // }), param(void));

  // Recreate(Preprocessor, stdout, pp_stdlib);
  // Recreate(Preprocessor, stdout, pp_time);
  // Recreate(Preprocessor, stdout, pp_Compound_entry);
  // Recreate(Function, stdout, fn_Main);

  // int retval = invoke(fn_Main);

  return 0;
}
