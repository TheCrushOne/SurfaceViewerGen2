#pragma once

#include "crossdllinterface/ChartObjectGeneratorInterface.h"
#include "Modules\IsolineGenerator.h"
#include "Modules\CoverageGenerator.h"
#include "Modules\ZoneGenerator.h"
#include "chart_storage.h"

namespace chart_object
{
   class ChartObjectGenerator : public iGenerator, public Central
   {
   public:
      ChartObjectGenerator();

      bool Init(central_pack* pack) override final;
      bool GenerateStatic(const converter::raw_data_ref& rawdata) override final;
      const colreg::chart_objects_ref& GetChartObjects() const override final;
      void Release() override final { delete this; }
   protected:
      void addChartObject(chart_storage& storage);
      chart_storage& generateNew() { m_chartStorage.emplace_back(); return m_chartStorage.back(); }
      void prepareRef() const;
      void prepareLocalStorage();
   private:
      bool m_lock = false;

      IsolineGenerator m_isolineGenerator;
      ZoneGenerator m_zoneGenerator;
      CoverageGenerator m_coverageGenerator;

      std::vector<chart_storage> m_chartStorage;

      std::vector<colreg::chart_object> m_chartObjVct;

      colreg::chart_objects_ref m_chartObjectRef;
   };
}