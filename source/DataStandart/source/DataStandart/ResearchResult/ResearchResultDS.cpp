#include "stdafx.h"
#include "ResearchResultDS.h"

using namespace SV;
using namespace SV::data_standart;

size_t ResearchResultDataStandart::m_maxSize = 0;

const char* PYTHONIZE_ENTRANCE_POINT_PATH = "python.exe PyDirectDrawer/main.py";

void ResearchResultDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   // TODO: check!!!
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

void ResearchResultDataStandart::SetData(const research::task_holder_statistic::mcmanager_overall_log& stat, bool pythonize_result)
{
   m_statistic = stat;
   std::filesystem::path path(getPath());
   std::filesystem::create_directories(path);
   saveStatisticDataToFile();
   if (pythonize_result)
      pythonizeResult();
}

void ResearchResultDataStandart::saveStatisticDataToFile()
{
   std::string dataFilePath = getDataFilePath();
   std::ofstream file(dataFilePath);
   //reorganizeStatistic();
   Json::Value data;
   data[tag::overall_log] = writeOverallLog(m_statistic);
   file << data;
}

void ResearchResultDataStandart::pythonizeResult()
{
   std::string path(PYTHONIZE_ENTRANCE_POINT_PATH);
   path.append(" ");
   path.append(getDataFilePath());
   std::system(path.c_str());
}

Json::Value ResearchResultDataStandart::writeOverallLog(const research::task_holder_statistic::mcmanager_overall_log& log)
{
   Json::Value ehistory;
   Json::Value jhistory(Json::arrayValue);
   for (const auto& stat : log.pipeline_run_list)
   {
      Json::Value runh;
      runh[tag::pipeline_run_log] = writePipelineRunLog(stat);
      jhistory.append(runh);
   }
   ehistory[tag::history] = jhistory;
   return ehistory;
}

Json::Value ResearchResultDataStandart::writePipelineRunLog(const research::task_holder_statistic::mcmanager_pipeline_run_log& hcrhistory)
{
   organized_statistic::holder_data::task_data fictive{ 0, 0 };
   Json::Value jrundata;
   Json::Value jstat(Json::arrayValue);
   size_t idx = 0;
   for (const auto& data : hcrhistory.step_list)
   {
      idx++;
      Json::Value rund;
      rund[tag::run_log] = writeRunLog(data);
      jstat.append(rund);
   }
   /*for (size_t fillerIdx = idx; fillerIdx < m_maxSize; fillerIdx++)
   {
      jstat.append(writeClusterRunData(fictive));
   }*/
   //jrundata[tag::unit_count] = hcrhistory.unit_count;
   jrundata[tag::history] = jstat;
   jrundata[tag::thread_count] = hcrhistory.thread_count;
   jrundata[tag::packet_size] = hcrhistory.packet_size;
   return jrundata;
}

Json::Value ResearchResultDataStandart::writeRunLog(const research::task_holder_statistic::mcmanager_run_log& hcrdata)
{
   Json::Value jrundata;
   //Json::Value jdata_inner;
   Json::Value jrun(Json::arrayValue);
   //jdata_inner[tag::count] = data.data.size();//std::to_string(data.size());
   size_t maxSize = 0;
   size_t maxUnitIdx = 0;
   /*for (const auto& time : data.data)
   {
      for (const auto& stamp : time.second.data)
         maxUnitIdx = stamp.unit_idx > maxUnitIdx ? stamp.unit_idx : maxUnitIdx;
   }
   for (const auto& time : data.data)
   {
      maxSize = maxSize > time.second.data.size() ? maxSize : time.second.data.size();
   }
   size_t holderIdx = 0;*/
   for (const auto& time : hcrdata.packet_list)
   {
      //size_t holderIdx = time.first;
      Json::Value rund;
      rund[tag::packet_log] = writePacketLog(time);
      jrun.append(rund);
      //jdata_inner[std::to_string(holderIdx++)] = rund;
   }
   jrundata[tag::data] = jrun;
   jrundata[tag::unit_count] = hcrdata.unit_count;
   //jdata[tag::unit_count] = maxUnitIdx + 1;
   return jrundata;
}

Json::Value ResearchResultDataStandart::writePacketLog(const research::task_holder_statistic::mcmanager_packet_log& data)
{
   //auto fictive = research::task_holder_statistic::statistic_unit{ 0, 0 };
   Json::Value jrundata;
   Json::Value jrun(Json::arrayValue);
   for (const auto& times : data.task_list)
   {
      Json::Value unitdata;
      unitdata[tag::task_log] = writeTaskLog(times, data.packet_idx);
      jrun.append(unitdata);
   }
   // NOTE: Только для матлаба
   /*for (size_t idx = data.size(); idx < maxSize; idx++)
   {
      jdata.append(writeUnitData(fictive));
   }*/
   jrundata[tag::data] = jrun;
   jrundata[tag::start_ts] = data.start_ts;
   jrundata[tag::finish_ts] = data.finish_ts;
   return jrundata;
}

Json::Value ResearchResultDataStandart::writeTaskLog(const research::task_holder_statistic::mcmanager_task_log& data, size_t packetIdx)
{
   Json::Value jstamp;
   jstamp[tag::start_ts] = data.start_ts;
   jstamp[tag::finish_ts] = data.finish_ts;
   jstamp[tag::holder_idx] = data.holder_idx;
   jstamp[tag::task_idx] = data.task_idx;
   jstamp[tag::unit_idx] = data.unit_idx;
   jstamp[tag::shard_idx] = data.shard_idx;
   jstamp[tag::packet_idx] = packetIdx;
   return jstamp;
}

iDataStandart* CreateResearchResultDataStandart(SV::central_pack* pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService* pService)
{
   return new ResearchResultDataStandart(pack, base_folder, pService);
}