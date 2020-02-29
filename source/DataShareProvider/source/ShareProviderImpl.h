#pragma once

#include "crossdllinterface/DataShareInterface.h"

namespace data_share
{
   class ShareProviderImpl : public iDataShareProvider
   {
   public:
      void Share(const wchar_t*, const settings::map_settings&, const double**);
      void GetShared(const wchar_t*, const settings::map_settings&, double***);
      void Release() override { delete this; }
   };
}