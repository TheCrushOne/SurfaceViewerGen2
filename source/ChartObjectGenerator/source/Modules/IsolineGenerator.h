#pragma once

#include "common/central_class.h"
#include "chart_storage.h"
#include "ModuleBase.h"

namespace chart_object
{
   class IsolineGenerator : public ModuleBase
   {
   public:
      IsolineGenerator() {}
      void GenerateIsolines(const converter::raw_data_ref& rawdata);
   private:
      void generateIsolineLevel(const converter::raw_data_ref& rawdata, double height, int H);
   private:
      double m_maxRadius;
   };
}