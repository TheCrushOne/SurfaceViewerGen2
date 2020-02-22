#pragma once

#include "SurfaceInterfaces.h"
#include "common/share_types.h"

namespace data_share
{
   struct iDataShareProvider : colreg::iReleasable
   {
      virtual void Share(share_meta&, const double**) = 0;
      virtual void GetShared(const share_meta&, double***) = 0;
   };
}

extern "C" EXPRTIMPRT data_share::iDataShareProvider * CreateDataShareProvider();