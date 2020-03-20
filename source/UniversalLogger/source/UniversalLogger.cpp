#include "UniversalLogger.h"

using namespace logger;

UniversalLogger::UniversalLogger()
   : Communicable(nullptr)
{}

void UniversalLogger::Init(ICommunicator* comm)
{
   m_communicator = comm;
}

bool UniversalLogger::LogThreadResearchResult(const wchar_t* logPath, const ThreadResearchComplexStorage& meta)
{
   std::vector
}

logger::iUniversalLogger* CreateUniversalLogger()
{
   return new logger::UniversalLogger();
}