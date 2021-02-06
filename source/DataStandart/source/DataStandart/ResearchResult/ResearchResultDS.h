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
         static constexpr char data[] = "data";

         static constexpr char task_log[] = "task_log";
         static constexpr char packet_log[] = "packet_log";
         static constexpr char run_log[] = "run_log";
         static constexpr char pipeline_run_log[] = "pipeline_run_log";
         static constexpr char overall_log[] = "overall_log";

         static constexpr char unit_count[] = "unit_count";

         static constexpr char count[] = "count";

         static constexpr char start_ts[] = "start_ts";
         static constexpr char finish_ts[] = "finish_ts";
         static constexpr char task_idx[] = "task_idx";
         static constexpr char holder_idx[] = "holder_idx";
         static constexpr char unit_idx[] = "unit_idx";
         static constexpr char shard_idx[] = "shard_idx";
         static constexpr char packet_idx[] = "packet_idx";
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
      void SetData(const research::task_holder_statistic::mcmanager_overall_log&, bool) override final;
   private:
      // Common
      std::string getDataFilePath()
      {
         time_t rawtime;
         struct tm* timeinfo;
         wchar_t buffer[80];

         time(&rawtime);
         timeinfo = localtime(&rawtime);

         wcsftime(buffer, sizeof(buffer), L"%d%m%Y_%H%M%S_", timeinfo);
         std::wstring str(buffer);
         return std::string(getPath()) + "\\" + SVGUtils::wstringToString(str) + "universal.rres";
      }
      void reorganizeStatistic();
      // Read
      // Write
      static Json::Value writeOverallLog(const research::task_holder_statistic::mcmanager_overall_log& history);
      static Json::Value writePipelineRunLog(const research::task_holder_statistic::mcmanager_pipeline_run_log& history);
      static Json::Value writeRunLog(const research::task_holder_statistic::mcmanager_run_log& data);
      static Json::Value writePacketLog(const research::task_holder_statistic::mcmanager_packet_log& data);
      static Json::Value writeTaskLog(const research::task_holder_statistic::mcmanager_task_log& data, size_t packetIdx);
      void saveStatisticDataToFile();
      void pythonizeResult();
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      research::task_holder_statistic::mcmanager_overall_log m_statistic;
      organized_statistic m_organizedStatistic;
      static size_t m_maxSize;
   };
}