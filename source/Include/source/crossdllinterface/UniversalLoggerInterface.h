#pragma once

#include "common/communicator.h"
#include "common/research_structs.h"
#include "common/central_class.h"
#include "colreg/BaseRef.h"

namespace SV::logger
{
   struct iUniversalLogger : iReleasable
   {
      virtual bool LogThreadResearchResult(const research::ThreadResearchComplexStorage&) = 0;
   };
}

#ifdef UNILOGGER_DLL
#define UNILOGGEREXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define UNILOGGEREXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" UNILOGGEREXPRTIMPRT SV::logger::iUniversalLogger * CreateUniversalLogger(SV::central_pack*);