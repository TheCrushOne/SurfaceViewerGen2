#pragma once

#include "common/communicator.h"
#include "colreg/BaseRef.h"
#include "common/pathfinder_structs.h"
#include "common/simulation_structs.h"
#include "common/research_structs.h"
#include "common/header_collector.h"
#include "common/central_class.h"

namespace checksum
{
   struct iChecksumService : colreg::iReleasable
   {
      virtual const char* CalcHash(const char* filename) = 0;
   };
}

#ifdef CHECKSUMSERVICE_EXPORTS
#define CHCKEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHCKEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHCKEXPRTIMPRT checksum::iChecksumService * CreateChecksumService(central_pack*);