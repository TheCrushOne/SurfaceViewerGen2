#pragma once

#include "datastandart\ResearchResultDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "datastandart\DataStandartStruct.h"

namespace SV::data_standart
{
   class ResearchResultDataStandart
      : public iResearchResultDataStandart
      , public DataStandart<DataStandartType::DST_RESRES, research_result_data_standart>
   {
   public:
      ResearchResultDataStandart(central_pack* pack, LPCSTR base_folder, navigation_dispatcher::iComService* pService)
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
   private:
      // Common
      std::string getDataFilePath() { return std::string(getPath()) + "\\universal.rres"; }
      // Read
      // Write
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   };
}