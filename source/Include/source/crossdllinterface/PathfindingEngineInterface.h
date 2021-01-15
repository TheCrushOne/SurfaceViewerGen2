#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/research_structs.h"
#include "common/header_collector.h"
#include "common/central_class.h"

namespace SV::engine
{
   struct iPathfindingEngine : iReleasable
   {
      virtual void ProcessPathFind(const pathfinder::path_finder_indata&, const pathfinder::GeoMatrix&, std::shared_ptr<settings::application_settings>, std::function<void(void)>) = 0;
      virtual const pathfinder::route_data& GetLastProcessedPaths() const = 0;
      virtual const pathfinder::UnsignedMatrix& GetLandUnitExplication() const = 0;
      virtual const pathfinder::UnsignedMatrix& GetAirUnitExplication() const = 0;
      virtual const std::vector<pathfinder::SharedUnsignedMatrix>& GetCoverageHistory() const = 0;
   };
}

#ifdef ENGINE_EXPORTS
#define ENGEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define ENGEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" ENGEXPRTIMPRT SV::engine::iPathfindingEngine * CreatePathfindingEngine(SV::central_pack*);