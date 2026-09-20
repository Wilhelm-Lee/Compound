#include "../inc/allocator.h"
#include "../inc/memory_internal.h"
#include "../inc/memory_stack.h"

struct MemoryStack {
  Memory *data;
  llong capacity;  // The total capacity of the instance.
  llong height;  // The current indexer of the instance.
};

Memory _MEMORY_STACK_DATA[MEMORY_STACK_HEIGHT_MAXIMUM] = Empty;
MemoryStack MEMORY_STACK = Empty;

void InitialiseMemoryStack(void)
{
  MEMORY_STACK.data = _MEMORY_STACK_DATA;
  MEMORY_STACK.capacity = MEMORY_STACK_HEIGHT_MAXIMUM;
  MEMORY_STACK.height = -1;
}

void DeinitialiseMemoryStack(void)
{
  MemoryStack_PopAll(&MEMORY_STACK);
}

llong MemoryStack_Push(MemoryStack *const inst, void *const addr)
{
  if (!inst || !addr) {
    return -1;
  }

  if (MemoryStack_IsFull(inst)) {
    fprintf(
      stderr,
      "Memory stack is full (cur: %lld pointers out of cap: %lld pointers)"
      NEWLINE,
      inst->height + 1, inst->capacity
    );

    /* Clean up before leave. */
    _Deallocate(addr);
    DeinitialiseMemoryStack();

    exit(EXIT_FAILURE);
  }

  inst->height++;

  inst->data[inst->height].header.actual = addr;
  inst->data[inst->height].header.user = addr;

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

  _Deallocate(top->header.actual);

  *top = (Memory)Empty;

  inst->height--;
}

void MemoryStack_PopAll(MemoryStack *const inst)
{
  if (!inst) {
    return;
  }

  while (!MemoryStack_IsEmpty(inst)) {
    MemoryStack_Pop(inst);
  }
}

inline Memory *MemoryStack_Top(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return null;
  }

  return &inst->data[inst->height];
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

inline llong MemoryStack_GetHeight(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return -1;
  }

  return inst->height;
}

inline void *MemoryStack_GetAddress(MemoryStack *const inst, const llong idx)
{
  if (!inst || idx < 0 || idx > inst->height) {
    return nll;
  }

  return inst->data[idx].header.user;
}
