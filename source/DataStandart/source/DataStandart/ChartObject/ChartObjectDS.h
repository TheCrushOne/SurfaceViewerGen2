#pragma once

#include "datastandart\ChartObjectDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "DataStandart\DataStandartStruct.h"

namespace data_standart
{
   class ChartObjectDataStandart
      : public iChartObjectDataStandart
      , public DataStandart<DataStandartType::DST_OBJ, chart_object_data_standart>
   {
   public:
      ChartObjectDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
         : DataStandart(pack, base_folder, pService)
      {}
   public:
      // Common
      // Read
      // Write
      void SaveData() override final;
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
      LPCWSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   };
}