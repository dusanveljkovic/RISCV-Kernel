#ifndef _MEM_ALLOCATOR_HPP
#define _MEM_ALLOCATOR_HPP

#include "hw.h"

struct BlockHeader {
  size_t        size;
  BlockHeader*  next;
};

class MemoryAllocator {
private:
  static BlockHeader* freeList;

public:
  static void     initialize();
  static void*    mem_alloc(size_t blocksToAllocate);
  static int      mem_free(void* ptr);
  static size_t   mem_get_free_space();
  static size_t   mem_get_largest_free_block();
};

#endif
