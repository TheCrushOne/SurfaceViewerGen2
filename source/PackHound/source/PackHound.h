#pragma once

#include "crossdllinterface\TaskInterface.h"
#include "common\central_class.h"
#include "navdisp\OrderBase.h"
#include "navdisp\OrderStruct.h"

namespace SV::pack_hound
{
   class PackHound
      : public navigation_dispatcher::OrderBase<navigation_dispatcher::OrderType::OT_PACKHOUND, navigation_dispatcher::packhound_order>
   {
   public:
      PackHound(central_pack* pack, navigation_dispatcher::iComService* pService);
      void Release() override final { delete this; }
   private:
      virtual bool processCommand() override final;
   };
}