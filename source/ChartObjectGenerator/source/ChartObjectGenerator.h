#pragma once

#include "crossdllinterface\TaskInterface.h"
#include "Modules\IsolineGenerator.h"
#include "Modules\CoverageGenerator.h"
#include "Modules\ZoneGenerator.h"
#include "common/chart_object.h"
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
      ChartObjectGenerator(central_pack *pack, navigation_dispatcher::iComService* service);
      void Release() override final { delete this; }
   private:
      virtual bool processCommand() override final;
   protected:
      bool readFromSource(data_standart::iSurfaceVieverGenMapDataStandart*);
      bool writeToDestination(data_standart::iChartObjectDataStandart*);

      bool generateStatic();
      void prepareLocalStorage();
   private:
      bool m_lock = false;

      IsolineGenerator m_isolineGenerator;
      ZoneGenerator m_zoneGenerator;
      CoverageGenerator m_coverageGenerator;

      chart_object::chart_object_unit_vct m_chartStorage;

      chart_object::chart_object_unit_vct m_staticObjectStorage;
      chart_object::chart_object_unit_vct m_dynamicObjectStorage;

      chart_object::chart_object_unit_vct m_chartObjVct;

      //colreg::chart_objects_ref m_chartObjectRef;
      pathfinder::GeoMatrix m_rawDataPtr;
   };
}