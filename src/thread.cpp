#include "../h/thread.hpp"
#include "../h/scheduler.hpp"
#include "../h/helpers.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/sleep.hpp"

using ThreadNS::TCB;
using ThreadNS::TrapFrame;

TCB*  TCB::running          = nullptr;
uint8 TCB::timeSliceCounter = 0;
bool  TCB::needSwitch       = false;

uint64 initialSSTATUS() {
  uint64 s = RISCV::r_sstatus();
  s &= ~(1ull << 8); // SPP na 0 - korisnicki rezim
  s |= 1ull << 5; // SPIE na 1 - spoljasni prekidi dozvoljeni
  return s;
}

void ThreadNS::initMainThread() {
  static TCB main{};
  static TrapFrame tf{};
  asm volatile("mv %0, sp" : "=r"(tf.sp));

  main.tf = &tf;
  main.state = RUNNING;
  TCB::running = &main;
}

void ThreadNS::cleanExiting() {
  // print("THREAD IS EXITING", (uint64)TCB::running, '\n');
  TCB* oldRunning = TCB::running;
  oldRunning->state = FINISHED;

  mem_free((void*)((uint64)oldRunning->stackTop + sizeof(TrapFrame) - DEFAULT_STACK_SIZE));
  delete oldRunning;
}

void ThreadNS::threadWrapper() {
  TCB* running = TCB::running;
  void (*functionToRun)(void*) = running->start;
  void* args = running->arg;

  functionToRun(args);

  thread_exit();
}


int ThreadNS::create(TCB** handle, void (*start)(void*), void* arg, void* stackTop) {
  if(handle == nullptr || start == nullptr || stackTop == nullptr) return -1;
  TCB* t = new TCB();

  t->start = start;
  t->arg = arg;
  t->stackTop = stackTop;
  uint64 sp = (uint64)stackTop - sizeof(TrapFrame);

  TrapFrame* tf = (TrapFrame*)sp;

  for(int i = 0; i < 34; i++) 
    *((uint64*)tf + i) = 0;
  
  tf->sepc = (uint64)threadWrapper;
  tf->sstatus = initialSSTATUS();
  tf->sp = sp;
  
  t->tf = tf;
  t->state = READY;

  Scheduler::put(t);
  *handle = t;

  return 0;
}

TrapFrame* ThreadNS::maybeSwitch(TrapFrame *savedTF) {
  if(!TCB::needSwitch) return savedTF;
  // Scheduler::show();
  TCB::needSwitch = false;
  TCB::timeSliceCounter = 0;

  TCB* oldRunning = TCB::running;
  TCB* next = Scheduler::get();

  if(next != nullptr && next->state == READY) {
    if(oldRunning) {
      oldRunning->tf = savedTF;
      if(oldRunning->state == RUNNING) {
        oldRunning->state = READY;
        Scheduler::put(oldRunning);
      }
    } 

    TCB::running = next;

    next->state = RUNNING;
    return next->tf;
  }
  return savedTF;
}

void ThreadNS::tick() {
  ThreadNS::TCB::timeSliceCounter++; 
  if(ThreadNS::TCB::timeSliceCounter >= ThreadNS::TCB::running->timeSlice)
    ThreadNS::TCB::needSwitch = true;
}

void ThreadNS::sleepRunning(uint64 ticks) {
  if(ticks > 0)
    Sleep::sleep(TCB::running, ticks);
  
  TCB::needSwitch = true;
  return;
}
