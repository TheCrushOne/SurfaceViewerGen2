#pragma once

#include "datastandart\SVGenMapDataStandartInterface.h"
#include "DataStandart\DataStandartBase.h"
#include "DataStandart\DataStandartStruct.h"
#include "colreg/ModuleGuard.h"


namespace SV::data_standart
{
   class SurfaceViewerGenMapDataStandart
      : public iSurfaceVieverGenMapDataStandart
      , public DataStandart<DataStandartType::DST_SVGM, svgm_data_standart>
   {
      struct tag
      {
         static constexpr char row_count[] = "row_count";
         static constexpr char col_count[] = "col_count";
      };
   public:
      SurfaceViewerGenMapDataStandart(central_pack*, LPCWSTR, navigation_dispatcher::iComService*);
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
      pathfinder::GeoMatrix& GetData() override final;
      settings::unit_source_data& GetUnitData() override final;
      // Write
      void SetData(const pathfinder::GeoMatrix&) override final;
   private:
      // Common
      std::string getMetaFilePath() { return std::string(getPath()) + "\\heigthmap.meta"; }
      std::string getDataFilePath() override final { return std::string(getPath()) + "\\heigthmap.hdat"; }
      // Read
      void readMetaData();
      void readHeightData();
      void readUnitData();
      // Write
      void saveMetaData();
      void saveHeightData();
   private:
      void resolvePathDee();
      void mountPathFileStorage();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      // NOTE: путь до каталога
      size_t m_rowCount, m_colCount;
      std::wstring m_baseFolderPath;
      pathfinder::GeoMatrix m_rawdata;
      settings::unit_source_data m_unitData;
      file_utils::global_path_storage m_storage;
   };
}
