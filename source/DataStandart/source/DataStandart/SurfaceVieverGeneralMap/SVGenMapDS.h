#pragma once

#include "datastandart\SVGenMapDataStandartInterface.h"
#include "DataStandart\DataStandartBase.h"
#include "DataStandart\DataStandartStruct.h"

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
      // Write
      void SetData(const pathfinder::GeoMatrix&) override final;
   private:
      // Common
      std::string getMetaFilePath() { return std::string(getPath()) + "\\heigthmap.meta"; }
      std::string getDataFilePath() { return std::string(getPath()) + "\\heigthmap.hdat"; }
      // Read
      void readMetaData();
      void readHeightData();
      // Write
      void saveMetaData();
      void saveHeightData();
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      // NOTE: ���� �� ��������
      size_t m_rowCount, m_colCount;
      std::wstring m_baseFolderPath;
      pathfinder::GeoMatrix m_rawdata;
   };
}
