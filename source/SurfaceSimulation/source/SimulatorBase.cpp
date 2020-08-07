#include "stdafx.h"
#include "SimulatorBase.h"

using namespace ColregSimulation;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
   }

SimulatorBase::SimulatorBase(central_pack * pack, iPropertyInterface * prop)
   : Central(pack)
   , m_prop(prop)
{
   VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController, pack);
   VALID_CHECK_DLL_LOAD("Engine", "CreateEngine", m_engine, pack);
   VALID_CHECK_DLL_LOAD("NavigationDispatcher", "CreateNavigationDispatcher", m_navigationDispatcher, pack);
   VALID_CHECK_DLL_LOAD("UniversalLogger", "CreateUniversalLogger", m_logger, pack);
}