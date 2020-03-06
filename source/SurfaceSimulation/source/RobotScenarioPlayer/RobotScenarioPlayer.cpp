#include "stdafx.h"
#include "RobotScenarioPlayer.h"
#include "colreg\ColregContainers.h"
#include "SVCG/positioning.h"

using namespace ColregSimulation;

RobotScenarioPlayer::RobotScenarioPlayer(ICommunicator* pCommunicator, iPropertyInterface* prop)
   : SimulatorBase(pCommunicator, prop)
   , m_routedata(std::make_shared<pathfinder::route_data>())
{
   m_engine.Create(SVGUtils::CurrentDllPath("Engine").c_str(), "CreateEngine");
   if (!m_engine.IsValid())
   {
      m_communicator->Message(ICommunicator::MS_Error, "Can't load 'Engine'");
      return;
   }
}

void RobotScenarioPlayer::prepareRootData()
{
   auto PointSettingElementToRoute = [](const settings::point_setting_element& elem) -> pathfinder::route
   {
      pathfinder::route route;
      route.control_point_list = elem.control_point_list;
      route.start = elem.start;
      route.finish = elem.finish;
      return route;
   };
   m_routedata->air_routes.resize(m_settings.unit_stt.air_units.size());
   for (size_t idx = 0; idx < m_settings.unit_stt.air_units.size(); idx++)
   {
      m_routedata->air_routes[idx] = PointSettingElementToRoute(m_settings.unit_stt.air_units.at(idx));
   }

   m_routedata->land_routes.resize(m_settings.unit_stt.land_units.size());
   for (size_t idx = 0; idx < m_settings.unit_stt.land_units.size(); idx++)
   {
      m_routedata->land_routes[idx] = PointSettingElementToRoute(m_settings.unit_stt.land_units.at(idx));
   }
}

void RobotScenarioPlayer::Start()
{
   m_databaseController->LoadScenarioData(m_settings, m_coordGrid);
   prepareRootData();
   addUnitsFromScenario();
   m_engine->ProcessPathFind(m_routedata, m_coordGrid);
   updateUnitsPath();
   int i = 0;
}

void RobotScenarioPlayer::Stop()
{}

void RobotScenarioPlayer::Reset()
{
   Stop();
   Start();
}

bool RobotScenarioPlayer::IsRunning() const
{
   return false;
}

size_t RobotScenarioPlayer::GetControlPointsCount() const
{
   return 0;
}

const control_point_info& RobotScenarioPlayer::GetControlPointInfo(size_t controlPntIdx) const
{
   return control_point_info{ 0 };
}

bool RobotScenarioPlayer::PlayFrom(size_t controlPointIdx)
{
   return false;
}

bool RobotScenarioPlayer::NextControlPoint()
{
   return false;
}

size_t RobotScenarioPlayer::GetCurrentControlPointIdx() const
{
   return 0;
}

bool RobotScenarioPlayer::SaveLogPair(const char* filename) const
{
   return false;
}

const ColregSimulation::SIMULATION_PLAYER_TYPE RobotScenarioPlayer::GetSimulationType()
{
   return ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_LOG;
}

void RobotScenarioPlayer::SetAppSettings(const settings::application_settings& s)
{}

void RobotScenarioPlayer::ReloadSettings()
{}

const settings::application_settings& RobotScenarioPlayer::GetAppSettings() const
{
   return m_settings;
}

size_t RobotScenarioPlayer::GetUnitCount(UNIT_TYPE type) const
{
   switch (type)
   {
   case UNIT_TYPE::UT_DRONE:
      return m_settings.unit_stt.air_units.size();
   case UNIT_TYPE::UT_ROVER:
      return m_settings.unit_stt.land_units.size();
   }
   return 0;
}

const iUnit& RobotScenarioPlayer::GetUnit(UNIT_TYPE type, size_t idx) const
{
   switch (type)
   {
   case UNIT_TYPE::UT_ROVER:
      return m_rovers.at(idx);
   case UNIT_TYPE::UT_DRONE:
      return m_drones.at(idx);
   case UNIT_TYPE::UT_SHIP:
   default:
      return m_ships.at(idx);
   }
}

double RobotScenarioPlayer::GetTime() const
{
   return 1.;
}

bool RobotScenarioPlayer::PrepareDataForSave(/*const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, */const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const
{
   return false;
}

void RobotScenarioPlayer::addUnit(const settings::point_setting_element& setting, UNIT_TYPE type)
{
   switch (type)
   {
   case UNIT_TYPE::UT_DRONE:
   {
      SimulationDrone drone;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start);
      
      drone.SetPosInfo(info);
      m_drones.emplace_back(std::move(drone));
      return;
   }
   case UNIT_TYPE::UT_ROVER:
   {
      SimulationRover rover;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start);
      info.point.course = 0;
      info.point.heading = 0;
      info.point.left_XTE = 1;
      info.point.right_XTE = 0;
      info.point.speed = 10;
      rover.SetPosInfo(info);
      colreg::ship_info sInfo;
      const std::string& strName = setting.name;
      memcpy(sInfo.name, &strName[0], strName.size());
      sInfo.length = 100;
      sInfo.width = 20;
      sInfo.draft = 5;
      sInfo.safety_contour = 5;
      rover.SetInfo(sInfo);
      m_rovers.emplace_back(std::move(rover));
      return;
   }
   case UNIT_TYPE::UT_SHIP:
   {
      SimulationShip ship;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start);
      ship.SetPosInfo(info);
      m_ships.emplace_back(std::move(ship));
      return;
   }
   }
   
}

void RobotScenarioPlayer::updateUnitsPath()
{
   for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_DRONE); idx++)
   {
      auto& unit = GetUnit(UNIT_TYPE::UT_DRONE, idx);
     
      /*if (ship.GetSimulationSettings().calculate_chart_context || ship.GetSimulationSettings().calculate_domain_border)
         chartContext = getTrackChartContext(ship.Info(), track, _scenarioSettings.environment.chart_settings.analysisObjectsMask);
      if (ship.GetSimulationSettings().calculate_domain_border)
         domainBorder = getTrackDomainBorderInfo(ship.Info().id, track, &chartContext);*/
      //ColregTrackPoints track = getTrack(pColregModel, ship.GetPos().point, ship.ProjToRoute().time,
         //_scenarioSettings.setting.timePrediction, _scenarioSettings.settings.timeStep);

      //ship.SetModelTrack(std::move(track), {},
         //ship.GetSimulationSettings().calculate_chart_context ? std::move(chartContext) : ColregChartContext(), std::move(domainBorder));
      //ship.SetModelRoute(getRoute(pColregModel));
   }
}

void RobotScenarioPlayer::addUnitsFromScenario()
{
   auto& air_units = m_settings.unit_stt.air_units;

   for (auto& elem : air_units)
   {
      addUnit(elem, UNIT_TYPE::UT_DRONE);
   }

   auto& land_units = m_settings.unit_stt.land_units;

   for (auto& elem : land_units)
   {
      addUnit(elem, UNIT_TYPE::UT_ROVER);
   }
}