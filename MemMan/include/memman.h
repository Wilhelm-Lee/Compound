#ifndef MEMMAN_H
#define MEMMAN_H

#include <Compound/common.h>
#include <Compound/status.h>

/*
   Higher the priority it is, the less time for lifespan it has.
 */

typedef struct {
  void *addr;
  int priority; // -1 for manual
} Memory;

typedef struct {
  Memory *members;
  size_t poolsize;
  int priority;
} MemoryPool;

typedef struct {
  MemoryPool *members;
  void *(*allocator)(size_t sz);
  void (*delocator)(void *addr);
} MemoryPoolManager;

Status memman_memory_allocate(Memory *inst, size_t sz);
Status memman_memory_reallocate(Memory *inst, size_t sz);
void memman_memory_release(Memory *inst);

Status memman_memorypool_create(MemoryPool *inst, Memory **members,
                                size_t poolsize);
Status memman_memorypool_constr(MemoryPool *inst, Memory **members,
                                size_t poolsize, int priority);
Status memman_memorypool_allocate(MemoryPool *inst, int idx, size_t sz);
Status memman_memorypool_reallocate(MemoryPool *inst, int idx, size_t sz);
Status memman_memorypool_release(MemoryPool *inst, int idx);
Status memman_memorypool_prioritise(MemoryPool *inst, int idx);
Status memman_memorypool_deprioritise(MemoryPool *inst, int idx);
void memman_memorypool_delete(MemoryPool *inst);

Status memman_memorypoolmanager_create(MemoryPoolManager *inst,
                                       MemoryPool **membersptr);
Status memman_memorypoolmanager_constr(MemoryPoolManager *inst,
                                       MemoryPool **membersptr,
                                       void *(*allocator)(size_t sz),
                                       void (*delocator)(void *addr));
Status memman_memorypoolmanager_addmember(MemoryPoolManager *inst,
                                          MemoryPool *newmember);
Status memman_memorypoolmanager_removemember(MemoryPoolManager *inst, int idx);
Status memman_memorypoolmanager_allocate(MemoryPoolManager *inst,
                                         ArrayIndexerRange selected, size_t sz);
Status memman_memorypoolmanager_release(MemoryPoolManager *inst,
                                        ArrayIndexerRange selected);

#endif /* MEMMAN_H */
