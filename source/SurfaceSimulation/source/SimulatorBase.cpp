#include "stdafx.h"
#include "SimulatorBase.h"

using namespace ColregSimulation;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return false; \
   } \
   guard->Init(GetPack());


bool SimulatorBase::Init(central_pack* pack)
{
   Central::Init(pack);

   VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController);
   VALID_CHECK_DLL_LOAD("Engine", "CreateEngine", m_engine);
   VALID_CHECK_DLL_LOAD("ChartObjectGenerator", "CreateGenerator", m_generator);
   VALID_CHECK_DLL_LOAD("UniversalLogger", "CreateUniversalLogger", m_logger);

   return true;
}

