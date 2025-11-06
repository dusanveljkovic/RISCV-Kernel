#ifndef _SLEEP_HPP
#define _SLEEP_HPP

#include "hw.h"
#include "thread.hpp"

namespace Sleep {
  struct Node {
    ThreadNS::TCB* thread;
    uint64 wakeupTick;
    Node* next;

    Node(ThreadNS::TCB* t, uint64 wt, Node* n): thread(t), wakeupTick(wt), next(n) {};
  };

  void  insert(Node* n);
  Node* pop(uint64 now);

  void  wakeup();
  void  sleep(ThreadNS::TCB* thread, uint64 ticks);
}

#endif
