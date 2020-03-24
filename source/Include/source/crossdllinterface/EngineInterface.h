#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/simulation_structs.h"
#include "common/research_structs.h"
#include "common/settings.h"
#include "common/central_class.h"

namespace engine
{
   struct iEngine : colreg::iReleasable
   {
      virtual void Init(std::shared_ptr<central_pack>) = 0;
      virtual void ProcessPathFind(const ColregSimulation::scenario_data&, const std::vector<std::vector<double>>&, std::function<void(void)>) = 0;
      virtual const pathfinder::route_data& GetLastProcessedPaths() const = 0;
      virtual void LaunchResearch() = 0;
      virtual const TimeResearchComplexStorage& GetTimeResearchResult() const = 0;
      virtual const LengthResearchComplexStorage& GetLengthResearchResult() const = 0;
      virtual const ThreadResearchComplexStorage& GetThreadResearchResult() const = 0;
   };
}

#ifdef ENGINE_DLL
#define ENGEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define ENGEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" ENGEXPRTIMPRT engine::iEngine * CreateEngine();