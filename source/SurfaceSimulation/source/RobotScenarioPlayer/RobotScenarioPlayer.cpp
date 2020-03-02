#include "stdafx.h"
#include "RobotScenarioPlayer.h"

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
   m_engine->ProcessPathFind(m_routedata, m_coordGrid);
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

void RobotScenarioPlayer::SetSettings(const settings::application_settings& s)
{}

void RobotScenarioPlayer::ReloadSettings()
{}

const settings::application_settings& RobotScenarioPlayer::GetSettings() const
{
   return m_settings;
}