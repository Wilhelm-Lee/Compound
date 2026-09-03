#include "../inc/memory_stack.h"

extern llong HEAP_CAPACITY;
extern void *HEAP;

# define MEMORY_STACK_HEIGHT_MAXIMUM  (0x100000LL)
void *MEMORY_STACK_DATA[MEMORY_STACK_HEIGHT_MAXIMUM];

// struct Memory
// {
//   void *addr;

//   /* Submitted size for allocation in bytes. */
//   size_t size;

//   /* If given with a custom Destructor,
//      an execution of it will take place before calling for @_Deallocate.

//      Tip: resetting @addr to null within @Destructor to fully charge
//           the handling without releasing after it since @_Deallocate
//           skips null. */
//   void (*Destructor)(void *);

// #ifdef __COMPOUND_ALLOW_BACKTRACING__
//   Location *allocation;
//   Location *deallocation;
// #endif

// #ifdef __COMPOUND_ALLOW_RECOLLECTOR__
//   boolean is_released_automatically;
//   boolean is_garbage_collection_marked;
// #endif
// };

MemoryStack MEMORY_STACK;

inline void *Allocate(const size_t nmemb, const size_t size)
{
  void *const inst = calloc(nmemb, size);
  if (!inst && (nmemb && size)) {
    // throw(InsufficientMemory, "The size for allocation was %lu.", size);
    return null;
  }

//   Memory inst = (Memory) {
//     .addr = allocation,
//     .size = size,
// #ifdef __COMPOUND_ALLOW_BACKTRACING__
//     .allocation = __HERE__,
//     .deallocation = (Location)EMPTY,
// #endif
// #ifdef __COMPOUND_ALLOW_RECOLLECTOR__
//     .is_released_automatically = false,
//     .is_garbage_collection_marked = false,
//     .Destructor = _Deallocate
// #endif
  // };

# ifdef __COMPOUND_FEATURE_RECYCLER__
  MemoryStack_Push(&MEMORY_STACK, inst);
# endif

  return inst;
}

void _Deallocate(void *const inst)
{
  // uintptr_t allocated = false;
  // hashmap_get(MEMORY_REGISTRY, inst, sizeof(void *), &allocated);

  if (inst /* && allocated */ ) {
    // hashmap_set(MEMORY_REGISTRY, inst, sizeof(void *), false);
    free(inst);
  }
}

void InitialiseMemoryStack(MemoryStack *const inst)
{
  if (!inst) {
    fprintf(stderr,"Invalid memory stack instance pointer's reference."NEWLINE);
    exit(EXIT_FAILURE);
  }

  inst->data = MEMORY_STACK_DATA;
  inst->capacity = MEMORY_STACK_HEIGHT_MAXIMUM;
  inst->height = -1;
}

void DeinitialiseMemoryStack(MemoryStack *const inst)
{
  if (!inst) {
    return;
  }

  while (!MemoryStack_IsEmpty(inst)) {
    MemoryStack_Pop(inst);
  }
}

llong MemoryStack_Push(MemoryStack *const inst, void *const addr)
{
  if (!inst || !addr) {
    return -1;
  }

  /* Trigger GC -- Out of available memory. */
  if (MemoryStack_IsFull(inst)) {
    fprintf(
      stderr,
      "Memory stack is full (cur: %lld pointers out of cap: %lld pointers)"
      NEWLINE,
      inst->height + 1, inst->capacity);

    /* Clean up before leave. */
    _Deallocate(addr);
    DeinitialiseMemoryStack(&MEMORY_STACK);

    exit(EXIT_FAILURE);
  }

  inst->height++;
  inst->data[inst->height] = addr;

  return inst->height;
}

void MemoryStack_Pop(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return;
  }

  void *const top = MemoryStack_Top(inst);
  if (!top) {
    return;
  }

  _Deallocate(top);
  inst->height--;
}

inline void *MemoryStack_Top(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return null;
  }

  return inst->data[inst->height];
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
