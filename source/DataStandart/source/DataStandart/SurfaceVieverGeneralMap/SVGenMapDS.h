#pragma once

#include "datastandart\SVGenMapDataStandartInterface.h"
#include "DataStandart\DataStandartBase.h"
#include "DataStandart\DataStandartStruct.h"
#include "colreg/ModuleGuard.h"
#include "crossdllinterface\UnitDataSerializerInterface.h"
#include "crossdllinterface\SettingsSerializerInterface.h"

namespace data_standart
{
   class SurfaceViewerGenMapDataStandart
      : public iSurfaceVieverGenMapDataStandart
      , public DataStandart<DataStandartType::DST_SVGM, svgm_data_standart>
   {
   public:
      SurfaceViewerGenMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
         : DataStandart(pack, base_folder, pService)
      {}
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
      pathfinder::GeoMatrix& GetData() override final;
      settings::unit_source_data& GetUnitData() override final;
      settings::application_settings& GetSettings() override final;
      // Write
      void SetData(const pathfinder::GeoMatrix&) override final;
   private:
      // Common
      std::string getMetaFilePath() { return std::string(getPath()) + "\\heigthmap.meta"; }
      std::string getDataFilePath() { return std::string(getPath()) + "\\heigthmap.hdat"; }
      // Read
      void readMetaData();
      void readHeightData();
      void readUnitData();
      void readSettings();
      // Write
      void saveMetaData();
      void saveHeightData();
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      // NOTE: путь до каталога
      size_t m_rowCount, m_colCount;
      std::wstring m_baseFolderPath;
      pathfinder::GeoMatrix m_rawdata;
      settings::unit_source_data m_unitData;
      settings::application_settings m_settings;
      colreg::ModuleGuard<colreg::iUnitDataSerializer> m_unitDataSerializer;
      colreg::ModuleGuard<colreg::iSettingsSerializer> m_settingsSerializer;
   };
}
