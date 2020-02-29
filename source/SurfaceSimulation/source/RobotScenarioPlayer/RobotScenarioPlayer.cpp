#include "stdafx.h"
#include "RobotScenarioPlayer.h"

using namespace ColregSimulation;

RobotScenarioPlayer::RobotScenarioPlayer(ICommunicator* pCommunicator, iPropertyInterface* prop)
   : SimulatorBase(pCommunicator, prop)
{}

void RobotScenarioPlayer::Start()
{
   Message(ICommunicator::MS_Info, "scen sim triggered");
   m_databaseController->LoadScenarioData(m_settings, &m_coordGrid);
   int i = 0;
}

void RobotScenarioPlayer::Stop()
{}

void RobotScenarioPlayer::Reset()
{}

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