#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/research_structs.h"
#include "common/header_collector.h"
#include "common/central_class.h"

namespace SV::engine
{
   struct iEngine : iReleasable
   {
      virtual void ProcessPathFind(const pathfinder::path_finder_indata&, const pathfinder::GeoMatrix&, std::shared_ptr<settings::application_settings>, std::function<void(void)>) = 0;
      virtual const pathfinder::route_data& GetLastProcessedPaths() const = 0;
      virtual void LaunchResearch(std::function<void(void)>) = 0;
      virtual const research::TimeResearchComplexStorage& GetTimeResearchResult() const = 0;
      virtual const research::LengthResearchComplexStorage& GetLengthResearchResult() const = 0;
      virtual const research::ThreadResearchComplexStorage& GetThreadResearchResult() const = 0;
   };
}

#ifdef ENGINE_EXPORTS
#define ENGEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define ENGEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" ENGEXPRTIMPRT SV::engine::iEngine * CreateEngine(SV::central_pack*);