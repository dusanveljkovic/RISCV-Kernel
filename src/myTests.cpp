#include "../h/hw.h"
#include "../h/helpers.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../h/semaphore.hpp"


void memTest() {
  print("\n");
  print("Heap start address", (uint64)HEAP_START_ADDR, '\n');
  print("Heap end address", (uint64)HEAP_END_ADDR, '\n');

  print("Free memory", mem_get_free_space(), '\n', 10);

  int* pint = (int*)mem_alloc(sizeof(int));
  *pint = (int)10;

  print("Allocated int[#]", (uint64)pint);
  print("Allocated int[v]", *pint, '\n', 10);

  print("Free memory", mem_get_free_space(), '\n', 10);

  mem_free(pint);

  print("Free memory", mem_get_free_space(), '\n', 10);

  print("\n");
}

static uint64 expensiveCalc(uint64 start, uint64 steps) {
  uint64 x = start;
  for(uint64 i = 0; i < steps; i++) {
    x = (x * 16625344ULL + 100234225554ULL) ^ (x >> 13);
  }
  return x;
}

class Worker : public Thread {
public:
  Worker(int id, uint64 seed, int iters, uint64 work)
  : Thread(), id(id), seed(seed), iters(iters), work(work) {}
protected:
  void run() override {
    uint64 v = seed;
    for(int i = 0; i < iters; i++) {
      v = expensiveCalc(v, work);
      print("Thread", id);
      print("Work", work, '\n');
      // ThreadNS::dispatch();
    }
  }
private:
  int id;
  uint64 seed;
  int iters;
  uint64 work;
};

struct twoInts {
  int first;
  int second;
  twoInts(int a, int b): first(a), second(b) {};
};
void work(void* arg) {
  twoInts num = *(twoInts*)arg;
  print("Thread", (uint64)ThreadNS::TCB::running, '\t', 16);

  while(num.second > 0) {
    for(int i = 0; i < 1000; i++) {};
    num.second--;
    if(num.second % 10000 == 0) {
      print("Thread", (uint64)ThreadNS::TCB::running, '\t', 16);
      print("Work", num.second, '\n', 10);
      // ThreadNS::dispatch();
    }
  }
  return;
}

struct semAndIter {
  Sem** semHandle;
  uint64 iter;
  semAndIter(Sem** a, uint64 b): semHandle(a), iter(b) {};
};

void sem_work(void* arg) {
  semAndIter s = *(semAndIter*)arg;
  for(uint64 i = 0; i < 5; i++) {
    sem_wait(*(s.semHandle));
    print("", s.iter, '\t', 10);
    print("ulazi u kriticnu sekciju iteracija", i, '\n', 10);
    for(volatile int k = 0; k < 1000000; k++) {
      for(volatile int j = 0; j < 100; j++) {}
    }

    print("", s.iter, '\t', 10);
    print("izlazi iz kriticne sekcije iteracija", i, '\n', 10);
    sem_signal(*(s.semHandle));
  }
  return;
}

class SemWorker : public Thread {
public:
  SemWorker(semAndIter* s): Thread(), s(s) {};
  void run() override {
    sem_work(s);
  }
private:
  semAndIter* s;
};

static Sem* semHandle;

void semaphoreTest() {
  thread_t myHandle;
  sem_open(&semHandle, 1);
  semAndIter* arg = new semAndIter(&semHandle, 1);
  semAndIter* arg2 = new semAndIter(&semHandle, 2);
  semAndIter* arg3 = new semAndIter(&semHandle, 3);
  thread_create(&myHandle, &sem_work, (void*)arg);
  thread_create(&myHandle, &sem_work, (void*)arg2);
  thread_create(&myHandle, &sem_work, (void*)arg3);
  return;
}

void busy(void*) {
  while(true) {};
}

void timerTest() {
  thread_t myHandle;
  thread_create(&myHandle, busy, nullptr);
  print("111");
  time_sleep(20);
  print("222");
}
