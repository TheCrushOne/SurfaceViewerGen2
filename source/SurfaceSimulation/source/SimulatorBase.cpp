#include "stdafx.h"
#include "SimulatorBase.h"

#include <filesystem>

using namespace ColregSimulation;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
   }

struct tag
{
   // TODO: запаковать в одно место(дублируется с ConfigDispatcherImpl.h)
   static constexpr char root[] = "root";
   static constexpr char standarts[] = "data_standarts";
   static constexpr char standart[] = "data_standart";
   static constexpr char type[] = "type";
   static constexpr char params[] = "params";
};

SimulatorBase::SimulatorBase(central_pack * pack, iPropertyInterface * prop, navigation_dispatcher::iComServicePtr service)
   : Central(pack)
   , m_prop(prop)
   , m_service(service)
{
   m_orderCacheFolder = std::filesystem::absolute(std::filesystem::current_path().generic_wstring() + L"\\..\\..\\..\\cache\\order_heap\\");

   VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController, pack);
   VALID_CHECK_DLL_LOAD("Engine", "CreateEngine", m_engine, pack);
   VALID_CHECK_DLL_LOAD("UniversalLogger", "CreateUniversalLogger", m_logger, pack);

   VALID_CHECK_DLL_LOAD("DataStandart", "CreateSurfaceViewerGenMapDataStandart", m_mapDS, pack, "", m_service);
   VALID_CHECK_DLL_LOAD("DataStandart", "CreateChartObjectDataStandart", m_mapObjDS, pack, "", m_service);
   VALID_CHECK_DLL_LOAD("DataStandart", "CreatePathStorageDataStandart", m_pathDS, pack, "", m_service);
   VALID_CHECK_DLL_LOAD("DataStandart", "CreateOptimizedPathStorageDataStandart", m_optPathDS, pack, "", m_service);
}

bool SimulatorBase::CheckOpenScenario()
{
   // TODO: check!!! а то я чет не уверен, что тут всё верно
   SetSimulatorScenarioState(ColregSimulation::SCENARIO_STATUS::SS_MAP_CHECKOPENED);
   SetSimulatorSimulationState(ColregSimulation::SIMULATION_STATUS::SS_STOP);
   return true;
}

template<typename Standart>
void deserializeStandartAttrs(Standart* standart, const char* configPath, data_standart::DataStandartType expected)
{
   xml_properties::PropertyContainer properties("NavigationDispatcherOrderConfig", "1.0");

   if (!properties.load(configPath))
      return;
   const auto& sClass = properties.GetRoot();
   const auto root = sClass.GetChild(tag::root);

   auto checkDataStandart = [](const xml_properties::PropertyItem& properties, Standart* ds, data_standart::DataStandartType expected)
   {
      std::string type;
      std::wstring wtype;
      properties[tag::type].Get(type);
      wtype = SVGUtils::stringToWstring(type);
      if (data_standart::convert_datastandart_name(wtype.c_str()) == expected)
         ds->DeserializeAttrs(properties[tag::params]);
   };

   const auto standarts = root->GetChild(tag::standarts);
   const auto& standartList = standarts->GetChildren().equal_range(tag::standart);
   for (auto iter = standartList.first; iter != standartList.second; ++iter)
     checkDataStandart(iter->second, standart, expected);
}

bool SimulatorBase::LoadProcessedMap()
{
   auto* mapDS = reinterpret_cast<data_standart::iSurfaceVieverGenMapDataStandart*>(m_mapDS.operator->());
   m_currentConfig = m_orderCacheFolder + L"process_map.xml";
   deserializeStandartAttrs(mapDS, SVGUtils::wstringToString(m_currentConfig).c_str(), data_standart::DataStandartType::DST_SVGM);
   SetSimulatorScenarioState(ColregSimulation::SCENARIO_STATUS::SS_MAP_PROCESSED);
   SetSimulatorSimulationState(ColregSimulation::SIMULATION_STATUS::SS_STOP);
   return true;
}

bool SimulatorBase::LoadProcessedMapObjects()
{
   auto* mapObjDS = reinterpret_cast<data_standart::iChartObjectDataStandart*>(m_mapObjDS.operator->());
   m_currentConfig = m_orderCacheFolder + L"process_map_object.xml";
   deserializeStandartAttrs(mapObjDS, SVGUtils::wstringToString(m_currentConfig).c_str(), data_standart::DataStandartType::DST_OBJ);
   auto& junc = mapObjDS->GetData();
   // TODO: разделить, если потребуется
   for (auto& dyn : junc.dynamic_objects)
      m_chartObjects.emplace_back(dyn);
   for (auto& stat : junc.static_objects)
      m_chartObjects.emplace_back(stat);
   SetSimulatorScenarioState(ColregSimulation::SCENARIO_STATUS::SS_MAPOBJ_PROCESSED);
   SetSimulatorSimulationState(ColregSimulation::SIMULATION_STATUS::SS_STOP);
   return true;
}

bool SimulatorBase::LoadProcessedPaths()
{
   auto* pathDS = reinterpret_cast<data_standart::iPathStorageDataStandart*>(m_pathDS.operator->());
   m_currentConfig = m_orderCacheFolder + L"process_path_find.xml";
   deserializeStandartAttrs(pathDS, SVGUtils::wstringToString(m_currentConfig).c_str(), data_standart::DataStandartType::DST_PATHS);
   auto& rt = pathDS->GetData();
   auto routeMover = [](const std::vector<settings::route>& src, std::vector<settings::unit_data_element>& dst)
   {
      for (auto& pt : src)
         dst.emplace_back(settings::unit_data_element("", pt.start, pt.finish, pt.route_list, pt.control_point_list));
   };
   routeMover(rt.air_routes, m_data.unit_data.air_units);
   routeMover(rt.land_routes, m_data.unit_data.land_units);
   SetSimulatorScenarioState(ColregSimulation::SCENARIO_STATUS::SS_PATHS_COUNTED);
   SetSimulatorSimulationState(ColregSimulation::SIMULATION_STATUS::SS_STOP);
   return true;
}

bool SimulatorBase::LoadProcessedOptPaths()
{
   auto* optPathDS = reinterpret_cast<data_standart::iOptimizedPathStorageDataStandart*>(m_optPathDS.operator->());
   SetSimulatorScenarioState(ColregSimulation::SCENARIO_STATUS::SS_OPT_PATHS_COUNTED);
   SetSimulatorSimulationState(ColregSimulation::SIMULATION_STATUS::SS_STOP);
   return true;
}