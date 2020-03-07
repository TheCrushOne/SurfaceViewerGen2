#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/simulation_structs.h"

namespace engine
{
   struct iEngine : colreg::iReleasable
   {
      virtual void ProcessPathFind(ColregSimulation::scenario_data& scenarioData, const std::vector<std::vector<double>>& rawData) = 0;
   };
}

#ifdef ENGINE_DLL
#define ENGEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define ENGEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" ENGEXPRTIMPRT engine::iEngine * CreateEngine();