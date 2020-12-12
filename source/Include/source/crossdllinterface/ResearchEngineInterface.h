#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/research_structs.h"
#include "common/header_collector.h"
#include "common/central_class.h"
#include "common/statistic_types.h"

namespace SV::engine
{
   struct iResearchEngine : iReleasable
   {
      virtual void LaunchResearch(std::function<void(void)>, std::shared_ptr<settings::application_settings>) = 0;
      virtual const research::TimeResearchComplexStorage& GetTimeResearchResult() const = 0;
      virtual const research::LengthResearchComplexStorage& GetLengthResearchResult() const = 0;
      virtual const research::ThreadResearchComplexStorage& GetThreadResearchResult() const = 0;
      virtual const research::statistic_data_history& GetStatisticHistory() const = 0;
   };
}

#ifdef ENGINE_EXPORTS
#define ENGEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define ENGEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" ENGEXPRTIMPRT SV::engine::iResearchEngine * CreateResearchEngine(SV::central_pack*);