#include "stdafx.h"
#include "ResearchResultDS.h"

using namespace SV;
using namespace SV::data_standart;

size_t ResearchResultDataStandart::m_maxSize = 0;

void ResearchResultDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   // TODO: check!!!
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

void ResearchResultDataStandart::SetData(const research::task_holder_statistic::experiment_history& stat)
{
   m_statistic = stat;
   std::filesystem::path path(getPath());
   std::filesystem::create_directories(path);
   saveStatisticDataToFile();
}

void ResearchResultDataStandart::saveStatisticDataToFile()
{
   std::string dataFilePath = getDataFilePath();
   std::ofstream file(dataFilePath);
   //reorganizeStatistic();
   Json::Value data;
   data[tag::experiment_history] = writeExperimentHistory(m_statistic);
   file << data;
}

/*void ResearchResultDataStandart::reorganizeStatistic()
{
   for (const auto& stat : m_statistic)
   {
      for (const auto& stamp : stat)
      {
         if (m_organizedStatistic.stat.find(stamp.holder_idx) == m_organizedStatistic.stat.end())
            m_organizedStatistic.stat.emplace(stamp.holder_idx, organized_statistic::holder_data());
         auto& data = m_organizedStatistic.stat.at(stamp.holder_idx).data;
         if (data.find(stamp.task_idx) == data.end())
            data.emplace(stamp.task_idx, organized_statistic::holder_data::task_data());
         data.at(stamp.task_idx).start_ts = stamp.start_ts;
         data.at(stamp.task_idx).finish_ts = stamp.finish_ts;
      }
   }

   m_maxSize = 0;
   for (const auto& stat : m_organizedStatistic.stat)
      m_maxSize = stat.second.data.size() > m_maxSize ? stat.second.data.size() : m_maxSize;
}*/

Json::Value ResearchResultDataStandart::writeExperimentHistory(const research::task_holder_statistic::experiment_history& history)
{
   Json::Value ehistory;
   Json::Value jhistory(Json::arrayValue);
   for (const auto& stat : history)
      jhistory.append(writeClusterRunHistory(stat));
   ehistory[tag::cluster_history] = jhistory;
   return ehistory;
}

Json::Value ResearchResultDataStandart::writeClusterRunHistory(const research::task_holder_statistic::holder_cluster_run_history& history)
{
   organized_statistic::holder_data::task_data fictive{ 0, 0 };
   Json::Value jrundata;
   Json::Value jstat(Json::arrayValue);
   size_t idx = 0;
   for (const auto& data : history)
   {
      idx++;
      jstat.append(writeClusterRunData(data));
   }
   /*for (size_t fillerIdx = idx; fillerIdx < m_maxSize; fillerIdx++)
   {
      jstat.append(writeClusterRunData(fictive));
   }*/
   jrundata[tag::cluster_run_data] = jstat;
   return jrundata;
}

Json::Value ResearchResultDataStandart::writeClusterRunData(const research::task_holder_statistic::holder_cluster_run_data& data)
{
   Json::Value jdata;
   Json::Value jdata_inner;
   jdata_inner[tag::count] = data.size();//std::to_string(data.size());
   size_t maxSize = 0;
   for (const auto& time : data)
   {
      maxSize = maxSize > time.second.size() ? maxSize : time.second.size();
   }
   size_t holderIdx = 0;
   for (const auto& time : data)
   {
      //size_t holderIdx = time.first;
      //size_t holderIdx = time.first;
      jdata_inner[std::to_string(holderIdx++)] = writeHolderRunData(time.second, maxSize);      
   }
   jdata[tag::holder_run_data] = jdata_inner;
   return jdata;
}

Json::Value ResearchResultDataStandart::writeHolderRunData(const research::task_holder_statistic::holder_run_data& data, size_t maxSize)
{
   auto fictive = research::task_holder_statistic::statistic_unit{ 0, 0 };
   Json::Value jdata(Json::arrayValue);
   for (const auto& times : data)
   {
      jdata.append(writeUnitData(times));
   }
   for (size_t idx = data.size(); idx < maxSize; idx++)
   {
      jdata.append(writeUnitData(fictive));
   }
   return jdata;
}

Json::Value ResearchResultDataStandart::writeUnitData(const research::task_holder_statistic::statistic_unit& data)
{
   Json::Value jstamp;
   jstamp[tag::start_ts] = data.start_ts;
   jstamp[tag::finish_ts] = data.finish_ts;
   //jstamp[tag::holder_idx] = stamp.holder_idx;
   jstamp[tag::task_idx] = data.task_idx;
   jstamp[tag::unit_idx] = data.unit_idx;
   jstamp[tag::shard_idx] = data.shard_idx;
   return jstamp;
}

iDataStandart* CreateResearchResultDataStandart(SV::central_pack* pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService* pService)
{
   return new ResearchResultDataStandart(pack, base_folder, pService);
}