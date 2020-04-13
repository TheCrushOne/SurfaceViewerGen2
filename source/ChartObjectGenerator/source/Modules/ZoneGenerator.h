#pragma once

#include "common/central_class.h"
#include "chart_storage.h"
#include "ModuleBase.h"

namespace chart_object
{
   class ZoneGenerator : public ModuleBase
   {
   public:
      ZoneGenerator() {}

      void GenerateNoGoAreas(const converter::raw_data_ref& rawdata);
      void GenerateNoFlyAreas(const converter::raw_data_ref& rawdata);
   private:
   };
}