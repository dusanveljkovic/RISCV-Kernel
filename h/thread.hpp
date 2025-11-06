#ifndef _THREAD_HPP
#define _THREAD_HPP

#include "hw.h"

namespace ThreadNS {
  struct TrapFrame {
    uint64 x0 = 0;  // 0
    uint64 ra = 0;  // 1 * 8   
    uint64 sp = 0;  // 2 * 8  
    uint64 gp = 0;  // 3 * 8 
    uint64 tp = 0;  // 4 * 8

    uint64 t0 = 0;  // 5 * 8
    uint64 t1 = 0;  // 6 * 8
    uint64 t2 = 0;  // 7 * 8

    uint64 s0 = 0;     
    uint64 s1 = 0;     

    uint64 a0 = 0;  // 10 * 8
    uint64 a1 = 0;  // 10 * 8
    uint64 a2 = 0;  // 10 * 8
    uint64 a3 = 0;  // 10 * 8
    uint64 a4 = 0;  // 10 * 8
    uint64 a5 = 0;  // 10 * 8
    uint64 a6 = 0;  // 10 * 8
    uint64 a7 = 0;  // 10 * 8

    uint64 s2 = 0;  // 10 * 8
    uint64 s3 = 0;  // 10 * 8
    uint64 s4 = 0;  // 10 * 8
    uint64 s5 = 0;  // 10 * 8
    uint64 s6 = 0;  // 10 * 8
    uint64 s7 = 0;  // 10 * 8
    uint64 s8 = 0;  // 10 * 8
    uint64 s9 = 0;  // 10 * 8
    uint64 s10 = 0; // 10 * 8
    uint64 s11 = 0; // 10 * 8

    uint64 t3 = 0;
    uint64 t4 = 0;
    uint64 t5 = 0;
    uint64 t6 = 0;

    uint64 sepc = 0; // 32 * 8
    uint64 sstatus = 0;//33 * 8
  }; 

  enum ThreadState {READY, RUNNING, FINISHED, BLOCKED, SLEEPING};
  struct TCB {
    void        (*start)(void*) = nullptr;
    void*       arg             = nullptr;
    void*       stackTop        = nullptr;
    ThreadState state           = READY;
    uint8       timeSlice       = DEFAULT_TIME_SLICE;
    TrapFrame*  tf;

    static TCB*   running;
    static uint8  timeSliceCounter;
    static bool   needSwitch;
  };

  void threadWrapper();

  int   create(TCB** handle, void (*start)(void*), void* arg, void* stackTop);
  void  dispatch();
  void  cleanExiting();
  void  initMainThread();
  void  sleepRunning(uint64 ticks);

  TrapFrame* maybeSwitch(TrapFrame* savedTF);
  void tick();
  inline void forceSwitch() {ThreadNS::TCB::needSwitch = true;};
}


#endif  
