#pragma once
#include "crossdllinterface\ConverterInterface.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "crossdllinterface\SettingsSerializerInterface.h"
#include "crossdllinterface\UnitDataSerializerInterface.h"
#include "colreg/ModuleGuard.h"

#include <png.h>

namespace converter
{
   class HeightMapConverter : public iConverter, public Communicable
   {
   public:
      HeightMapConverter();

      bool Init(ICommunicator* comm);

      bool Convert(const file_utils::file_storage_base& src, const file_utils::file_storage_base& dst) override final;
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
      settings::application_settings m_settings;
   };
}