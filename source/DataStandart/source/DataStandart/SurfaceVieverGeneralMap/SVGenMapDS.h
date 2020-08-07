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
      // Common
      //void SetData(const pathfinder::GeoMatrix&, const svgm_meta&) override final;
      //pathfinder::GeoMatrix* GetData() override final { return m_rawdata.Get(); }
      // Read
      // Write
   private:
      // Common
      std::wstring getMetaFilePath() { return m_baseFolderPath + L"\\heigthmap.meta"; }
      std::wstring getDataFilePath() { return m_baseFolderPath + L"\\heigthmap.hdat"; }
      // Read
      void readMetaData();
      void readHeightData();
      // Write
      void saveMetaData();
      void saveHeightData();
   protected:
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
   private:
      // NOTE: путь до каталога
      std::wstring m_baseFolderPath;
      svgm_meta m_meta;
      TransitPtr<pathfinder::GeoMatrix> m_rawdata;
   };
}
