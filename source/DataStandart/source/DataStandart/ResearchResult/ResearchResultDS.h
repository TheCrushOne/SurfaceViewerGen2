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
         static constexpr char experiment_history[] = "experiment_history";

         static constexpr char cluster_history[] = "cluster_history";

         static constexpr char cluster_run_data[] = "cluster_run_data";

         static constexpr char holder_run_data[] = "holder_run_data";

         static constexpr char count[] = "count";

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
      void SetData(const research::task_holder_statistic::experiment_history&) override final;
   private:
      // Common
      std::string getDataFilePath() { return std::string(getPath()) + "\\universal.rres"; }
      void reorganizeStatistic();
      // Read
      // Write
      static Json::Value writeExperimentHistory(const research::task_holder_statistic::experiment_history& history);
      static Json::Value writeClusterRunHistory(const research::task_holder_statistic::holder_cluster_run_history& history);
      static Json::Value writeClusterRunData(const research::task_holder_statistic::holder_cluster_run_data& data);
      static Json::Value writeHolderRunData(const research::task_holder_statistic::holder_run_data& data, size_t maxSize);
      static Json::Value writeUnitData(const research::task_holder_statistic::statistic_unit& data);
      void saveStatisticDataToFile();
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
      research::task_holder_statistic::experiment_history m_statistic;
      organized_statistic m_organizedStatistic;
      static size_t m_maxSize;
   };
}