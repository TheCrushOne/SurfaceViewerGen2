#pragma once

#include "common/central_class.h"
#include "colreg/BaseRef.h"

namespace SV::raw_data
{
   struct iRawDataController : iReleasable
   {
      virtual void LoadRawData() = 0;
      virtual void SaveRawData() = 0;
   };
}

#ifdef RAWDATA_DLL
#define RDATAEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define RDATAEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif


extern "C" RDATAEXPRTIMPRT SV::raw_data::iRawDataController * CreateRawDataController(SV::central_pack*);