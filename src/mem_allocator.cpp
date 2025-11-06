#include "../h/mem_allocator.hpp"

BlockHeader* MemoryAllocator::freeList = nullptr;

static inline uint64 heapStartAddress() { return (uint64)HEAP_START_ADDR; }
static inline uint64 heapEndAddress()   { return (uint64)HEAP_END_ADDR; }

static inline uint64 alignToBlock(uint64 addr) {
  uint64 offset = addr - heapStartAddress();
  size_t blockIndex = offset / MEM_BLOCK_SIZE;
  return heapStartAddress() + blockIndex * MEM_BLOCK_SIZE;
}

void MemoryAllocator::initialize() {
  if(freeList != nullptr) return;
  uint64 start = alignToBlock(heapStartAddress());
  uint64 end = alignToBlock(heapEndAddress());

  size_t numberOfBlocks = (end - start) / MEM_BLOCK_SIZE;
  freeList = (BlockHeader*)start;
  freeList->size = numberOfBlocks;
  freeList->next = nullptr;
}

void* MemoryAllocator::mem_alloc(size_t blocksToAllocate) {
  if(blocksToAllocate == 0 || freeList == nullptr) return nullptr;
  
  BlockHeader* prev = nullptr;
  BlockHeader* current = freeList;

  while(current) {
    uint64 addrToReturn = (uint64)current + sizeof(uint64);
    if(current->size >= blocksToAllocate) {
      if(current->size == blocksToAllocate) {
        if (prev)
          prev->next = current->next;
        else 
          freeList = current->next;
      } else {
        uint64 nextFreeBlockAddress = (uint64)current + blocksToAllocate * MEM_BLOCK_SIZE;
        BlockHeader* nextFreeBlock = (BlockHeader*)nextFreeBlockAddress;

        nextFreeBlock->size = current->size - blocksToAllocate;
        nextFreeBlock->next = current->next;

        if (prev)
          prev->next = nextFreeBlock;
        else
          freeList = nextFreeBlock;
      }

      *(uint64*)current = blocksToAllocate;
      return (void*)addrToReturn;
    }
    prev = current;
    current = current->next;
  }

  return nullptr;
}

int MemoryAllocator::mem_free(void* ptr) {
  if(ptr == nullptr) return -1;

  // Pointer mora da bude izmedju pocetka i kraja heapa i poravnat na blok
  ptr = (void*)((uint64)ptr - sizeof(uint64));
  uint64 pointer = (uint64)ptr;
  if(pointer < heapStartAddress() || pointer >= heapEndAddress ()) return -2;
  if((pointer - heapStartAddress()) % MEM_BLOCK_SIZE != 0 ) return -3;

  BlockHeader* blockToFree = (BlockHeader*)ptr;

  if (blockToFree->size == 0) return -4;

  BlockHeader* prev = nullptr;
  BlockHeader* current = freeList;

  while(current && (uint64)current < (uint64)blockToFree) {
    prev = current;
    current = current->next;
  }

  if(prev != nullptr) {
    uint64 prevEnd = (uint64)prev + prev->size * MEM_BLOCK_SIZE;
    if(prevEnd == (uint64)blockToFree) {
      prev->size += blockToFree->size;

      if(current != nullptr) {
        uint64 newEnd = prevEnd + blockToFree->size * MEM_BLOCK_SIZE;
        if (newEnd == (uint64)current) {
          prev->size += current->size;
          prev->next = current->next;
        }
      }

      return 0;
    }
  }

  if(current != nullptr) {
    uint64 toFreeEnd = (uint64)blockToFree + blockToFree->size * MEM_BLOCK_SIZE;
    if(toFreeEnd == (uint64)current) {
      blockToFree->size += current->size;

      blockToFree->next = current->next;
      if (prev)
        prev->next = blockToFree;
      else
        freeList = blockToFree;       

      return 0;
    }
  }

  blockToFree->next = current;
  if (prev)
    prev->next = blockToFree;
  else
    freeList = blockToFree;       
  return 0;
}

size_t MemoryAllocator::mem_get_free_space() {
  size_t total = 0;
  BlockHeader* current = freeList;
  while(current) {
    total += current->size * MEM_BLOCK_SIZE;
    current = current->next;
  }
  return total;
}

size_t MemoryAllocator::mem_get_largest_free_block() {
  size_t max = 0;
  BlockHeader* current = freeList;
  while(current) {
    if (current->size > max)
      max = current->size;
    current = current->next;
  }
  return max * MEM_BLOCK_SIZE;
}
