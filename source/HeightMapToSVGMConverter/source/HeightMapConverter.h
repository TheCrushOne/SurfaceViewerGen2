#pragma once
#include "crossdllinterface\ConverterInterface.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "colreg/ModuleGuard.h"

#include <png.h>

namespace converter
{
   class HeightMapConverter : public iConverter
   {
   public:
      HeightMapConverter();

      const double** Convert(const file_utils::file_storage_base& src, const file_utils::file_storage_base& dst) override;
      void Release() override { delete this; }
   private:
      void readDataFromPng(const char* srcPath);
      void convertToDatabaseFormat();
      void safeReleaseData();
   private:
      colreg::ModuleGuard<database::iSVGMDatabaseController> m_databaseController;
      png_bytep* m_row_pointers;
      bool m_lock = false;
      double** m_rawData;
      database::chart_meta m_currentMeta;
   };
}