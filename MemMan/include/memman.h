#ifndef COMPOUND_MEMMAN_H
# define COMPOUND_MEMMAN_H

# include <Compound/common.h>
# include <Compound/status.h>

/* Higher the priority it is, the less lifespan it has. */

typedef struct {
  void *addr;
  size_t size;
  int priority; // Negative for manual; Higher than 0 it is,
                // higher the priority is.
  bool alive;
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

Status Memory_Create(Memory *inst, size_t size);
Status Memory_Delete(Memory *inst);
Status Memory_Allocate(Memory *inst);
Status Memory_Reallocate(Memory *inst, size_t size);
Status Memory_Release(Memory *inst);
bool   Memory_Equals(Memory *inst, Memory *other);

Status MemoryPool_Create(MemoryPool *inst, size_t volume);
Status MemoryPool_Constr(MemoryPool *inst, size_t volume, int priority);
Status MemoryPool_AllocateAt(MemoryPool *inst, int idx, size_t sz);
Status MemoryPool_ReallocateAt(MemoryPool *inst, int idx, size_t sz);
Status MemoryPool_ReleaseAt(MemoryPool *inst, int idx);
void   MemoryPool_Delete(MemoryPool *inst);

Status MemoryPoolManager_Create(MemoryPoolManager *inst,
                                       MemoryPool **members);
Status MemoryPoolManager_Constr(MemoryPoolManager *inst,
                                       MemoryPool **members,
                                       void *(*allocator)(size_t sz),
                                       void (*deallocator)(void *addr));
Status MemoryPoolManager_Merge(MemoryPool *pool1, MemoryPool *pool2);
Status MemoryPoolManager_Divide(MemoryPool *src, int off, int len);

#endif /* COMPOUND_MEMMAN_H */
