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
      struct tag
      {
         static constexpr char history[] = "history";

         static constexpr char start_ts[] = "start_ts";
         static constexpr char finish_ts[] = "finish_ts";
         static constexpr char task_idx[] = "task_idx";
         static constexpr char holder_idx[] = "holder_idx";
      };

      struct organized_statistic
      {
         struct holder_data
         {
            struct task_data
            {
               __int64 start_ts;
               __int64 finish_ts;
            };

            std::unordered_map<size_t, task_data> data;
         };

         std::unordered_map<size_t, holder_data> stat;
      };
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

      // iResearchResultDataStandart
      // Common
      // Read
      // Write
      void SetData(const research::statistic_data_history&) override final;
   private:
      // Common
      std::string getDataFilePath() { return std::string(getPath()) + "\\universal.rres"; }
      void reorganizeStatistic();
      // Read
      // Write
      static Json::Value writeStatisticHistory(const organized_statistic& history);
      static Json::Value writeStatisticLine(const organized_statistic::holder_data& statistic);
      static Json::Value writeStatisticStamp(const organized_statistic::holder_data::task_data& statistic);
      void saveStatisticDataToFile();
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      research::statistic_data_history m_statistic;
      organized_statistic m_organizedStatistic;
   };
}