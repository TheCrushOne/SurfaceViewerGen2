#include "stdafx.h"
#include "RobotLogPlayer.h"

using namespace ColregSimulation;

RobotLogPlayer::RobotLogPlayer(central_pack* pack, iPropertyInterface* prop, navigation_dispatcher::iComServicePtr service)
   : SimulatorBase(pack, prop, service)
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

void RobotLogPlayer::ReloadSettings()
{}

void RobotLogPlayer::RecountRoutes()
{}

void RobotLogPlayer::RecountResearch()
{}

void RobotLogPlayer::LogResearchResult()
{}

size_t RobotLogPlayer::GetUnitCount(UNIT_TYPE type) const
{
   return 0;
}

const iUnit* RobotLogPlayer::GetUnitById(SVCG::id_type id) const
{
   return nullptr;
}

const iUnit* RobotLogPlayer::GetUnitByIdx(UNIT_TYPE type, size_t idx) const
{
   return nullptr;
}

double RobotLogPlayer::GetTime() const
{
   return 0.;
}

const settings::map_settings& RobotLogPlayer::GetChartGridMeta() const
{
   static settings::map_settings meta;
   return meta;
}

const chart_object::chart_object_unit_vct& RobotLogPlayer::GetChartObjects() const
{
   return const_cast<const chart_object::chart_object_unit_vct&>(m_chartObjects);
}

const chart_object::chart_object_unit* RobotLogPlayer::GetChartObject(SVCG::chart_object_id id) const
{
   return nullptr;
}

bool RobotLogPlayer::PrepareDataForSave(const bool focused, const SVCG::geo_contour_vct& ships, const chart_object::chart_object_unit_vct& chart_objects) const
{
   return false;
}