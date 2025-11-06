#include "../h/riscv.hpp"

uint64 RISCV::ecall(uint64 code, uint64 arg1, uint64 arg2, uint64 arg3, uint64 arg4) {
  asm volatile ("ecall");  
  uint64 ret; 
  asm volatile ("mv %0, a0" : "=r"(ret));
  return ret;
}
