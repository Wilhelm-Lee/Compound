#include "../inc/init.h"

# ifdef __COMPOUND_FEATURE_STATUS__
void InitialiseStatusStack(Stack(Status) **const instptr)
{
  if (!instptr) {
    return;
  }

  *instptr = Create(Stack(Status), STATUS_STACK_HEIGHT_MAXIMUM);
}

void DeinitialiseStatusStack(Stack(Status) **const instptr)
{
  if (!instptr) {
    return;
  }

  Delete(Stack(Status), instptr);
  *instptr = NULL;
}
# endif

int InitialiseMain(
  const int argc,
  const char *const *restrict const argv,
  const char *const *restrict const envp,
  Array(String) **const args,
  Array(String) **const envs
) {

  /* Since the parameter is not adaptive to
     __COMPOUND_FEATURE_ARGUMENT__ nor __COMPOUND_FEATURE_ENVIRONMENT__,
     this tries to resolve unused parameters warning by ignoring them. */
  ignore argc;
  ignore argv;
  ignore envp;
  ignore args;
  ignore envs;

  if (
# if !defined (__COMPOUND_FEATURE_ARGUMENT__) &&\
     !defined (__COMPOUND_FEATURE_ENVIRONMENT__)
    false
# endif
# ifdef __COMPOUND_FEATURE_ARGUMENT__
    !argv
    || !args
# endif
# ifdef __COMPOUND_FEATURE_ENVIRONMENT__
    || !envp
    || !envs
# endif
  ) {
    fprintf(
      stderr,
      "Invalid parameters were passed to %s: "
      "assert(!argv || !envp || !args || !envs)",
      __func__
    );
    return 1;
  }

# ifdef __COMPOUND_FEATURE_ARGUMENT__
  *args = array(String, argc);
  if (!args) {
    fprintf(stderr, "Failed to construct args."NEWLINE);
    return 1;
  }

  iterate (String, i, *args, {
    set(Array(String), *args, i, string(argv[i]));
  })
# endif

# ifdef __COMPOUND_FEATURE_ENVIRONMENT__
  /* Count for envp length. */
  register llong envp_len = 0;
  while (envp[envp_len]) {
    envp_len++;
  }

  *envs = array(String, envp_len);
  if (!envs) {
    erase(Array(String), args);
    Deallocate(args);
    return 1;
  }

  loop (i, envp_len) {
    ref(Array(String), *envs, i) = string(envp[i]);
  }
# endif

# ifdef __COMPOUND_FEATURE_STATUS__
  InitialiseStatusStack(&STATUS_STACK);
# endif

# ifdef __COMPOUND_FEATURE_MEMORY__
  InitialiseMemoryStack(&MEMORY_STACK);
# endif

  return 0;
}

int DeinitialiseMain(
  Array(String) **const args,
  Array(String) **const envs
) {
  ignore args;
  ignore envs;

  if (envs) {
    erase(Array(String), *envs);
    Delete(Array(String), *envs);
    *envs = NULL;
  }

  if (args) {
    erase(Array(String), *args);
    Delete(Array(String), *args);
    *args = NULL;
  }

# ifdef __COMPOUND_FEATURE_STATUS__
  DeinitialiseStatusStack(&STATUS_STACK);
# endif

# ifdef __COMPOUND_FEATURE_MEMORY__
  DeinitialiseMemoryStack(&MEMORY_STACK);
# endif

  return 0;
}
