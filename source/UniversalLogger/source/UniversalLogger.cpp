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
   : Central(nullptr)
{}

void UniversalLogger::Init(ICommunicator* comm)
{
   m_communicator = comm;
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

   auto path = std::wstring(logPath) + L"//" + str;
   //fs::create_directories(std::wstring(logPath) + L"//" + str);

   //std::vector
   return true;
}

logger::iUniversalLogger* CreateUniversalLogger()
{
   return new logger::UniversalLogger();
}