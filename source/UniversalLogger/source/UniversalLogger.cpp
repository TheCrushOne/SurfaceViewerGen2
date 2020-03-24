#include "UniversalLogger.h"

#include <iostream>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;
using namespace logger;

UniversalLogger::UniversalLogger()
{}

void UniversalLogger::Init(std::shared_ptr<central_pack> pack)
{
   Central::Init(pack);
}

bool UniversalLogger::LogThreadResearchResult(const ThreadResearchComplexStorage& meta)
{
   time_t rawtime;
   struct tm* timeinfo;
   wchar_t buffer[80];

   time(&rawtime);
   timeinfo = localtime(&rawtime);

   //strftime(buffer, sizeof(buffer), "%d%m%Y_%H%M%S_TRR", timeinfo);
   wcsftime(buffer, sizeof(buffer), L"%d%m%Y_%H%M%S_TRR", timeinfo);
   std::wstring str(buffer);

   auto path = std::wstring(GetPack()->paths->logger_folder_path) + L"//" + str;
   //fs::create_directories(std::wstring(logPath) + L"//" + str);

   //std::vector
   return true;
}

logger::iUniversalLogger* CreateUniversalLogger()
{
   return new logger::UniversalLogger();
}