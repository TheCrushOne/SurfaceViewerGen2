#pragma once

#include "common/central_class.h"
#include "OrderInterface.h"

namespace navigation_dispatcher
{
   struct iOrderFactory
   {
      virtual iOrderPtr CreateOrder(OrderType type) = 0;
   };
}