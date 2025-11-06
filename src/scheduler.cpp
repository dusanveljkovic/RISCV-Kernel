#include "../h/scheduler.hpp"
#include "../h/helpers.hpp"
#include "../h/riscv.hpp"

using ThreadNS::TCB;

ThreadList* Scheduler::readyHead = nullptr;
ThreadList* Scheduler::readyTail = nullptr;

TCB* Scheduler::get() {
  ThreadList* t = readyHead;
  if(t == nullptr) return nullptr;

  // print("Scheduler getting", (uint64)t->thread, '\n');

  readyHead = readyHead->next;
  if(readyHead == nullptr) readyTail = nullptr;

  TCB* tcb = t->thread;
  delete t;
  return tcb;
}

void Scheduler::put(TCB* t) {
  // print("Scheduler putting", (uint64)t, '\n');

  t->state = ThreadNS::READY;
  ThreadList* newElem = new ThreadList(t, nullptr);

  if(readyTail == nullptr)
    readyHead = readyTail = newElem;
  else {
    readyTail->next = newElem;
    readyTail = newElem;
  }
}

void Scheduler::show() {
  ThreadList* t = readyHead;
  print("[S] SCHEDULER:\t");
  while(t != nullptr) {
    print("", (uint64)t->thread, '\t');
    t = t->next;
  }
  print("\n");
}
