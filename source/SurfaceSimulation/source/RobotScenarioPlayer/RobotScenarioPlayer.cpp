#include "stdafx.h"
#include "RobotScenarioPlayer.h"
#include "colreg\ColregContainers.h"
#include "SVCG/positioning.h"

using namespace ColregSimulation;

RobotScenarioPlayer::RobotScenarioPlayer(ICommunicator* pCommunicator, iPropertyInterface* prop)
   : SimulatorBase(pCommunicator, prop)
{
   m_engine.Create(SVGUtils::CurrentDllPath("Engine").c_str(), "CreateEngine");
   if (!m_engine.IsValid())
   {
      m_communicator->Message(ICommunicator::MS_Error, "Can't load 'Engine'");
      return;
   }

   m_generator.Create(SVGUtils::CurrentDllPath("ChartObjectGenerator").c_str(), "CreateGenerator");
   if (!m_generator.IsValid())
   {
      m_communicator->Message(ICommunicator::MS_Error, "Can't load 'ChartObjectGenerator'");
      return;
   }
}

void RobotScenarioPlayer::Start()
{
   m_databaseController->LoadScenarioData(m_settings, m_data.unit_data, m_coordGrid);
   m_settings.env_stt.gcs_info.scale = 0.001;
   m_generator->Init(m_communicator, &(m_settings.env_stt));
   addUnitsFromScenario();
   correctCoordinateGrid();
   converter::raw_data_ref ref;
   std::vector<converter::data_line_ref> lines;
   for (auto& elem : m_coordGrid)
      lines.emplace_back(converter::data_line_ref{ elem.data(), elem.size() });
   ref = converter::raw_data_ref{ lines.data(), lines.size() };
   m_generator->GenerateStatic(ref);
   m_engine->ProcessPathFind(m_data, m_coordGrid);
   updateUnitsPath();
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
      return m_data.unit_data.air_units.size();
   case UNIT_TYPE::UT_ROVER:
      return m_data.unit_data.land_units.size();
   }
   return 0;
}

const iUnit& RobotScenarioPlayer::GetUnit(UNIT_TYPE type, size_t idx) const
{
   return const_cast<RobotScenarioPlayer*>(this)->getUnit(type, idx);
}

double RobotScenarioPlayer::GetTime() const
{
   return 1.;
}

const colreg::chart_objects_ref& RobotScenarioPlayer::GetChartObjects() const
{
   return m_generator->GetChartObjects();
}

bool RobotScenarioPlayer::PrepareDataForSave(/*const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, */const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const
{
   return false;
}

SimulationUnit& RobotScenarioPlayer::getUnit(UNIT_TYPE type, size_t idx)
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

void RobotScenarioPlayer::addUnit(const settings::unit_data_element& setting, UNIT_TYPE type)
{
   switch (type)
   {
   case UNIT_TYPE::UT_DRONE:
   {
      SimulationDrone drone;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start, m_settings.env_stt);
      
      drone.SetPosInfo(info);
      m_drones.emplace_back(std::move(drone));
      return;
   }
   case UNIT_TYPE::UT_ROVER:
   {
      SimulationRover rover;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start, m_settings.env_stt);
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
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start, m_settings.env_stt);
      ship.SetPosInfo(info);
      m_ships.emplace_back(std::move(ship));
      return;
   }
   }
   
}

void RobotScenarioPlayer::correctCoordinateGrid()
{

}

void RobotScenarioPlayer::updateUnitsPath()
{
   for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_ROVER); idx++)
   {
      auto& unit = getUnit(UNIT_TYPE::UT_ROVER, idx);
     
      /*if (ship.GetSimulationSettings().calculate_chart_context || ship.GetSimulationSettings().calculate_domain_border)
         chartContext = getTrackChartContext(ship.Info(), track, _scenarioSettings.environment.chart_settings.analysisObjectsMask);*/
      //if (/*ship.GetSimulationSettings().calculate_domain_border*/true)
         //domainBorder = getTrackDomainBorderInfo(ship.Info().id, track, &chartContext);
      //ColregTrackPoints track = getTrack(pColregModel, ship.GetPos().point, ship.ProjToRoute().time,
         //_scenarioSettings.setting.timePrediction, _scenarioSettings.settings.timeStep);

      ColregRoutePoints route;
      for (const auto& point : m_data.unit_data.land_units.at(idx).route_point_list)
         route.emplace_back(SVCG::RoutePointToPositionPoint(point, m_settings.env_stt));
      unit.SetSrcRoute(std::forward<ColregRoutePoints>(route));
      //for (a)
      //ship.SetModelTrack(std::move(track), {},
         //ship.GetSimulationSettings().calculate_chart_context ? std::move(chartContext) : ColregChartContext(), std::move(domainBorder));
      //ship.SetModelRoute(getRoute(pColregModel));
   }

   for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_DRONE); idx++)
   {
      auto& unit = getUnit(UNIT_TYPE::UT_DRONE, idx);

      ColregRoutePoints route;
      for (const auto& point : m_data.unit_data.air_units.at(idx).route_point_list)
         route.emplace_back(SVCG::RoutePointToPositionPoint(point, m_settings.env_stt));
      unit.SetSrcRoute(std::forward<ColregRoutePoints>(route));
   }
}

void RobotScenarioPlayer::addUnitsFromScenario()
{
   auto& air_units = m_data.unit_data.air_units;

   for (auto& elem : air_units)
   {
      addUnit(elem, UNIT_TYPE::UT_DRONE);
   }

   auto& land_units = m_data.unit_data.land_units;

   for (auto& elem : land_units)
   {
      addUnit(elem, UNIT_TYPE::UT_ROVER);
   }
}