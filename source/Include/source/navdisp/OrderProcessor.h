#pragma once

#include "OrderInterface.h"

namespace SV::navigation_dispatcher
{
   struct iOrderProcessor
   {
      virtual void AddOrder(iOrder* command) = 0;
      virtual void ClearOrders() = 0;

      virtual bool ProcessOrders(LPCSTR begCommandName = NULL) = 0;
   };
}