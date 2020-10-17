#pragma once

#include "datastandart\ChartObjectDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "DataStandart\DataStandartStruct.h"

namespace SV::data_standart
{
   class ChartObjectDataStandart
      : public iChartObjectDataStandart
      , public DataStandart<DataStandartType::DST_OBJ, chart_object_data_standart>
   {
      struct tag
      {
         static constexpr char static_obj[] = "static_obj";
         static constexpr char dynamic_obj[] = "dynamic_obj";

         static constexpr char type[] = "type";
         static constexpr char prop_list[] = "prop_list";
         static constexpr char contour_list[] = "contour_list";

         static constexpr char key[] = "key";
         static constexpr char val[] = "val";

         static constexpr char lat[] = "lat";
         static constexpr char lon[] = "lon";
      };
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
      size_t GetDataHash() override final { return getDataHash(); }
      // Read
      // Write

      // iSurfaceViewerGenMapDataStandart
      // Common
      // Read
      const geom_object_vec_junc& GetData() override final;
      // Write
      void SetData(const chart_object::chart_object_unit_vct&, const chart_object::chart_object_unit_vct&) override final;
   private:
      // Common
      std::string getDataFilePath() override final { return std::string(getPath()) + "\\chart_objects.co"; }
      // Read
      void readObjectData();
      // Write
      void saveObjectData();
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      geom_object_vec_junc m_objectVecJunc;
   };
}