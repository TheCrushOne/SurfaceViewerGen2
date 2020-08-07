#include "stdafx.h"
#include "RobotScenarioPlayer.h"
#include "colreg\ColregContainers.h"
#include "SVCG/positioning.h"
#include "math/math_utils.h"
#include "Debug/DebugInfoImpl.h"

using namespace ColregSimulation;

RobotScenarioPlayer::RobotScenarioPlayer(central_pack* pack, iPropertyInterface* prop)
   : SimulatorBase(pack, prop)
{
   m_debugInfo = dbg::CreateDebugInfoManager();
}

void RobotScenarioPlayer::Start()
{
   std::thread(&RobotScenarioPlayer::start, this, [this]() { GetPack()->comm->UpdateUI(); }).detach();
}

void RobotScenarioPlayer::start(std::function<void(void)> callback)
{
   //m_settings.env_stt.gcs_info.scale = 0.001; // вроде как это вынесено в настройки...теперь
   ATLASSERT(false);
   // TODO: переделать!!!
   /*m_databaseController->LoadScenarioData(m_data.unit_data, m_coordGrid);
   addUnitsFromScenario();
   correctCoordinateGrid();
   converter::raw_data_ref ref;
   std::vector<converter::data_line_ref> lines;
   for (auto& elem : m_coordGrid)
      lines.emplace_back(converter::data_line_ref{ elem.data(), elem.size() });
   ref = converter::raw_data_ref{ lines.data(), lines.size() };
   m_generator->GenerateStatic(ref);
   callback();
   SetSimulatorScenarioState(ColregSimulation::SCENARIO_STATUS::SS_PAUSE);*/
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
   m_currentIdx = controlPointIdx;
   moveUnits();
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
   m_currentIdx++;
   moveUnits();
}

void RobotScenarioPlayer::moveUnits()
{
   ATLASSERT(false);
   settings::environment_settings env_stt;
   if (m_currentIdx >= m_dronesIdxVct.size())
   {
      Stop();  // NOTE: остановка по окончании симуляции
      return;
   }

   auto& paths = m_engine->GetLastProcessedPaths();

   for (size_t idx = 0; idx < m_drones.size(); idx++)
   {
      auto pos = m_drones.at(idx)->GetPos();
      auto& rl = paths.air_routes.at(idx).route_list;

      auto rlIdx = m_dronesIdxVct.at(m_currentIdx).at(idx);
      if (rlIdx < rl.size())
      {
         pos.point.pos = SVCG::RoutePointToPositionPoint(rl.at(rlIdx), /*GetSettings()->*/env_stt);
         if (rl.size() > rlIdx + 1)
         {
            auto nextPoint = SVCG::RoutePointToPositionPoint(rl.at(rlIdx + 1), /*GetSettings()->*/env_stt);
            pos.point.heading = math::direction(pos.point.pos, static_cast<colreg::geo_point>(nextPoint));
            pos.point.course = pos.point.heading;
            pos.point.speed = m_defaultUnitSpeed;
         }
         else
            pos.point.speed = 0.;
         m_drones.at(idx)->SetPosInfo(pos);
      }
   }

   for (size_t idx = 0; idx < m_rovers.size(); idx++)
   {
      auto pos = m_rovers.at(idx)->GetPos();
      auto& rl = paths.land_routes.at(idx).route_list;

      auto rlIdx = m_roversIdxVct.at(m_currentIdx).at(idx);
      if (rlIdx < rl.size())
      {
         pos.point.pos = SVCG::RoutePointToPositionPoint(rl.at(rlIdx), /*GetSettings()->*/env_stt);
         if (rl.size() > rlIdx + 1)
         {
            auto nextPoint = SVCG::RoutePointToPositionPoint(rl.at(rlIdx + 1), /*GetSettings()->*/env_stt);
            pos.point.heading = math::direction(pos.point.pos, static_cast<colreg::geo_point>(nextPoint));
            pos.point.course = pos.point.heading;
            pos.point.speed = m_defaultUnitSpeed;
         }
         else
            pos.point.speed = 0.;
         m_rovers.at(idx)->SetPosInfo(pos);
      }
   }
}

size_t RobotScenarioPlayer::GetCurrentControlPointIdx() const
{
   return m_currentIdx;
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
   // NOTE: Отключено для отладки отрисовки изолиний
   m_engine->ProcessPathFind(m_data, m_coordGrid, [this]() { updateUnitsPath(); GetPack()->comm->OnPathFound(); });
   m_currentIdx = 0;
}

void RobotScenarioPlayer::RecountResearch()
{
   // NOTE: пока так
   m_engine->LaunchResearch([this]() { LogResearchResult(); });
}

