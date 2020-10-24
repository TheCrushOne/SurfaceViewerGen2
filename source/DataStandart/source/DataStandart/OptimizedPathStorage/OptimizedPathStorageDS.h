#pragma once

#include "datastandart\OptimizedPathStorageDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "datastandart\DataStandartStruct.h"

namespace SV::data_standart
{
   class OptimizedPathStorageDataStandart
      : public iPathStorageDataStandart
      , public DataStandart<DataStandartType::DST_OPTPATHS, optimized_path_storage_data_standart>
   {
   public:
      OptimizedPathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
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
      // Write
      void SetData(const pathfinder::route_data& paths) override final;
      const pathfinder::route_data& GetData() override final;
   private:
      // Common
      std::string getDataFilePath() override final { return std::string(getPath()) + "\\optpathdata.pl"; }
      // Read
      // Write
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      pathfinder::route_data m_paths;
   };
}