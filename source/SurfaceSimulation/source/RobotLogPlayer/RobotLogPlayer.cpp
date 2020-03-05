#include "stdafx.h"
#include "RobotLogPlayer.h"

using namespace ColregSimulation;

RobotLogPlayer::RobotLogPlayer(ICommunicator* pCommunicator, iPropertyInterface* prop)
   : SimulatorBase(pCommunicator, prop)
{}

void RobotLogPlayer::Start()
{}

void RobotLogPlayer::Stop()
{}

void RobotLogPlayer::Reset()
{}

bool RobotLogPlayer::IsRunning() const
{
   return false;
}

size_t RobotLogPlayer::GetControlPointsCount() const
{
   return 0;
}

const control_point_info& RobotLogPlayer::GetControlPointInfo(size_t controlPntIdx) const
{
   static control_point_info cp{ 0 };
   return cp;
}

bool RobotLogPlayer::PlayFrom(size_t controlPointIdx)
{
   return false;
}

bool RobotLogPlayer::NextControlPoint()
{
   return false;
}

size_t RobotLogPlayer::GetCurrentControlPointIdx() const
{
   return 0;
}

bool RobotLogPlayer::SaveLogPair(const char* filename) const
{
   return false;
}

const ColregSimulation::SIMULATION_PLAYER_TYPE RobotLogPlayer::GetSimulationType()
{
   return ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_LOG;
}

void RobotLogPlayer::SetAppSettings(const settings::application_settings& s)
{}

void RobotLogPlayer::ReloadSettings()
{}

const settings::application_settings& RobotLogPlayer::GetAppSettings() const
{
   return m_settings;
}

size_t RobotLogPlayer::GetUnitCount(UNIT_TYPE type) const
{
   return 0;
}

const iUnit& RobotLogPlayer::GetUnit(UNIT_TYPE type, size_t idx) const
{
   static SimulationShip ship;
   return ship;
}

double RobotLogPlayer::GetTime() const
{
   return 0.;
}

bool RobotLogPlayer::PrepareDataForSave(/*const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, */const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const
{
   return false;
}