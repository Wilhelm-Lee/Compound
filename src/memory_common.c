#include "../inc/memory_common.h"

inline void *Allocate(const size_t nmemb, const size_t size)
{
  void *const inst = calloc(nmemb, size);
  if (!inst && (nmemb && size)) {
    // throw(InsufficientMemory, "The size for allocation was %lu.", size);
    return NULL;
  }

  return inst;
}

inline void Deallocate(void *const inst)
{
  // uintptr_t allocated = false;
  // hashmap_get(MEMORY_REGISTRY, inst, sizeof(void *), &allocated);

  if (inst /* && allocated */ ) {
    // hashmap_set(MEMORY_REGISTRY, inst, sizeof(void *), false);
    free(inst);
  }
}
