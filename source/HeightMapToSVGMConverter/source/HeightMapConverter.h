#pragma once
#include "crossdllinterface\ConverterInterface.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "crossdllinterface\SettingsSerializerInterface.h"
#include "crossdllinterface\UnitDataSerializerInterface.h"
#include "colreg/ModuleGuard.h"
#include "common/converter_structs.h"

#include <png.h>

namespace converter
{
   class HeightMapConverter : public iConverter, public Central
   {
   public:
      HeightMapConverter();

      void Init(std::shared_ptr<central_pack> pack) override final { Central::Init(pack); }

      bool Convert() override final;
      void Release() override final { delete this; }
   private:
      void readDataFromPng(const char* srcPath);
      void convertToDatabaseFormat();
      void safeReleaseData();
   private:
      colreg::ModuleGuard<database::iSVGMDatabaseController> m_databaseController;
      colreg::ModuleGuard<colreg::iSettingsSerializer> m_settingsSerializer;
      colreg::ModuleGuard<colreg::iUnitDataSerializer> m_unitDataSerializer;
      png_bytep* m_row_pointers;
      bool m_lock = false;

      std::vector<std::vector<double>> m_rawData;
      raw_data_ref m_rawDataRef;

      settings::application_settings m_settings;
      settings::unit_source_data m_unitData;
   };
}