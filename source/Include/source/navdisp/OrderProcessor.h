#pragma once

#include "OrderInterface.h"

namespace navigation_dispatcher
{
   struct iOrderProcessor
   {
      virtual void AddOrder(iOrderPtr command) = 0;
      virtual void ClearOrders() = 0;

      virtual bool ProcessOrders(LPCWSTR begCommandName = NULL) = 0;
   };
}