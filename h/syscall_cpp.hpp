#ifndef _SYSCALL_CPP_HPP
#define _SYSCALL_CPP_HPP

#include "hw.h"
#include "syscall_c.h"

class Thread {
public:
  Thread (void (*body)(void*), void* arg): body(body), arg(arg), myHandle(nullptr) {};
  virtual ~Thread() {};

  int start() {
    if (body != nullptr) 
      return thread_create(&myHandle, body, arg);
    else
      return thread_create(&myHandle, &Thread::threadWrapper, this);
  }
  static void dispatch()        { thread_dispatch(); };
  static int  sleep (time_t t)  { return time_sleep(t); };

protected:
  Thread() : body(nullptr), arg(nullptr), myHandle(nullptr) {};
  virtual void run() = 0;

private:
  static void threadWrapper(void* obj) {
    Thread* t = (Thread*)obj;
    t->run();
  }
  void        (*body)(void*);
  void*       arg;
  thread_t    myHandle;
};

class Semaphore {
public:
  Semaphore(unsigned init = 1): myHandle(nullptr)  { sem_open(&myHandle, init); };
  virtual ~Semaphore ()         { sem_close(myHandle); };

  int   wait()    { return sem_wait(myHandle); };
  int   signal()  { return sem_signal(myHandle); };

private:
  sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
  void terminate () { this->period = 0; };
protected:
  PeriodicThread (time_t p): period(p) {};
  virtual void periodicActivation () {};
  void run() override {
    while(this->period) {
      Thread::sleep(period);
      this->periodicActivation();
    }
  }
private:
  time_t period;
};

class Console {
public:
  static char getc ()       { return ::getc(); };
  static void putc (char c) { ::putc(c); };
};

#endif  
