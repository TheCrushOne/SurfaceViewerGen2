#pragma once

#include "common/communicator.h"
#include "SurfaceInterfaces.h"
#include "common/research_structs.h"
#include "common/central_class.h"

namespace logger
{
   struct iUniversalLogger : colreg::iReleasable
   {
      virtual void Init(std::shared_ptr<central_pack>) = 0;
      virtual bool LogThreadResearchResult(const ThreadResearchComplexStorage&) = 0;
   };
}

#ifdef UNILOGGER_DLL
#define UNILOGGEREXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define UNILOGGEREXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" UNILOGGEREXPRTIMPRT logger::iUniversalLogger * CreateUniversalLogger();