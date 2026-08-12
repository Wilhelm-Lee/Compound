#include "../inc/memory_stack.h"

struct Memory
{
  void *addr;

  /* Submitted size for allocation in bytes. */
  size_t size;

  /* If given with a custom Destructor,
     an execution of it will take place before calling for @Deallocate.

     Tip: resetting @addr to NULL within @Destructor to fully charge
          the handling without releasing after it since @Deallocate
          skips NULL. */
  void (*Destructor)(void *);

#ifdef __COMPOUND_ALLOW_BACKTRACING__
  Location *allocation;
  Location *deallocation;
#endif

#ifdef __COMPOUND_ALLOW_RECOLLECTOR__
  boolean is_released_automatically;
  boolean is_garbage_collection_marked;
#endif
};

struct MemoryStack {
  Memory *data;  // An array of references.
  llong capacity;  // The total capacity of the instance.
  llong height;  // The current indexer of the instance.
};

MemoryStack *MEMORY_STACK = NULL;

void InitialiseMemoryStack(MemoryStack **const inst)
{
  if (!inst) {
    fprintf(stderr,"Invalid memory stack instance pointer's reference."NEWLINE);
    exit(EXIT_FAILURE);
  }

  *inst = calloc(1, sizeof(MemoryStack));
  if (!*inst) {
    perror("Failed to initialise memory stack (*inst).");
    exit(EXIT_FAILURE);
  }

  (*inst)->data = calloc(
    __COMPOUND_MEMORY_STACK_HEIGHT_MAXIMUM__,
    sizeof(Memory)
  );
  if (!(*inst)->data) {
    free(*inst);
    perror("Failed to initialise memory stack ((*inst)->data).");
    fprintf(
      stderr,
      "Current __COMPOUND_MEMORY_STACK_HEIGHT_MAXIMUM__ is %lld"NEWLINE,
      __COMPOUND_MEMORY_STACK_HEIGHT_MAXIMUM__
    );
    exit(EXIT_FAILURE);
  }

  (*inst)->capacity = __COMPOUND_MEMORY_STACK_HEIGHT_MAXIMUM__;
  (*inst)->height = -1;
}

void DeinitialiseMemoryStack(MemoryStack **const inst)
{
  if (!inst || !*inst) {
    return;
  }

  while (!MemoryStack_IsEmpty(*inst)) {
    MemoryStack_Pop(*inst);
  }

  free((*inst)->data);
  (*inst)->data = NULL;

  free(*inst);
  *inst = NULL;
}

llong MemoryStack_Push(MemoryStack *const inst, Memory memory)
{
  if (!inst || !memory.addr) {
    return -1;
  }

  /* Trigger GC -- Out of available memory. */
  if (MemoryStack_IsFull(inst)) {
    fprintf(stderr, "Memory stack is full (cur: %lld out of cap: %lld)"NEWLINE,
      inst->height, inst->capacity);
    exit(EXIT_FAILURE);
  }

  inst->height++;
  inst->data[inst->height] = memory;

  return inst->height;
}

void MemoryStack_Pop(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return;
  }

  Memory *const top = MemoryStack_Top(inst);
  if (!top) {
    return;
  }

  Deallocate(top->addr);
  inst->height--;
}

inline Memory *MemoryStack_Top(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return NULL;
  }

  return &inst->data[inst->height];
}

inline llong MemoryStack_GetHeight(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return -1;
  }

  return inst->height;
}

inline boolean MemoryStack_IsEmpty(MemoryStack *const inst)
{
  if (!inst) {
    return true;
  }

  return inst->height < 0;
}

inline boolean MemoryStack_IsFull(MemoryStack *const inst)
{
  if (!inst) {
    return false;
  }

  return inst->height == (inst->capacity - 1);
}

inline void *Allocate(const size_t nmemb, const size_t size)
{
  void *const allocation = calloc(nmemb, size);
  if (!allocation && (nmemb && size)) {
    // throw(InsufficientMemory, "The size for allocation was %lu.", size);
    return NULL;
  }

  Memory inst = (Memory) {
    .addr = allocation,
    .size = size,
#ifdef __COMPOUND_ALLOW_BACKTRACING__
    .allocation = __HERE__,
    .deallocation = (Location)EMPTY,
#endif
#ifdef __COMPOUND_ALLOW_RECOLLECTOR__
    .is_released_automatically = false,
    .is_garbage_collection_marked = false,
    .Destructor = Deallocate
#endif
  };

  MemoryStack_Push(MEMORY_STACK, inst);

  return allocation;
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
