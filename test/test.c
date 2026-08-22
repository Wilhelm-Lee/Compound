#include "../inc/body.h"
#include "../inc/constructor.h"
#include "../inc/destructor.h"
#include "../inc/entry.h"
#include "../inc/field.h"
#include "../inc/preprocessor.h"

# define INIT_VALUE  1
# define LOOP_COUNT  16

int Main(void)
{
  ignore args, ignore envs;

  Preprocessor *const pp_stdio = preprocessor(include, <stdio.h>);
  Preprocessor *const pp_Compound_common = preprocessor(
    include, "/external/Documents/Projects/Compound/inc/common.h"
  );
  Preprocessor *const pp_Compound_types = preprocessor(
    include, "/external/Documents/Projects/Compound/inc/types.h"
  );
  Field *const fi_counter = field(ACCESS_PUBLIC, llong, counter, INIT_VALUE);
  Function *const fn_main = function(int, main, body({
    // this is the body.
    for (register llong i = 0; i < LOOP_COUNT; i++) {
      counter <<= 1;
    }

    printf("%lld"NEWLINE, counter);

    return 0;
  }), param());

  FILE *const fp = fopen("/tmp/counter.c", "w+");

  Recreate(Preprocessor, fp, pp_stdio);
  Recreate(Preprocessor, fp, pp_Compound_common);
  Recreate(Preprocessor, fp, pp_Compound_types);
  Recreate(Field, fp, fi_counter);
  Recreate(Function, fp, fn_main);
  fprintf(fp, NEWLINE);

  fclose(fp);

  return 0;
}
