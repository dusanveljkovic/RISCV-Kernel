#ifndef _SEMAPHORE_HPP
#define _SEMAPHORE_HPP

#include "helpers.hpp"

class Sem {
public:
  Sem(int init = 1): value(init), blockedHead(nullptr), blockedTail(nullptr), closed(false) {};

  static int create(Sem** handle, int init);
  
  int wait();
  int signal();
  int close();

private:
  int value;
  ThreadList* blockedHead;
  ThreadList* blockedTail;
  bool closed;
};


#endif  
