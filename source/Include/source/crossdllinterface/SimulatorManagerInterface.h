#pragma once

#include "SimulatorInterface.h"

namespace surface_simulation
{
   struct iSimulatorManager
   {
      virtual void Init() = 0;
      virtual iSimulator* Get(const char* filename) = 0;
   };
}