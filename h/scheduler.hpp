#ifndef _SCHEDULER_HPP
#define _SCHEDULER_HPP

#include "thread.hpp"
#include "helpers.hpp"

class Scheduler {
public:
  static ThreadList*  readyHead;
  static ThreadList*  readyTail;

  static ThreadNS::TCB*   get();
  static void             put(ThreadNS::TCB* t);
  static void             show();
};


#endif  
