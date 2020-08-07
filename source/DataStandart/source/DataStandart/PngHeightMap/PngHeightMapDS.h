#pragma once

#include "datastandart\PngHeightMapDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "datastandart\DataStandartStruct.h"

namespace data_standart
{
   class PngHeightMapDataStandart
      : public iPngHeightMapDataStandart
      , public DataStandart<DataStandartType::DST_PNGHM, png_hm_data_standart>
   {
   public:
      PngHeightMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
         : DataStandart(pack, base_folder, pService)
         , m_file(nullptr)
      {}
   public:
      // Common
      FILE* GetPngData() const override final { return m_file; }
      // Read
      void ReleasePngData() override final;
      // Write
      void WriteToDestination()/* override final */{}
   protected:
      // Common
      
      // Read
      virtual bool open() override final
      {
         resolvePathDee();
         // TODO: check existence
         return true; 
      }
      // Write
      virtual bool create() override final
      {
         resolvePathDee();
         //if (fpath(m_dataSourceData.file).is_exists())
            //return fpath(m_dataSourceData.file).delete_from_disk();
         return true;
      }
   private:
      void resolvePathDee();
      LPCWSTR getPath() override final { return m_dataStandartData.file.c_str(); }
   private:
      FILE* m_file;
   };
}