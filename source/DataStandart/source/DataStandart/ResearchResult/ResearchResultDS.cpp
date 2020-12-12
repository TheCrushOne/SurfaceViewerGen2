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
   Json::Value data;
   data[tag::history] = writeStatisticHistory(m_statistic);
   file << data;
}

Json::Value ResearchResultDataStandart::writeStatisticHistory(const research::statistic_data_history& history)
{
   Json::Value jhistory(Json::arrayValue);
   for (const auto& stat : history)
      jhistory.append(writeStatisticLine(stat));
   return jhistory;
}

Json::Value ResearchResultDataStandart::writeStatisticLine(const research::statistic_data& statistic)
{
   Json::Value jstat(Json::arrayValue);
   for (const auto& stamp : statistic)
      jstat.append(writeStatisticStamp(stamp));
   return jstat;
}

Json::Value ResearchResultDataStandart::writeStatisticStamp(const research::task_holder_statistic::statistic_unit& stamp)
{
   Json::Value jstamp;
   jstamp[tag::start_ts] = stamp.start_ts;
   jstamp[tag::finish_ts] = stamp.finish_ts;
   jstamp[tag::holder_idx] = stamp.holder_idx;
   jstamp[tag::task_idx] = stamp.task_idx;
   return jstamp;
}

iDataStandart* CreateResearchResultDataStandart(SV::central_pack* pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService* pService)
{
   return new ResearchResultDataStandart(pack, base_folder, pService);
}