#pragma once

#include <functional>
#include "common/chart_object.h"

namespace chart_object
{
   struct iGeneratorModule
   {
      virtual bool SetAdder(std::function<geometry_chart_object&()>, std::function<void(geometry_chart_object&)>) = 0;
   };
}