#include "stdafx.h"
#include "SimulatorBase.h"
#include "math/math_utils.h"
#include "SVCG/positioning.h"

#include <filesystem>

using namespace SV;
using namespace SV::surface_simulation;

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

SimulatorBase::SimulatorBase(central_pack* pack, iPropertyInterface* prop, navigation_dispatcher::iComService* service)
   : Central(pack)
   , Servicable(service)
   , m_prop(prop)
   , m_gridMeta({})
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
   SetSimulatorScenarioState(SCENARIO_STATUS::SS_MAP_CHECKOPENED);
   SetSimulatorSimulationState(SIMULATION_STATUS::SS_STOP);
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
      properties[tag::type].Get(type);
      if (data_standart::convert_datastandart_name(type.c_str()) == expected)
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
   SetSimulatorScenarioState(SCENARIO_STATUS::SS_MAP_PROCESSED);
   SetSimulatorSimulationState(SIMULATION_STATUS::SS_STOP);
   return true;
}

bool SimulatorBase::LoadProcessedMapObjects()
{
   auto* mapObjDS = reinterpret_cast<data_standart::iChartObjectDataStandart*>(m_mapObjDS.operator->());
   m_currentConfig = m_orderCacheFolder + L"process_map_object.xml";
   deserializeStandartAttrs(mapObjDS, SVGUtils::wstringToString(m_currentConfig).c_str(), data_standart::DataStandartType::DST_OBJ);
   auto& junc = mapObjDS->GetData();
   // TODO: разделить, если потребуется
   size_t id = 0;
   auto& chartObjs = m_data.chart_objects;
   chartObjs.clear();
   for (auto& dyn : junc.dynamic_objects)
   {
      chartObjs.emplace_back(LayerChartObjectImpl{});
      chartObjs.back().SetSrcRoute(layer_provider::layer_chart_object{ dyn, id++ });
   }
   for (auto& stat : junc.static_objects)
   {
      chartObjs.emplace_back(LayerChartObjectImpl{});
      chartObjs.back().SetSrcRoute(layer_provider::layer_chart_object{ stat, id++ });
   }
   SetSimulatorScenarioState(SCENARIO_STATUS::SS_MAPOBJ_PROCESSED);
   SetSimulatorSimulationState(SIMULATION_STATUS::SS_STOP);
   return true;
}

template<typename UnitType>
void routeMover(const settings::application_settings& settings, const std::vector<settings::route>& src, std::vector<UnitType>& dst)
{
   for (const auto& pt : src)
   {
      UnitType unit(settings);
      unit.SetSrcRoute(pt);
      dst.emplace_back(unit);
   }
}

bool SimulatorBase::LoadProcessedPaths()
{
   auto* pathDS = reinterpret_cast<data_standart::iPathStorageDataStandart*>(m_pathDS.operator->());
   m_currentConfig = m_orderCacheFolder + L"process_path_find.xml";
   deserializeStandartAttrs(pathDS, SVGUtils::wstringToString(m_currentConfig).c_str(), data_standart::DataStandartType::DST_PATHS);
   auto& rt = pathDS->GetData();
   routeMover<LayerDroneImpl>(m_settings, rt.air_routes, m_data.air_unit_objects);
   routeMover<LayerRoverImpl>(m_settings, rt.land_routes, m_data.land_unit_objects);
   calcStepCount();
   SetSimulatorScenarioState(SCENARIO_STATUS::SS_PATHS_COUNTED);
   SetSimulatorSimulationState(SIMULATION_STATUS::SS_STOP);
   return true;
}

bool SimulatorBase::LoadProcessedOptPaths()
{
   auto* optPathDS = reinterpret_cast<data_standart::iOptimizedPathStorageDataStandart*>(m_optPathDS.operator->());
   SetSimulatorScenarioState(surface_simulation::SCENARIO_STATUS::SS_OPT_PATHS_COUNTED);
   SetSimulatorSimulationState(surface_simulation::SIMULATION_STATUS::SS_STOP);
   return true;
}

void SimulatorBase::calcStepCount()
{
   size_t maxStepCount = 0;
   auto& au = m_data.air_unit_objects;
   for (const auto& elem : au)
      maxStepCount = maxStepCount < elem.GetPosCount() ? elem.GetPosCount() : maxStepCount;
   auto& lu = m_data.land_unit_objects;
   for (const auto& elem : lu)
      maxStepCount = maxStepCount < elem.GetPosCount() ? elem.GetPosCount() : maxStepCount;
   m_data.step_count = maxStepCount;
   m_data.current_step = 0;
}

size_t SimulatorBase::GetUnitCount(UNIT_TYPE type) const
{
   switch (type)
   {
   case UNIT_TYPE::UT_DRONE:
      return m_data.air_unit_objects.size();
   case UNIT_TYPE::UT_ROVER:
      return m_data.land_unit_objects.size();
   }
   return 0;
}

const iLayerUnit* SimulatorBase::GetUnitById(id_type id) const
{
   bool find = false;
   auto idChecker = [this, id, &find](surface_simulation::UNIT_TYPE type) -> const surface_simulation::iLayerUnit*
   {
      for (size_t idx = 0; idx < GetUnitCount(type); idx++)
      {
         if (GetUnitByIdx(type, idx)->GetInfo().id == id)
         {
            find = true;
            return GetUnitByIdx(type, idx);
         }
      }
      return nullptr;
   };
   const iLayerUnit* res = nullptr;
   res = idChecker(surface_simulation::UNIT_TYPE::UT_DRONE) ? idChecker(surface_simulation::UNIT_TYPE::UT_DRONE)
      : (idChecker(surface_simulation::UNIT_TYPE::UT_ROVER) ? idChecker(surface_simulation::UNIT_TYPE::UT_ROVER)
      : idChecker(surface_simulation::UNIT_TYPE::UT_SHIP));

   return res;
}

const iLayerUnit* SimulatorBase::GetUnitByIdx(UNIT_TYPE type, size_t idx) const
{
   return const_cast<SimulatorBase*>(this)->getUnitByIdx(type, idx);
}

iLayerUnit* SimulatorBase::getUnitByIdx(UNIT_TYPE type, size_t idx)
{
   switch (type)
   {
   case UNIT_TYPE::UT_ROVER:
      return &m_data.land_unit_objects.at(idx);
   case UNIT_TYPE::UT_DRONE:
      return &m_data.air_unit_objects.at(idx);
   case UNIT_TYPE::UT_SHIP:
   default:
      break;
   }
   ATLASSERT(false);
   return nullptr;
}

const iLayerChartObject* SimulatorBase::GetChartObjectByIdx(size_t idx) const
{
   /*const_cast<const math::geo_contour_vct_ref>(m_chartObjects.geom_contour_vct)*/
   return &m_data.chart_objects.at(idx);
}

const iLayerChartObject* SimulatorBase::GetChartObjectById(chart_object_id id) const
{
   for (size_t idx = 0; idx < GetChartObjectCount(); idx++)
   {
      const auto* obj = GetChartObjectByIdx(idx);
      if (obj->GetContourData()->id == id)
         return obj;
   }
   return nullptr;
}