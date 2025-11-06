#include "../h/trap.hpp"
#include "../h/riscv.hpp"
#include "../h/helpers.hpp"
#include "../h/mem_allocator.hpp"
#include "../h/thread.hpp"
#include "../h/console.hpp"
#include "../h/semaphore.hpp"
#include "../h/sleep.hpp"


void lock() {
  RISCV::disableSoftwareInterrupts();
  RISCV::disableExternalInterrupts();
}

void unlock() {
  RISCV::clearInterruprs();

  RISCV::enableSoftwareInterrupts();
  RISCV::enableExternalInterrupts();
}

void Trap::trapInit() {
  RISCV::w_stvec((void*)trapEntry);

  unlock();

  RISCV::enableInterrupts();
}

void handleSupervisorTrap(uint64 code, uint64 a1, uint64 a2, uint64 a3, uint64 a4, ThreadNS::TrapFrame* savedTF) {
  lock();

  uint64 scause = RISCV::r_scause();

  bool    isEcall       = ((scause & 1ULL << 63) == 0 && ((scause & 0xff) == 9 || (scause & 0xff) == 8));
  bool    isInterrupt   = (scause >> 63);
  uint64  interruptCode = (scause & ((1ULL << 63)-1));
  uint64  retValue      = 0;


  // timer interrupt
  if(isInterrupt && interruptCode == 1) {
    Sleep::wakeup();
    ThreadNS::tick();

  // console interrupt
  } else if (isInterrupt && interruptCode == 9) {
    int irq = plic_claim();
    if(irq == 10) consoleHandler();
    if(irq)       plic_complete(irq);
  }

  else if(isEcall) {
    switch(code) {
      case SYSCALL_MEM_ALLOC:
        retValue = (uint64)MemoryAllocator::mem_alloc(a1);
        break;
      case SYSCALL_MEM_FREE:
        retValue = (uint64)MemoryAllocator::mem_free((void*)a1);
        break;
      case SYSCALL_MEM_GET_FREE_SPACE:
        retValue = (uint64)MemoryAllocator::mem_get_free_space();
        break;
      case SYSCALL_MEM_GET_LARGEST_FREE_BLOCK:
        retValue = (uint64)MemoryAllocator::mem_get_largest_free_block();
        break;

      case SYSCALL_THREAD_CREATE: {
        ThreadNS::TCB** handle = (ThreadNS::TCB**)a1; 
        void (*body)(void*) = (void(*)(void*))a2;
        void* arg = (void*)a3;
        void* stackTop = (void*)a4;

        retValue = (uint64)ThreadNS::create(handle, body, arg, stackTop);
        break;
      }
        break;
      case SYSCALL_THREAD_EXIT:
        ThreadNS::cleanExiting();
        ThreadNS::forceSwitch();
        break;
      case SYSCALL_THREAD_DISPATCH: 
        ThreadNS::forceSwitch();
        break;
      

      case SYSCALL_SEM_OPEN: 
        retValue = Sem::create((Sem**)a1, (int)a2);
        break;
      case SYSCALL_SEM_CLOSE: 
        retValue = ((Sem*) a1)->close();
        break;
      case SYSCALL_SEM_WAIT: 
        retValue = ((Sem*) a1)->wait();
        break;
      case SYSCALL_SEM_SIGNAL: 
        retValue = ((Sem*) a1)->signal();
        break;

      case SYSCALL_TIME_SLEEP:
        ThreadNS::sleepRunning((uint64) a1);
        break;

      case SYSCALL_GETC:
        retValue = (uint64)consoleGETC();
        break;
      case SYSCALL_PUTC:
        retValue = (uint64)consolePUTC((char) a1);
        break;
        
    }

    uint64 sepc = RISCV::r_sepc() + 4;
    savedTF->sepc = sepc;
    savedTF->a0 = retValue;
  } else {
    print("ILLEGAL\n");
    print("scause", scause, '\n', 2);
    print("sepc", RISCV::r_sepc(), '\n');
    // RISCV::forceShutdown();
  }
  savedTF = ThreadNS::maybeSwitch(savedTF);
  unlock();
  asm volatile ("mv a0, %0" : : "r"(savedTF->a0));
  asm volatile ("mv a5, %0" : : "r"(savedTF));
  return;
}
