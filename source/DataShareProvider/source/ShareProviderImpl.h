#pragma once

#include "crossdllinterface/DataShareInterface.h"

namespace data_share
{
   class ShareProviderImpl : public iDataShareProvider
   {
   public:
      void Share(share_meta&, const double**);
      void GetShared(const share_meta&, double***);
      void Release() override { delete this; }
   };
}