void RobotScenarioPlayer::LogResearchResult()
{
   ATLASSERT(false);
   settings::research_settings res_stt;
   switch (/*GetSettings()->*/res_stt.res_type)
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

const iUnit* RobotScenarioPlayer::GetUnitById(colreg::id_type id) const
{
   bool find = false;
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

   return res;
}

const iUnit* RobotScenarioPlayer::GetUnitByIdx(UNIT_TYPE type, size_t idx) const
{
   return const_cast<RobotScenarioPlayer*>(this)->getUnitByIdx(type, idx);
}

double RobotScenarioPlayer::GetTime() const
{
   return 1.;
}

const settings::map_settings& RobotScenarioPlayer::GetChartGridMeta() const
{
   ATLASSERT(false);
   settings::map_settings map_stt;
   return /*GetSettings()->*/map_stt;
}

const colreg::chart_objects_ref& RobotScenarioPlayer::GetChartObjects() const
{
   ATLASSERT(false);
   colreg::chart_objects_ref ref;
   return ref;//m_generator->GetChartObjects();
}

const colreg::chart_object* RobotScenarioPlayer::GetChartObject(colreg::chart_object_id id) const
{
   for (auto& chartObject : GetChartObjects())
   {
      if (chartObject.id == id)
         return &chartObject;
   }
}

bool RobotScenarioPlayer::PrepareDataForSave(/*const ScenarioIO::scenario_data* pInputScenarioData, ScenarioIO::scenario_data* pScenarioData, */const bool focused, const colreg::geo_points_ref& ships, const colreg::base_ref<colreg::geo_points_ref>& chart_objects) const
{
   return false;
}

SimulationUnit* RobotScenarioPlayer::getUnitByIdx(UNIT_TYPE type, size_t idx)
{
   switch (type)
   {
   case UNIT_TYPE::UT_ROVER:
      return m_rovers.at(idx).get();
   case UNIT_TYPE::UT_DRONE:
      return m_drones.at(idx).get();
   case UNIT_TYPE::UT_SHIP:
      return m_ships.at(idx).get();
   default:
      break;
   }
   return nullptr;
}

void RobotScenarioPlayer::addUnit(const settings::unit_data_element& setting, UNIT_TYPE type, size_t uid)
{
   ATLASSERT(false);
   settings::environment_settings env_stt;
   switch (type)
   {
   case UNIT_TYPE::UT_DRONE:
   {
      SimulationDrone drone;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start, /*GetSettings()->*/env_stt);
      info.point.course = 0;
      info.point.heading = 0;
      info.point.left_XTE = 1;
      info.point.right_XTE = 0;
      info.point.speed = m_defaultUnitSpeed;
      drone.SetPosInfo(info);
      colreg::ship_info sInfo;
      const std::string& strName = setting.name;
      memcpy(sInfo.name, &strName[0], strName.size());
      sInfo.id = uid;
      sInfo.length = 100;
      sInfo.width = 20;
      sInfo.draft = 5;
      sInfo.safety_contour = 5;
      drone.SetInfo(sInfo);
      m_drones.emplace_back(std::make_unique<SimulationDrone>(std::move(drone)));
      return;
   }
   case UNIT_TYPE::UT_ROVER:
   {
      SimulationRover rover;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start, /*GetSettings()->*/env_stt);
      info.point.course = 0;
      info.point.heading = 0;
      info.point.left_XTE = 1;
      info.point.right_XTE = 0;
      info.point.speed = m_defaultUnitSpeed;
      rover.SetPosInfo(info);
      colreg::ship_info sInfo;
      const std::string& strName = setting.name;
      memcpy(sInfo.name, &strName[0], strName.size());
      sInfo.id = uid;
      sInfo.length = 100;
      sInfo.width = 20;
      sInfo.draft = 5;
      sInfo.safety_contour = 5;
      rover.SetInfo(sInfo);
      m_rovers.emplace_back(std::make_unique<SimulationRover>(std::move(rover)));
      return;
   }
   case UNIT_TYPE::UT_SHIP:
   {
      SimulationShip ship;
      track_point_full_info info;
      info.point.pos = SVCG::RoutePointToPositionPoint(setting.start, /*GetSettings()->*/env_stt);
      ship.SetPosInfo(info);
      m_ships.emplace_back(std::make_unique<SimulationShip>(std::move(ship)));
      return;
   }
   }
}

void RobotScenarioPlayer::correctCoordinateGrid()
{
}

