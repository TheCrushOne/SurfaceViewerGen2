#pragma once

#include "common/central_class.h"
#include "chart_storage.h"
#include "ModuleBase.h"

namespace chart_object
{
   class CoverageGenerator : public ModuleBase
   {
   public:
      CoverageGenerator() {}

      void GenerateChartBorder(const converter::raw_data_ref& rawdata);
   private:
   };
}