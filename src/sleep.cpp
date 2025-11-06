#include "../h/sleep.hpp"
#include "../h/thread.hpp"
#include "../h/scheduler.hpp"

using Sleep::Node;
using ThreadNS::TCB;

static Node* head = nullptr;
static uint64 ticksNow = 0;

void Sleep::insert(Node* n) {
  if(head == nullptr || n->wakeupTick < head->wakeupTick) {
    n->next = head;
    head = n;
    return;
  }
  Node* curr = head;
  while(curr->next && curr->next->wakeupTick <= n->wakeupTick)
    curr = curr->next;
  n->next = curr->next;
  curr->next = n;
}

Node* Sleep::pop(uint64 now) {
  Node* popped = nullptr;
  Node** tailPopped = &popped;
  while(head && head->wakeupTick <= now) {
    Node* n = head;
    head = head->next;
    n->next = nullptr;
    *tailPopped = n;
    tailPopped = &n->next;
  }
  return popped;
}

void Sleep::wakeup() {
  ticksNow++;
  Node* toWakeup = Sleep::pop(ticksNow);
  Node* curr = toWakeup;
  while(curr) {
    TCB* t = curr->thread;
    t->state = ThreadNS::READY;
    Scheduler::put(t);

    Node* next = curr->next;
    delete curr;
    curr = next;
  }
}

void Sleep::sleep(ThreadNS::TCB* t, uint64 ticks) {
  if (ticks == 0) return;
  t->state = ThreadNS::SLEEPING;
  Node* n = new Node(t, ticksNow + ticks, nullptr);
  insert(n);
}
