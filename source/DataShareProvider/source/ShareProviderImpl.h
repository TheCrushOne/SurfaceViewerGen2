#pragma once

#include "crossdllinterface/DataShareInterface.h"
#include "common/central_class.h"

namespace SV::data_share
{
   class ShareProviderImpl : public iDataShareProvider, public Central
   {
   public:
      ShareProviderImpl(central_pack* pack)
         : Central(pack)
      {}
      void Share(const wchar_t*, const std::vector<std::vector<double>>& data);
      void GetShared(const wchar_t*, std::vector<std::vector<double>>& data);
      void Release() override { delete this; }
   };
}