#pragma once
#include "crossdllinterface\TaskInterface.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "crossdllinterface\SettingsSerializerInterface.h"
#include "crossdllinterface\UnitDataSerializerInterface.h"
#include "colreg/ModuleGuard.h"
#include "common/converter_structs.h"
#include "datastandart\DataStandart.h"
#include "datastandart\PngHeightMapDataStandartInterface.h"
#include "datastandart\SVGenMapDataStandartInterface.h"
#include "navdisp\OrderBase.h"
#include "navdisp\OrderStruct.h"

#include <png.h>

namespace converter
{
   class HeightMapConverter
      : public navigation_dispatcher::OrderBase<navigation_dispatcher::OrderType::OT_PNGHMCONVERT, navigation_dispatcher::png_hm_convert_order>
   {
   public:
      HeightMapConverter(central_pack* pack, navigation_dispatcher::iComService* pService);
      void Release() override final { delete this; }
   private:
      virtual bool processCommand() override final;
   private:
      bool readFromSource(data_standart::iPngHeightMapDataStandart*);
      bool writeToDestination(data_standart::iSurfaceVieverGenMapDataStandart*);
      bool processData();
   private:
      colreg::ModuleGuard<database::iSVGMDatabaseController> m_databaseController;
      colreg::ModuleGuard<colreg::iSettingsSerializer> m_settingsSerializer;
      colreg::ModuleGuard<colreg::iUnitDataSerializer> m_unitDataSerializer;
      
      bool m_lock = false;

      std::vector<std::vector<double>> m_rawData;
      raw_data_ref m_rawDataRef;
      data_standart::png_data m_srcRawData;
      pathfinder::GeoMatrix m_dstRawData;
      //settings::unit_source_data m_unitData;
   };
}