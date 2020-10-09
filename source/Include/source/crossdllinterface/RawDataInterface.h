#pragma once

#include "SurfaceInterfaces.h"
#include "common/central_class.h"

namespace raw_data
{
   struct iRawDataController : colreg::iReleasable
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


extern "C" RDATAEXPRTIMPRT raw_data::iRawDataController * CreateRawDataController(central_pack_ptr);