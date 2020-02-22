#pragma once

#include "SurfaceInterfaces.h"

namespace raw_data
{
   struct iRawDataController : colreg::iReleasable
   {
      virtual void LoadRawData() = 0;
      virtual void SaveRawData() = 0;
   };
}

extern "C" EXPRTIMPRT raw_data::iRawDataController * CreateSQLiteDatabaseController();