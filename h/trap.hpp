#ifndef _TRAP_HPP
#define _TRAP_HPP

#include "hw.h"
#include "thread.hpp"

extern "C" void trapEntry();

extern "C" void handleSupervisorTrap(uint64 code, uint64 a1, uint64 a2, uint64 a3, uint64 a4, ThreadNS::TrapFrame* savedTF);
namespace Trap {
  void trapInit();
}

#endif  
