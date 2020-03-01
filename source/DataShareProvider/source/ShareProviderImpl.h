#pragma once

#include "crossdllinterface/DataShareInterface.h"

namespace data_share
{
   class ShareProviderImpl : public iDataShareProvider
   {
   public:
      void Share(const wchar_t*, const settings::map_settings&, const std::vector<std::vector<double>>& data);
      void GetShared(const wchar_t*, const settings::map_settings&, std::vector<std::vector<double>>& data);
      void Release() override { delete this; }
   };
}