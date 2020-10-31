#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/research_structs.h"
#include "common/header_collector.h"
#include "common/central_class.h"

namespace SV::checksum
{
   struct iChecksumService : iReleasable
   {
      virtual const char* CalcHash(const char* filename) = 0;
   };
}

#ifdef CHECKSUMSERVICE_EXPORTS
#define CHCKEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHCKEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHCKEXPRTIMPRT SV::checksum::iChecksumService * CreateChecksumService(SV::central_pack*);