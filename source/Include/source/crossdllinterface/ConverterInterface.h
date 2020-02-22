#pragma once

#include "SurfaceInterfaces.h"
#include "common/file_storage.h"

namespace converter
{
   struct iConverter : colreg::iReleasable
   {
      virtual const double** Convert(const file_utils::file_storage_base& src, const file_utils::file_storage_base& dst) = 0;
   };
}

extern "C" EXPRTIMPRT converter::iConverter * CreateConverter();