#ifndef COMPOUND_MEMMAN_H
# define COMPOUND_MEMMAN_H

# include <Compound/common.h>
# include <Compound/status.h>

/*
   Higher the priority it is, the less time for lifespan it has.
 */

typedef struct {
  void *addr;
  size_t length;
  int priority; // Negative for manual; Higher than 0 it is,
                // higher the priority is.
} Memory;  // 20 Bytes

typedef struct {
  Memory *members;
  size_t volume;
  int priority;
} MemoryPool;

typedef struct {
  MemoryPool *members;
  void *(*allocator)(size_t sz);
  void (*deallocator)(void *addr);
} MemoryPoolManager;

Status MemMan_Memory_Allocate(Memory *inst, size_t length);
Status MemMan_Memory_Reallocate(Memory *inst, size_t length);
void   MemMan_Memory_Release(Memory *inst);
Status MemMan_Memory_Prioritise(Memory *inst);
Status MemMan_Memory_Deprioritise(Memory *inst);
bool   MemMan_Memory_Equal(Memory *inst, Memory *other);

Status MemMan_MemoryPool_Create(MemoryPool *inst, size_t volume);
Status MemMan_MemoryPool_Constr(MemoryPool *inst, size_t volume, int priority);
Status MemMan_MemoryPool_AllocateAt(MemoryPool *inst, int idx, size_t sz);
Status MemMan_MemoryPool_ReallocateAt(MemoryPool *inst, int idx, size_t sz);
Status MemMan_MemoryPool_ReleaseAt(MemoryPool *inst, int idx);
void   MemMan_MemoryPool_Delete(MemoryPool *inst);

Status MemMan_MemoryPoolManager_Create(MemoryPoolManager *inst,
                                       MemoryPool **members);
Status MemMan_MemoryPoolManager_Constr(MemoryPoolManager *inst,
                                       MemoryPool **members,
                                       void *(*allocator)(size_t sz),
                                       void (*deallocator)(void *addr));
Status MemMan_MemoryPoolManager_Merge(MemoryPool *pool1, MemoryPool *pool2);
Status MemMan_MemoryPoolManager_Divide(MemoryPool *src, int off, int len);

#endif /* COMPOUND_MEMMAN_H */
