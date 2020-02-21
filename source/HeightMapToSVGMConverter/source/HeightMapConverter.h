#pragma once
#include "crossdllinterface\ConverterInterface.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "colreg/ModuleGuard.h"

namespace converter
{
   class HeightMapConverter : public iConverter
   {
   public:
      HeightMapConverter();

      bool Convert(const char* srcPath, const char* dstPath) override;
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