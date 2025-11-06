#include "../h/semaphore.hpp"
#include "../h/thread.hpp"
#include "../h/scheduler.hpp"
#include "../h/syscall_c.h"
#include "../h/helpers.hpp"

using ThreadNS::TCB;
using ThreadNS::ThreadState;

int Sem::create(Sem** handle, int init) {
  *handle = new Sem(init);
  if(*handle == nullptr) return -1;
  return 0;
}

int Sem::wait() {
  if(closed) return -1;
  if (--value < 0) {
    TCB* t = TCB::running;
    t->state = ThreadState::BLOCKED;
    
    ThreadList* newElem = new ThreadList(t, nullptr);

    if(blockedTail == nullptr)
      blockedHead = blockedTail = newElem;
    else {
      blockedTail->next = newElem;
      blockedTail = newElem;
    }
    thread_dispatch();
  }
  if(closed) return -1;
  return 0;
}

int Sem::signal() {
  if(closed) return -1;
  if(++value <= 0) {
    ThreadList* t = blockedHead;
    if(t == nullptr) return -1;

    blockedHead = blockedHead->next;
    if(blockedHead == nullptr) blockedTail = nullptr;

    t->thread->state = ThreadState::READY;
    Scheduler::put(t->thread);
    delete t;
  }
  return 0;
}

int Sem::close() {
  ThreadList* t = blockedHead;
  ThreadList* tmp;
  closed = true;
  while(t) {
    t->thread->state = ThreadState::READY;
    Scheduler::put(t->thread);
    tmp = t->next;
    delete t;
    t = tmp;
  }
  return 0;
}
