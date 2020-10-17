#include "stdafx.h"
#include "RobotScenarioPlayer.h"
#include "colreg\ColregContainers.h"
#include "SVCG/positioning.h"
#include "math/math_utils.h"
#include "Debug/DebugInfoImpl.h"

using namespace ColregSimulation;

RobotScenarioPlayer::RobotScenarioPlayer(central_pack_ptr pack, iPropertyInterface* prop, navigation_dispatcher::iComServicePtr service)
   : SimulatorBase(pack, prop, service)
   , m_gridMeta({})
{
   //m_debugInfo = dbg::CreateDebugInfoManager();
}

void RobotScenarioPlayer::Start()
{
   std::thread(&RobotScenarioPlayer::start, this, [this]() { GetPack()->comm->UpdateUI(); }).detach();
}

void RobotScenarioPlayer::start(std::function<void(void)> callback)
{
   //m_settings.env_stt.gcs_info.scale = 0.001; // вроде как это вынесено в настройки...теперь
   // TODO: переделать!!!
   /*m_databaseController->LoadScenarioData(m_data.unit_data, m_coordGrid);
   addUnitsFromScenario();
   correctCoordinateGrid();
   converter::raw_data_ref ref;
   std::vector<converter::data_line_ref> lines;
   for (auto& elem : m_coordGrid)
      lines.emplace_back(converter::data_line_ref{ elem.data(), elem.size() });
   ref = converter::raw_data_ref{ lines.data(), lines.size() };
   m_generator->GenerateStatic(ref);*/
   callback();
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
   return m_dronesIdxVct.size();
}

const control_point_info& RobotScenarioPlayer::GetControlPointInfo(size_t controlPntIdx) const
{
   return control_point_info{ 0 };
}

bool RobotScenarioPlayer::PlayFrom(size_t controlPointIdx)
{
   if (m_data.current_step >= m_data.step_count)
      return;
   m_data.current_step = controlPointIdx;
   return true;
}

bool RobotScenarioPlayer::NextControlPoint()
{
   step();
   // NOTE: не нужен, перерасчета нет
   //updateUnitsPath();
   return false;
}

void RobotScenarioPlayer::step()
{
   if (m_data.current_step >= m_data.step_count)
   {
      Stop();  // NOTE: остановка по окончании симул€ции
      return;
   }
   m_data.current_step++;
}

size_t RobotScenarioPlayer::GetCurrentControlPointIdx() const
{
   return m_data.current_step;
}

bool RobotScenarioPlayer::SaveLogPair(const char* filename) const
{
   return false;
}

const ColregSimulation::SIMULATION_PLAYER_TYPE RobotScenarioPlayer::GetSimulationType()
{
   return ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_LOG;
}

void RobotScenarioPlayer::ReloadSettings()
{}

void RobotScenarioPlayer::RecountRoutes()
{
   // NOTE: ќтключено дл€ реализации приказного интерфейса
   //m_engine->ProcessPathFind(m_data, m_coordGrid, [this]() { updateUnitsPath(); GetPack()->comm->OnPathFound(); });
   //m_currentIdx = 0;
}

void RobotScenarioPlayer::RecountResearch()
{
   // NOTE: пока так
   m_engine->LaunchResearch([this]() { LogResearchResult(); });
}

void RobotScenarioPlayer::LogResearchResult()
{
   const settings::research_settings& res_stt = GetAppSettings().res_stt;
   switch (res_stt.res_type)
   {
   case settings::ResearchType::RT_TIME:
   {
      break;
   }
   case settings::ResearchType::RT_LENGTH:
   {
      break;
   }
   case settings::ResearchType::RT_THREAD:
   {
      m_logger->LogThreadResearchResult(m_engine->GetThreadResearchResult());
      break;
   }
   }
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

const iUnit* RobotScenarioPlayer::GetUnitById(SVCG::id_type id) const
{
   /*bool find = false;
   auto idChecker = [this, id, &find](ColregSimulation::UNIT_TYPE type) -> const iUnit*
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
   const iUnit* res = nullptr;
   res = idChecker(ColregSimulation::UNIT_TYPE::UT_DRONE) ? idChecker(ColregSimulation::UNIT_TYPE::UT_DRONE)
      : (idChecker(ColregSimulation::UNIT_TYPE::UT_ROVER) ? idChecker(ColregSimulation::UNIT_TYPE::UT_ROVER)
      : idChecker(ColregSimulation::UNIT_TYPE::UT_SHIP));

   return res;*/
   return nullptr;
}

const settings::unit_data_element& RobotScenarioPlayer::GetUnitByIdx(UNIT_TYPE type, size_t idx) const
{
   return const_cast<RobotScenarioPlayer*>(this)->getUnitByIdx(type, idx);
}

double RobotScenarioPlayer::GetTime() const
{
   return 1.;
}

const settings::map_settings& RobotScenarioPlayer::GetChartGridMeta() const
{   
   return GetAppSettings().map_stt;
}

const math::geo_contour_vct_ref RobotScenarioPlayer::GetChartObjects() const
{
   return const_cast<const math::geo_contour_vct_ref>(m_chartObjects.geom_contour_vct);
}

const chart_object::chart_object_unit RobotScenarioPlayer::GetChartObject(SVCG::chart_object_id id) const
{
   for (auto& chartObject : GetChartObjects())
   {
      if (chartObject.id == id)
         return chartObject;
   }
   return nullptr;
}

bool RobotScenarioPlayer::PrepareDataForSave(const bool focused, const SVCG::geo_contour_vct& ships, const chart_object::chart_object_unit_vct& chart_objects) const
{
   return false;
}

settings::unit_data_element& RobotScenarioPlayer::getUnitByIdx(UNIT_TYPE type, size_t idx)
{
   settings::unit_data_element spike;
   switch (type)
   {
   case UNIT_TYPE::UT_ROVER:
      return m_data.unit_data.land_units.at(idx);
   case UNIT_TYPE::UT_DRONE:
      return m_data.unit_data.air_units.at(idx);
   case UNIT_TYPE::UT_SHIP:
   default:
   }
   ATLASSERT(false);
   return spike;
}