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
      struct meta
      {
         size_t row_count;
         size_t col_count;
      };
   public:
      OptimizedPathStorageDataStandart(central_pack* pack, LPCSTR base_folder, navigation_dispatcher::iComService* pService)
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
      const pathfinder::route_data& GetData() const override final;
      const pathfinder::UnsignedMatrix& GetLandUnitExplication() const override final;
      const pathfinder::UnsignedMatrix& GetAirUnitExplication() const override final;
      const std::vector<pathfinder::UnsignedMatrix>& GetCoverageHistory() const override final;
      // Write
      void SetData(const pathfinder::route_data& paths, const pathfinder::UnsignedMatrix& land, const pathfinder::UnsignedMatrix& air, const std::vector<pathfinder::SharedUnsignedMatrix>& coverages) override final;
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
      pathfinder::UnsignedMatrix m_landExplication;
      pathfinder::UnsignedMatrix m_airExplication;
      std::vector<pathfinder::UnsignedMatrix> m_coverageHistory;
      meta m_meta;
   };
}