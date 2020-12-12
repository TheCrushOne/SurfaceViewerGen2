#include "stdafx.h"
#include "RobotScenarioPlayer.h"
#include "SVCG/positioning.h"
#include "math/math_utils.h"
#include "Debug/DebugInfoImpl.h"

using namespace SV;
using namespace SV::surface_simulation;

RobotScenarioPlayer::RobotScenarioPlayer(central_pack* pack, iPropertyInterface* prop, navigation_dispatcher::iComService* service)
   : SimulatorBase(pack, prop, service)
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

//size_t RobotScenarioPlayer::GetControlPointsCount() const
//{
   //return 0/*m_dronesIdxVct.size()*/;
//}

/*const control_point_info& RobotScenarioPlayer::GetControlPointInfo(size_t controlPntIdx) const
{
   return control_point_info{ 0 };
}*/

bool RobotScenarioPlayer::PlayFrom(size_t controlPointIdx)
{
   if (m_data.current_step >= m_data.step_count)
      return false;
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

const surface_simulation::SIMULATION_PLAYER_TYPE RobotScenarioPlayer::GetSimulationType()
{
   return surface_simulation::SIMULATION_PLAYER_TYPE::SPT_LOG;
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
   //m_researchEngine->LaunchResearch([this]() { LogResearchResult(); }, );
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
      m_logger->LogThreadResearchResult(m_researchEngine->GetThreadResearchResult());
      break;
   }
   }
}

double RobotScenarioPlayer::GetTime() const
{
   return 1.;
}

const settings::map_settings& RobotScenarioPlayer::GetChartGridMeta() const
{
   return GetAppSettings().map_stt;
}

bool RobotScenarioPlayer::PrepareDataForSave(const bool focused, const CG::geo_contour_vct& ships, const chart_object::chart_object_unit_vct& chart_objects) const
{
   return false;
}