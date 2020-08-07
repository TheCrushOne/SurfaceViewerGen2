#pragma once

#include "crossdllinterface\TaskInterface.h"
#include "Modules\IsolineGenerator.h"
#include "Modules\CoverageGenerator.h"
#include "Modules\ZoneGenerator.h"
#include "chart_storage.h"
#include "datastandart\SVGenMapDataStandartInterface.h"
#include "datastandart\ChartObjectDataStandartInterface.h"
#include "navdisp\OrderBase.h"
#include "navdisp\OrderStruct.h"

namespace chart_object
{
   class ChartObjectGenerator
      : public navigation_dispatcher::OrderBase<navigation_dispatcher::OrderType::OT_GENOBJLIST, navigation_dispatcher::chart_object_gen_order>
   {
   public:
      ChartObjectGenerator(central_pack *pack, navigation_dispatcher::iComService* pService);
      void Release() override final { delete this; }
   private:
      virtual bool processCommand() override final;
   protected:
      bool readFromSource(data_standart::iSurfaceVieverGenMapDataStandart*);
      bool writeToDestination(data_standart::iChartObjectDataStandart*);

      bool generateStatic();
      //const colreg::chart_objects_ref& getChartObjects() const;
      void init();
      void addChartObject(chart_storage& storage);
      chart_storage& generateNew() { m_chartStorage.emplace_back(); return m_chartStorage.back(); }
      //void prepareRef() const;
      void prepareLocalStorage();
   private:
      bool m_lock = false;

      IsolineGenerator m_isolineGenerator;
      ZoneGenerator m_zoneGenerator;
      CoverageGenerator m_coverageGenerator;

      std::vector<chart_storage> m_chartStorage;

      //std::vector<colreg::chart_object> m_chartObjVct;

      //colreg::chart_objects_ref m_chartObjectRef;
      pathfinder::GeoMatrix* m_rawDataPtr = nullptr;
   };
}