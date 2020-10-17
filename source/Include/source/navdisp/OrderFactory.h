#pragma once

#include "common/central_class.h"
#include "OrderInterface.h"

namespace SV::navigation_dispatcher
{
   struct iOrderFactory
   {
      virtual iOrder* CreateOrder(OrderType type, LPCSTR name) = 0;
      virtual void DeleteOrder(LPCSTR id) = 0;

      virtual iOrder* GetOrder(LPCSTR id) const = 0;

      virtual void Clear() = 0;
   };
}