#pragma once

#include "crossdllinterface/DataShareInterface.h"
#include "common/central_class.h"

namespace data_share
{
   class ShareProviderImpl : public iDataShareProvider, public Central
   {
   public:
      void Init(central_pack* pack) override final { Central::Init(pack); }
      void Share(const wchar_t*, const std::vector<std::vector<double>>& data);
      void GetShared(const wchar_t*, std::vector<std::vector<double>>& data);
      void Release() override { delete this; }
   };
}