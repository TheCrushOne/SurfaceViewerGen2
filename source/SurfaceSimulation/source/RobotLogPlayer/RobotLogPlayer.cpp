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
   return control_point_info{ 0 };
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

void RobotLogPlayer::SetSettings(const settings::application_settings& s)
{}

void RobotLogPlayer::ReloadSettings()
{}

const settings::application_settings& RobotLogPlayer::GetSettings() const
{
   return m_settings;
}