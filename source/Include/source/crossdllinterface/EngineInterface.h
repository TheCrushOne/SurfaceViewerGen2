#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/simulation_structs.h"
#include "common/settings.h"

namespace engine
{
   struct iEngine : colreg::iReleasable
   {
      virtual void Init(ICommunicator*) = 0;
      virtual void ProcessPathFind(const ColregSimulation::scenario_data&, const std::vector<std::vector<double>>&, std::function<void(void)>) = 0;
      virtual const pathfinder::route_data& GetLastProcessedPaths() const = 0;
      virtual void LaunchResearch(const settings::research_settings&) = 0;
   };
}

#ifdef ENGINE_DLL
#define ENGEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define ENGEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" ENGEXPRTIMPRT engine::iEngine * CreateEngine();