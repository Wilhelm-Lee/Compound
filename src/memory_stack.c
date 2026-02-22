#include "../inc/memory_stack.h"

struct MemoryStack {
  Memory **data;
  llong capacity;  //< The total capacity of the instance.
  llong height;  //< The current indexer of the instance.
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

  (*inst)->data = calloc(MEMORY_STACK_HEIGHT_MAXIMUM, sizeof(Memory *));
  if (!(*inst)->data) {
    perror("Failed to initialise memory stack ((*inst)->data).");
    fprintf(stderr, "Current MEMORY_STACK_HEIGHT_MAXIMUM is %lld"NEWLINE,
           MEMORY_STACK_HEIGHT_MAXIMUM);
    exit(EXIT_FAILURE);
  }

  (*inst)->capacity = MEMORY_STACK_HEIGHT_MAXIMUM;
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

llong MemoryStack_Push(MemoryStack *const inst, const Memory *memory)
{
  if (!inst || !memory) {
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

  Memory **const top = MemoryStack_Top(inst);
  if (!top || !(*top)) {
    return;
  }

  _Deallocate(Memory_GetAddr(*top));
  Delete(Memory, MemoryStack_Top(inst));
  inst->height--;
}

inline Memory **MemoryStack_Top(MemoryStack *const inst)
{
  if (!inst || MemoryStack_IsEmpty(inst)) {
    return NULL;
  }

  return &inst->data[inst->height];
}

inline boolean MemoryStack_IsEmpty(const MemoryStack *const inst)
{
  if (!inst) {
    return true;
  }

  return inst->height < 0;
}

inline boolean MemoryStack_IsFull(const MemoryStack *const inst)
{
  if (!inst) {
    return false;
  }

  return inst->height == (inst->capacity - 1);
}

