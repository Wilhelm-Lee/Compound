#include <Compound/common.h>
#include <Compound/memman.h>

/*
  enum {
    MEMMAN_RELEASE_LEVEL_INSTANTAL = 0,
    MEMMAN_RELEASE_LEVEL_STACK = 1,
    MEMMAN_RELEASE_LEVEL_HEAP = 2
  };

  typedef struct {
    void *addr;
    int release_level;
  } Memory;

  typedef struct {
    Memory *members;
    int release_level;
  } MemoryPool;

  typedef struct {
    MemoryPool *members;
    void *(*allocator)(size_t sz);
    void (*delocator)(void *addr);
  } MemoryPoolManager;
*/

int memman_memorypoolmanager_create(MemoryPoolManager *inst,
                                    MemoryPool **membersptr)
{
  fails(inst, COMMON_ERROR_INVALID_ARGUMENT);
  fails(membersptr, COMMON_ERROR_INVALID_ARGUMENT);
  
  
}
