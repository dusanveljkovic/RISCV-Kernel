#ifndef _RISCV_HPP
#define _RISCV_HPP

#include "hw.h"

#ifdef __cplusplus
extern "C" {
#endif

enum SycallCode {
  SYSCALL_MEM_ALLOC                   = 0x01,
  SYSCALL_MEM_FREE                    = 0x02,
  SYSCALL_MEM_GET_FREE_SPACE          = 0x03,
  SYSCALL_MEM_GET_LARGEST_FREE_BLOCK  = 0x04,

  SYSCALL_THREAD_CREATE               = 0x11,
  SYSCALL_THREAD_EXIT                 = 0x12,
  SYSCALL_THREAD_DISPATCH             = 0x13,

  SYSCALL_SEM_OPEN                    = 0x21,
  SYSCALL_SEM_CLOSE                   = 0x22,
  SYSCALL_SEM_WAIT                    = 0x23,
  SYSCALL_SEM_SIGNAL                  = 0x24,

  SYSCALL_TIME_SLEEP                  = 0x31,

  SYSCALL_GETC                        = 0x41,
  SYSCALL_PUTC                        = 0x42,
};

#define SIE_SSIE (1ull << 1)
#define SIE_SEIE (1ull << 9)
#define SSTATUS_SIE (1ull << 1)

namespace RISCV {
  inline uint64 r_sepc() {
    uint64 x;
    asm volatile("csrr %0, sepc" : "=r"(x));
    return x;
  }

  inline uint64 r_sstatus() {
    uint64 x;
    asm volatile("csrr %0, sstatus" : "=r"(x));
    return x;
  }

  inline uint64 r_scause() {
    uint64 x;
    asm volatile("csrr %0, scause" : "=r"(x));
    return x;
  }

  inline uint64 r_sip() {
    uint64 x;
    asm volatile("csrr %0, sip" : "=r"(x));
    return x;
  }

  inline uint64 r_sie() {
    uint64 x;
    asm volatile("csrr %0, sie" : "=r"(x));
    return x;
  }

  inline uint64 r_stval() {
    uint64 x;
    asm volatile("csrr %0, stval" : "=r"(x));
    return x;
  }

  inline void w_sepc(uint64 x)    {asm volatile("csrw sepc, %0" : : "r"(x)); }
  inline void w_sstatus(uint64 x) {asm volatile("csrw sstatus, %0" : : "r"(x));}
  inline void w_stvec(void* x)    {asm volatile("csrw stvec, %0" : : "r"(x));}
  inline void w_sip(uint64 x)     {asm volatile("csrw sip, %0" : : "r"(x));}
  inline void w_sie(uint64 x)     {asm volatile("csrw sie, %0" : : "r"(x));}

  inline void enableSoftwareInterrupts() { asm volatile("csrs sie, %0" : : "r"(SIE_SSIE));}
  inline void disableSoftwareInterrupts() { asm volatile("csrc sie, %0" : : "r"(SIE_SSIE));}
  inline void enableExternalInterrupts() { asm volatile("csrs sie, %0" : : "r"(SIE_SEIE));}
  inline void disableExternalInterrupts() { asm volatile("csrc sie, %0" : : "r"(SIE_SEIE));}
  inline void enableInterrupts() { asm volatile("csrs sstatus, %0" : : "r"(SSTATUS_SIE));}
  inline void disableInterrupts() { asm volatile("csrs sstatus, %0" : : "r"(SSTATUS_SIE));}

  inline void clearInterruprs() { asm volatile("csrc sip, %0" : : "r"(SIE_SSIE));}

  uint64 ecall(uint64 code, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0, uint64 arg4 = 0);

  inline void forceShutdown() { *(volatile uint32*)0x100000 = 0x5555; }
};

#ifdef __cplusplus
}
#endif

#endif  
