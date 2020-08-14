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
      // iDataStandart
      // Common
      virtual bool Create() override final
      {
         resolvePathDee();
         //if (fpath(m_dataSourceData.file).is_exists())
            //return fpath(m_dataSourceData.file).delete_from_disk();
         return true;
      }
      bool DeserializeAttrs(const xml_properties::PropertyItem& standart) override final { return deserializeAttrs(standart); }
      LPCSTR GetPath() override { return getPath(); }
      DataStandartType GetType() const override { return getType(); }
      void Release() override final { delete this; }
      // Read
      // Write

      // iSurfaceViewerGenMapDataStandart
      // Common
      // Read
      // Write
      void SaveData() override final;
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   };
}