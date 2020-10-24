#pragma once

#include "datastandart\PngHeightMapDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "datastandart\DataStandartStruct.h"

#include <png.h>

namespace SV::data_standart
{
   class PngHeightMapDataStandart
      : public iPngHeightMapDataStandart
      , public DataStandart<DataStandartType::DST_PNGHM, png_hm_data_standart>
   {
   public:
      PngHeightMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
         : DataStandart(pack, base_folder, pService)
         , m_file(nullptr)
         , m_row_pointers(nullptr)
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
      void ReleasePngData() override final;
      // Read
      const png_data& GetPngData() const override final;
      // Write
   private:
      // Common
      std::string getDataFilePath() override final { return getPath(); }
      // Read
      // Write
   private:
      void resolvePathDee();
      void readDataFromPng();
      void convertToRawData();
      LPCSTR getPath() override final { return m_dataStandartData.file.c_str(); }
   private:
      mutable bool m_read = false;
      mutable FILE* m_file;
      png_data m_data;
      png_bytep* m_row_pointers = nullptr;
   };
}