void RobotScenarioPlayer::createIndexMaps()
{
   m_roversIdxVct.clear();
   m_dronesIdxVct.clear();

   //m_maxStepIdx = 0;
   auto& paths = m_engine->GetLastProcessedPaths();

   size_t stepIdx = 0;
   size_t dronePointIdx = 0;
   bool dronesFinished = false;
   while (!dronesFinished)
   {
      dronesFinished = true;
      m_dronesIdxVct.emplace_back();
      m_roversIdxVct.emplace_back();
      ATLASSERT(m_roversIdxVct.size() == m_dronesIdxVct.size());
      ATLASSERT(m_roversIdxVct.size() == stepIdx + 1);
      auto& drCell = m_dronesIdxVct.at(stepIdx);
      auto& rvCell = m_roversIdxVct.at(stepIdx);
      drCell.resize(GetUnitCount(UNIT_TYPE::UT_DRONE));
      rvCell.resize(GetUnitCount(UNIT_TYPE::UT_ROVER));

      for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_DRONE); idx++)
      {
         auto& routeData = paths.air_routes.at(idx);

         if (routeData.route_list.size() > dronePointIdx)
         {
            drCell[idx] = dronePointIdx;
            dronesFinished = false;
         }
         else
            drCell[idx] = idx > 0 ? drCell[idx - 1] : 0;
      }

      for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_ROVER); idx++)
      {
         rvCell[idx] = 0;
      }

      stepIdx++;
      dronePointIdx++;
   }

   size_t roverPointIdx = 0;
   bool roversFinished = false;
   while (!roversFinished)
   {
      roversFinished = true;
      m_dronesIdxVct.emplace_back();
      m_roversIdxVct.emplace_back();
      ATLASSERT(m_roversIdxVct.size() == m_dronesIdxVct.size());
      ATLASSERT(m_roversIdxVct.size() == stepIdx + 1);
      auto& drCell = m_dronesIdxVct.at(stepIdx);
      auto& rvCell = m_roversIdxVct.at(stepIdx);
      drCell.resize(GetUnitCount(UNIT_TYPE::UT_DRONE));
      rvCell.resize(GetUnitCount(UNIT_TYPE::UT_ROVER));

      for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_ROVER); idx++)
      {
         auto& routeData = paths.land_routes.at(idx);

         if (routeData.route_list.size() > roverPointIdx)
         {
            rvCell[idx] = roverPointIdx;
            roversFinished = false;
         }
         else
            rvCell[idx] = idx > 0 ? rvCell[idx - 1] : 0;
      }

      for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_DRONE); idx++)
      {
         drCell[idx] = 0;
      }

      stepIdx++;
      roverPointIdx++;
   }
}

void RobotScenarioPlayer::updateUnitsPath()
{
   ATLASSERT(false);
   settings::environment_settings env_stt;
   auto& paths = m_engine->GetLastProcessedPaths();
   if (GetUnitCount(UNIT_TYPE::UT_ROVER) != paths.land_routes.size()
      || GetUnitCount(UNIT_TYPE::UT_DRONE) != paths.air_routes.size())
      return;
   for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_ROVER); idx++)
   {
      auto* unit = getUnitByIdx(UNIT_TYPE::UT_ROVER, idx);
      auto& routeData = paths.land_routes.at(idx);

      /*if (ship.GetSimulationSettings().calculate_chart_context || ship.GetSimulationSettings().calculate_domain_border)
         chartContext = getTrackChartContext(ship.Info(), track, _scenarioSettings.environment.chart_settings.analysisObjectsMask);*/
      //if (/*ship.GetSimulationSettings().calculate_domain_border*/true)
         //domainBorder = getTrackDomainBorderInfo(ship.Info().id, track, &chartContext);
      //ColregTrackPoints track = getTrack(pColregModel, ship.GetPos().point, ship.ProjToRoute().time,
         //_scenarioSettings.setting.timePrediction, _scenarioSettings.settings.timeStep);

      ColregRoutePoints route;
      for (const auto& point : routeData.route_list)
         route.emplace_back(SVCG::RoutePointToPositionPoint(point, /*GetSettings()->*/env_stt));
      unit->SetSrcRoute(std::forward<ColregRoutePoints>(route));

      ColregRoutePoints cplist;
      for (const auto& point : routeData.control_point_list)
         cplist.emplace_back(SVCG::RoutePointToPositionPoint(point, /*GetSettings()->*/env_stt));
      unit->SetSrcContolPoints(std::forward<ColregRoutePoints>(cplist));
      //for (a)
      //ship.SetModelTrack(std::move(track), {},
         //ship.GetSimulationSettings().calculate_chart_context ? std::move(chartContext) : ColregChartContext(), std::move(domainBorder));
      //ship.SetModelRoute(getRoute(pColregModel));
   }

   for (size_t idx = 0; idx < GetUnitCount(UNIT_TYPE::UT_DRONE); idx++)
   {
      auto* unit = getUnitByIdx(UNIT_TYPE::UT_DRONE, idx);
      auto& routeData = paths.air_routes.at(idx);

      ColregRoutePoints route;
      for (const auto& point : routeData.route_list)
         route.emplace_back(SVCG::RoutePointToPositionPoint(point, /*GetSettings()->*/env_stt));
      unit->SetSrcRoute(std::forward<ColregRoutePoints>(route));

      ColregRoutePoints cplist;
      for (const auto& point : routeData.control_point_list)
         cplist.emplace_back(SVCG::RoutePointToPositionPoint(point, /*GetSettings()->*/env_stt));
      unit->SetSrcContolPoints(std::forward<ColregRoutePoints>(cplist));
   }
   GetPack()->comm->UpdateUI();

   // NOTE: повторно тоже работает
   createIndexMaps();
}

void RobotScenarioPlayer::addUnitsFromScenario()
{
   auto& air_units = m_data.unit_data.air_units;
   size_t uid = 0;

   for (auto& elem : air_units)
   {
      addUnit(elem, UNIT_TYPE::UT_DRONE, uid++);
   }

   auto& land_units = m_data.unit_data.land_units;

   for (auto& elem : land_units)
   {
      addUnit(elem, UNIT_TYPE::UT_ROVER, uid++);
   }
}