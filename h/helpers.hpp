#ifndef _HELPERS_HPP
#define _HELPERS_HPP

#include "hw.h"
#include "thread.hpp"

typedef struct ThreadList {
  ThreadNS::TCB* thread;
  ThreadList* next;

  ThreadList(ThreadNS::TCB* t, ThreadList* n): thread(t), next(n) {};
} ThreadList;

void print(const char* str); 
void print(uint64 num, uint8 base);
void print(const char* str, uint64 num, char delim = '\t', uint8 base = 16);

static inline size_t bytesToBlocks(size_t bytes) {
  return (bytes + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
}

#endif  
