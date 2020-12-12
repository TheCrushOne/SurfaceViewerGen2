#include "stdafx.h"
#include "ResearchResultDS.h"

using namespace SV;
using namespace SV::data_standart;

void ResearchResultDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   // TODO: check!!!
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

void ResearchResultDataStandart::SetData(const research::statistic_data_history& stat)
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
   reorganizeStatistic();
   Json::Value data;
   data[tag::history] = writeStatisticHistory(m_organizedStatistic);
   file << data;
}

void ResearchResultDataStandart::reorganizeStatistic()
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
}

Json::Value ResearchResultDataStandart::writeStatisticHistory(const organized_statistic& history)
{
   Json::Value jhistory(Json::arrayValue);
   for (const auto& stat : history.stat)
      jhistory.append(writeStatisticLine(stat.second));
   return jhistory;
}

Json::Value ResearchResultDataStandart::writeStatisticLine(const organized_statistic::holder_data& statistic)
{
   Json::Value jstat(Json::arrayValue);
   for (const auto& stamp : statistic.data)
      jstat.append(writeStatisticStamp(stamp.second));
   return jstat;
}

Json::Value ResearchResultDataStandart::writeStatisticStamp(const organized_statistic::holder_data::task_data& stamp)
{
   Json::Value jstamp;
   jstamp[tag::start_ts] = stamp.start_ts;
   jstamp[tag::finish_ts] = stamp.finish_ts;
   //jstamp[tag::holder_idx] = stamp.holder_idx;
   //jstamp[tag::task_idx] = stamp.task_idx;
   return jstamp;
}

iDataStandart* CreateResearchResultDataStandart(SV::central_pack* pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService* pService)
{
   return new ResearchResultDataStandart(pack, base_folder, pService);
}