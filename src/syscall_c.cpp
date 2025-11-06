#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/helpers.hpp"

void* mem_alloc(size_t size) {
  uint64 blocks = bytesToBlocks(size + sizeof(uint64));
  return (void*)RISCV::ecall(SYSCALL_MEM_ALLOC, (uint64)blocks);
}

int mem_free(void* addr) {
  return (int)RISCV::ecall(SYSCALL_MEM_FREE, (uint64)addr);
}

size_t mem_get_free_space() {
  return (size_t)RISCV::ecall(SYSCALL_MEM_GET_FREE_SPACE);
}

size_t mem_get_largest_free_block() {
  return (size_t)RISCV::ecall(SYSCALL_MEM_GET_LARGEST_FREE_BLOCK);
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
  void* stack = mem_alloc(DEFAULT_STACK_SIZE);
  if(stack == nullptr) return -1;

  void* stackTop = (void*)((uint64)stack + DEFAULT_STACK_SIZE);
  return (int)RISCV::ecall(SYSCALL_THREAD_CREATE, (uint64)handle, (uint64)start_routine, (uint64)arg, (uint64)stackTop);
}

int thread_exit() {
  return (int)RISCV::ecall(SYSCALL_THREAD_EXIT);
}

void thread_dispatch() {
  RISCV::ecall(SYSCALL_THREAD_DISPATCH);
}

int sem_open(sem_t* handle, unsigned init) {
  return (int)RISCV::ecall(SYSCALL_SEM_OPEN, (uint64)handle, (uint64)init);
}

int sem_close(sem_t handle) {
  return (int)RISCV::ecall(SYSCALL_SEM_CLOSE, (uint64)handle);
}

int sem_wait(sem_t id) {
  return (int)RISCV::ecall(SYSCALL_SEM_WAIT, (uint64)id);
}

int sem_signal(sem_t id) {
  return (int)RISCV::ecall(SYSCALL_SEM_SIGNAL, (uint64)id);
}

int time_sleep(time_t t) {
  return (int)RISCV::ecall(SYSCALL_TIME_SLEEP, (uint64)t);
}

char getc() {
  return (char)RISCV::ecall(SYSCALL_GETC);
}

void putc(char c) {
  RISCV::ecall(SYSCALL_PUTC, (uint64)c);
}
