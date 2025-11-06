#include "../h/console.hpp"
#include "../h/hw.h"
#include "../h/syscall_c.h"

#define CONSOLE_BUFFER_SIZE 256

typedef struct Buffer {
  int head;
  int tail;
  char buf[CONSOLE_BUFFER_SIZE];

  void  init()  { head = tail = 0; }
  bool  empty() { return head == tail; }
  bool  full()  { return ((tail + 1) % CONSOLE_BUFFER_SIZE) == head; }
  int   count() { return (tail - head + CONSOLE_BUFFER_SIZE) % CONSOLE_BUFFER_SIZE; }
  bool  push(char c) {
    if(full()) return false;
    buf[tail] = c;
    tail = (tail + 1) % CONSOLE_BUFFER_SIZE;
    return true;
  }
  bool pop(char& c) {
    if (empty()) return false;
    c = buf[head];
    head = (head + 1) % CONSOLE_BUFFER_SIZE;
    return true;
  }
}Buffer;

static Buffer inputBuffer;
static Buffer outputBuffer;

static sem_t inputSemaphore;
static sem_t outputSpaceSemaphore;
static sem_t dataSemaphore;

static inline uint8 readConsoleStatus() {
  return *(volatile uint8*)CONSOLE_STATUS;
}

void outputWorkerBody(void*) {
  while (true) {
    char c;
    while(outputBuffer.empty()) {thread_dispatch();}
    bool ok = outputBuffer.pop(c);
    while(!(readConsoleStatus() & CONSOLE_TX_STATUS_BIT)) {thread_dispatch();}
    *(volatile char*)CONSOLE_TX_DATA = c;
    sem_signal(outputSpaceSemaphore);
    // uint8 status = readConsoleStatus();
    // while(status & CONSOLE_TX_STATUS_BIT) {
    //   char c;
    //   bool ok = outputBuffer.pop(c);
    //   if(!ok) break;
    //
    //   *(volatile uint8*)CONSOLE_TX_DATA = (uint8) c;
    //
    //   sem_signal(outputSpaceSemaphore);
    //   status = readConsoleStatus();
    // }
    // thread_dispatch();
  }
}

void consoleInit() {
  inputBuffer.init();
  outputBuffer.init();
  sem_open(&inputSemaphore, 0);
  sem_open(&outputSpaceSemaphore, CONSOLE_BUFFER_SIZE - 1);
  sem_open(&dataSemaphore, 0);
}


void consoleHandler() {
  uint8 status = readConsoleStatus();
  while(status & CONSOLE_RX_STATUS_BIT) {
    char c = *(volatile uint8*)CONSOLE_RX_DATA;
    bool ok = inputBuffer.push(c);
    if(ok)
      sem_signal(inputSemaphore);
    status = readConsoleStatus();
  }
}

char consoleGETC() {
  sem_wait(inputSemaphore);
  char c;
  bool ok = inputBuffer.pop(c);
  if(!ok) return -1;
  return c;
}

int consolePUTC(char c) {
  sem_wait(outputSpaceSemaphore);

  bool ok = outputBuffer.push(c);
  if(!ok) {
    sem_signal(outputSpaceSemaphore);
    return -1;
  }

  sem_signal(dataSemaphore);
  return 0;
}




