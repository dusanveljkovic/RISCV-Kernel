#include "../h/console.hpp"
#include "../h/hw.h"
#include "../h/helpers.hpp"

#include "../h/trap.hpp"
#include "../h/syscall_c.h"
#include "../h/thread.hpp"
#include "../h/mem_allocator.hpp"
#include "../h/scheduler.hpp"

// extern void userMain();
extern void memTest();

void kmain(void*) {
  // Scheduler::show();
  // userMain();
  // memTest();
  // semaphoreTestCPP();

}

static thread_t userThread;
static thread_t consoleThread;

int main() {
  Trap::trapInit();
  MemoryAllocator::initialize();
  ThreadNS::initMainThread();
  consoleInit();

  thread_create(&consoleThread, outputWorkerBody, nullptr);
  thread_create(&userThread, kmain, nullptr);
  // memTest();

  thread_exit();
  return 0;
}
