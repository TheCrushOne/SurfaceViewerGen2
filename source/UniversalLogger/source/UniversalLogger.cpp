#include "UniversalLogger.h"

#include <iostream>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

#include "json/json_wrapper_impl.h"

namespace fs = std::filesystem;
using namespace SV;
using namespace SV::logger;

UniversalLogger::UniversalLogger(central_pack* pack)
   : Central(pack)
{}

bool UniversalLogger::LogThreadResearchResult(const research::ThreadResearchComplexStorage& meta)
{
   time_t rawtime;
   struct tm* timeinfo;
   wchar_t buffer[80];

   time(&rawtime);
   timeinfo = localtime(&rawtime);

   //strftime(buffer, sizeof(buffer), "%d%m%Y_%H%M%S_TRR", timeinfo);
   wcsftime(buffer, sizeof(buffer), L"%d%m%Y_%H%M%S_TRR", timeinfo);
   std::wstring str(buffer);
   logThreadResearchMeta(meta, str);
   logThreadResearchResult(meta, str);
   
   //fs::create_directories(std::wstring(logPath) + L"//" + str);
   //std::vector
   return true;
}

void UniversalLogger::logThreadResearchResult(const research::ThreadResearchComplexStorage& meta, const std::wstring str)
{
   return;
   auto filePath = std::wstring(/*GetPack()->paths->logger_folder_path*/) + L"//" + str + L".dat";
   std::ofstream fil;
   fil.open(filePath, std::ios::out | std::ios::binary);
   for (auto metaElement : meta.data)
   {
      // NOTE: тут только карта индексов, парсинг в значения есть в матлабе
      fil << metaElement.index.thread_pool_idx << " "
          << metaElement.index.task_pool_idx << " "
          << metaElement.index.fly_count_idx << " "
          << metaElement.index.length_idx << " "
          << metaElement.result.time.diff;
      fil << "\n";
   }
   fil.close();
}

void UniversalLogger::logThreadResearchMeta(const research::ThreadResearchComplexStorage& meta, const std::wstring str)
{
   return;
   auto filePath = std::wstring(/*GetPack()->paths->logger_folder_path*/) + L"//" + str + L".meta";
   std::ofstream o(filePath);
   Json::Value j;
   //j[tag::fly_count_values] = meta.info.fly_count_range.values;
   //j[tag::task_pool_values] = meta.info.task_pool_range.values;
   //j[tag::thread_pool_values] = meta.info.thread_pool_range.values;
   //j[tag::length_values] = meta.info.length_range.values;
   o << j;
}

logger::iUniversalLogger* CreateUniversalLogger(central_pack* pack)
{
   return new logger::UniversalLogger(pack);
}