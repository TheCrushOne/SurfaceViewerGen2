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

SimulatorBase::SimulatorBase(central_pack * pack, iPropertyInterface * prop, navigation_dispatcher::iComServicePtr service)
   : Central(pack)
   , m_prop(prop)
   , m_service(service)
{
   VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController, pack);
   VALID_CHECK_DLL_LOAD("Engine", "CreateEngine", m_engine, pack);
   VALID_CHECK_DLL_LOAD("UniversalLogger", "CreateUniversalLogger", m_logger, pack);

   VALID_CHECK_DLL_LOAD("DataStandart", "CreateSurfaceViewerGenMapDataStandart", m_mapDS, pack, "", m_service);
   VALID_CHECK_DLL_LOAD("DataStandart", "CreatePathStorageDataStandart", m_pathDS, pack, "", m_service);
   VALID_CHECK_DLL_LOAD("DataStandart", "CreateOptimizedPathStorageDataStandart", m_optPathDS, pack, "", m_service);
}

bool SimulatorBase::CheckOpenScenario()
{
   return true;
}

bool SimulatorBase::LoadProcessedMap()
{
   auto* mapDS = reinterpret_cast<data_standart::iSurfaceVieverGenMapDataStandart*>(m_mapDS.operator->());
   const pathfinder::GeoMatrix& matrix = mapDS->GetData();
   return true;
}

bool SimulatorBase::LoadProcessedPaths()
{
   auto* pathDS = reinterpret_cast<data_standart::iPathStorageDataStandart*>(m_mapDS.operator->());
   auto& rt = pathDS->GetData();
   auto routeMover = [](const std::vector<settings::route>& src, std::vector<settings::unit_data_element>& dst)
   {
      for (auto& pt : src)
         dst.emplace_back(settings::unit_data_element("", pt.start, pt.finish, pt.route_list, pt.control_point_list));
   };
   routeMover(rt.air_routes, m_data.unit_data.air_units);
   routeMover(rt.land_routes, m_data.unit_data.land_units);
   return true;
}

bool SimulatorBase::LoadProcessedOptPaths()
{
   auto* optPathDS = reinterpret_cast<data_standart::iOptimizedPathStorageDataStandart*>(m_mapDS.operator->());
   return true;
}