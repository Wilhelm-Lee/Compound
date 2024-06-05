#include <Compound/memman.h>

Status Memory_Create(Memory *inst, size_t size)
{
  fails(inst, UnavailableInstance);
  
  *inst = (Memory) {
    .addr = NULL,
    .size = size,
    .priority = 0,
    .alive = false
  };
  
  return NormalStatus;
}

Status Memory_Allocate(Memory *inst)
{
  fails(inst, UnavailableInstance);
  state(inst->alive, InstanceStillAlive);
  
  /* When failed on allocating. */
  state(!(inst->addr = malloc(inst->size)), InsufficientMemory);
  inst->alive = true;
  
  return NormalStatus;
}

Status Memory_Reallocate(Memory *inst, size_t size)
{
  fails(inst, UnavailableBuffer);
  state(!inst->alive, InstanceNotAlive);
  
  /* When failed on reallocating. */
  state(!(inst->addr = realloc(inst->addr, size)),
    error(InsufficientMemory, "Unsuccessful reallocation was received."))

  return NormalStatus;
}

Status Memory_Release(Memory *inst)
{
  fails(inst, UnavailableInstance);
  state(!inst->alive, error(InstanceNotAlive, "Cannot release a non-alive "
                            "instance."));

  free(inst->addr);
  inst->alive = false;

  return NormalStatus;
}

Status Memory_Delete(Memory *inst)
{
  fails(inst, UnavailableInstance);
  state(inst->alive, error(InstanceStillAlive, "Cannot deinitialise a instance "
                           "still alive."));

  inst->addr = NULL;
  inst->priority = 0;
  inst->size = 0;
  inst = NULL;

  return NormalStatus;
}

bool Memory_Equals(Memory *inst, Memory *other)
{
  state(!inst || !other, false);

  return (inst->addr == other->addr
          && inst->size == other->size
          && inst->priority == other->priority
          && inst->alive == other->alive);
}
