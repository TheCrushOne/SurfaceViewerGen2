#pragma once

#include <functional>
#include "chart_storage.h"

namespace chart_object
{
   struct iGeneratorModule
   {
      virtual bool SetAdder(std::function<chart_storage&()>, std::function<void(chart_storage&)>) = 0;
   };
}