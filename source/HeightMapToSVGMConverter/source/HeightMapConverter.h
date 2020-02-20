#pragma once
#include "crossdllinterface\ConverterInterface.h"
#include "crossdllinterface\SVGMDatabaseInterface.h"
#include "colreg/ModuleGuard.h"

namespace converter
{
   class HeightMapConverter : public iConverter
   {
   public:
      HeightMapConverter(const colreg::ModuleGuard<database::iSVGMDatabaseController> db);

      bool Convert(const char* srcPath, const char* dstPath) override;
   private:
      void readDataFromPng(const char* srcPath);
      void convertToDatabaseFormat();
   private:
      const colreg::ModuleGuard<database::iSVGMDatabaseController>& m_databaseController;
      png_bytep* m_row_pointers;
   };
}