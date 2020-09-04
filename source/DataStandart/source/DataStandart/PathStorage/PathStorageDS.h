#pragma once

#include "datastandart\PathStorageDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "datastandart\DataStandartStruct.h"

namespace data_standart
{
   

   //template<int DSType = DataStandartType::DST_PATHS, typename DSStruct = path_storage_data_standart>
   class PathStorageDataStandart
      : public iPathStorageDataStandart
      , public DataStandart<DataStandartType::DST_PATHS, path_storage_data_standart>
   {
      struct tag
      {
         static constexpr char air_routes[] = "air_routes";
         static constexpr char land_routes[] = "land_routes";

         static constexpr char start[] = "start";
         static constexpr char finish[] = "finish";

         static constexpr char control_point_list[] = "control_point_list";
         static constexpr char route_list[] = "route_list";
      };
   public:
      PathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
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
      size_t GetDataHash() override final;
      // Read
      // Write

      // iSurfaceViewerGenMapDataStandart
      // Common
      // Read
      // Write
      void SetData(const pathfinder::route_data& paths) override final;
      const pathfinder::route_data& GetData() override final;
   private:
      // Common
      std::string getDataFilePath() { return std::string(getPath()) + "\\pathdata.pl"; }
      // Read
      void readPathData();
      // Write
      void savePathData();
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      pathfinder::route_data m_paths;
   };
}