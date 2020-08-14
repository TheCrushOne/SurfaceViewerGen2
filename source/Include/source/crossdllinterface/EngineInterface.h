#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/simulation_structs.h"
#include "common/research_structs.h"
#include "common/header_collector.h"
#include "common/central_class.h"

namespace engine
{
   struct iEngine : colreg::iReleasable
   {
      virtual void ProcessPathFind(const ColregSimulation::scenario_data&, const pathfinder::GeoMatrix&, std::function<void(void)>) = 0;
      virtual const pathfinder::route_data& GetLastProcessedPaths() const = 0;
      virtual void LaunchResearch(std::function<void(void)>) = 0;
      virtual const TimeResearchComplexStorage& GetTimeResearchResult() const = 0;
      virtual const LengthResearchComplexStorage& GetLengthResearchResult() const = 0;
      virtual const ThreadResearchComplexStorage& GetThreadResearchResult() const = 0;
   };
}

#ifdef ENGINE_EXPORTS
#define ENGEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define ENGEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" ENGEXPRTIMPRT engine::iEngine * CreateEngine(central_pack*);