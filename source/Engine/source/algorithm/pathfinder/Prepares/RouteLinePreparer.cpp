#include "stdafx.h"
#include "RouteLinePreparer.h"

using namespace SV::pathfinder;

RouteLinePreparer::RouteLinePreparer(central_pack* pack)
   : m_pathfinder(std::make_unique<PathFinder>())
   , Central(pack)
{}

RouteLinePreparer::~RouteLinePreparer()
{}

void RouteLinePreparer::PrepareTaskList(std::vector<SV::settings::route> routeList, bool isAir)
{
   m_currentTaskList.clear();
   for (size_t idx = 0; idx < routeList.size(); idx++)
   {
      auto& route = routeList.at(idx);
      isAir ? prepareAirPath(idx, route) : prepareLandPath(idx, route);
   }
}

void RouteLinePreparer::RestorePathList(std::vector<SV::settings::route> routeList, bool isAir)
{
   for (size_t idx = 0; idx < routeList.size(); idx++)
   {
      auto& route = routeList.at(idx);
      isAir ? restoreAirPath(idx, route) : restoreLandPath(idx, route);
   }
}

namespace
{
   bool airCheckAffilation(const SharedRoutePointMatrix& data, const SharedUnsignedMatrix& coverageMatrix, size_t row, size_t col)
   {
      return data->Get(row, col).fly == FlyZoneAffilation::FZA_FORBIDDEN;
   }

   float airCorrector(float y)
   {
      return y + 50.f;
   }
}

void RouteLinePreparer::prepareAirPath(size_t unit_idx, SV::settings::route& route)
{
   CG::route_line waypointList;
   waypointList.emplace_back(route.start);
   waypointList.insert(waypointList.end(), route.control_point_list.begin(), route.control_point_list.end());
   waypointList.emplace_back(route.finish);
   this->m_pathStorage.air_path_map[unit_idx] = std::unordered_map<size_t, CG::route_line>();

   // NOTE: ћиграци€ чего-то непон€тно чего...
   // TODO: разобратьс€!!!
   for (auto& item : waypointList)
      item = m_rawdata->Get(item.row, item.col);

   //std::string wplist;
   //for (const auto& wp : waypointList)
      //wplist.append(std::string("[" + std::to_string(wp.row) + ";" + std::to_string(wp.col) + "]").c_str());
   /*THREADDEBUG("st-fn: "
            << std::string("[" + std::to_string(route.start.mp.row) + ";" + std::to_string(route.start.mp.col) + "]").c_str()
            << std::string("[" + std::to_string(route.finish.mp.row) + ";" + std::to_string(route.finish.mp.col) + "]").c_str());
   THREADDEBUG("wp list: " << waypointList.size() << " elems: " << wplist.c_str());*/

   for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   {
      path_finder_task task(airCheckAffilation, airCorrector);
      task.start = waypointList.at(idx);
      task.finish = waypointList.at(idx + 1);
      task.unit_index = unit_idx;
      task.shard_index = idx;
      task.rawdata = m_rawdata;
      task.coverageMatrix = m_coverage;
      task.runnable = [this, unit_idx, idx](const CG::route_point& start, const CG::route_point& finish, const path_finder_logic& logic, const SharedRoutePointMatrix& rawdata, const SharedUnsignedMatrix& coverageMatrix, bool* pathFound)
      {
         this->m_pathStorage.land_path_map[unit_idx][idx] = this->m_pathfinder->FindUniversalPath(start, finish, logic, rawdata, coverageMatrix, pathFound);
      };
      m_currentTaskList.emplace_back(std::move(task));
   }
}

namespace
{
   bool landAffilationChecker(const SharedRoutePointMatrix& data, const SharedUnsignedMatrix& covMatrix, size_t row, size_t col)
   {
      return data->Get(row, col).go == GoZoneAffilation::GZA_FORBIDDEN || covMatrix->Get(row, col) == 0;
   }

   float landCorrector(float y)
   {
      // NOTE: корректировок нет
      return y;
   }
}

void RouteLinePreparer::prepareLandPath(size_t unit_idx, SV::settings::route& route)
{
   CG::route_line waypointList;
   waypointList.emplace_back(route.start);
   waypointList.insert(waypointList.end(), route.control_point_list.begin(), route.control_point_list.end());
   waypointList.emplace_back(route.finish);
   this->m_pathStorage.land_path_map[unit_idx] = std::unordered_map<size_t, CG::route_line>();

   for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   {
      path_finder_task task(landAffilationChecker, landCorrector);
      task.start = waypointList.at(idx);
      task.finish = waypointList.at(idx + 1);
      task.unit_index = unit_idx;
      task.shard_index = idx;
      task.rawdata = m_rawdata;
      task.coverageMatrix = m_coverage;
      task.runnable = [this, unit_idx, idx](const CG::route_point& start, const CG::route_point& finish, const path_finder_logic& logic, const SharedRoutePointMatrix& rawdata, const SharedUnsignedMatrix& coverageMatrix, bool* path_found)
      {
         this->m_pathStorage.land_path_map[unit_idx][idx] = this->m_pathfinder->FindUniversalPath(start, finish, logic, rawdata, coverageMatrix, path_found);
      };
      m_currentTaskList.emplace_back(task);
   }
}

void RouteLinePreparer::restoreAirPath(size_t unit_idx, SV::settings::route& route)
{
   const auto& pathShardList = m_pathStorage.air_path_map.at(unit_idx);
   for (const auto& pathShard : pathShardList)
   {
      auto lvPath = pathShard.second;
      std::reverse(lvPath.begin(), lvPath.end());
      if (lvPath.size() != 0)
      {
         route.route_list.insert(route.route_list.end(), ++lvPath.begin(), lvPath.end());
         route.route_list.back().height += 50.f;   // NOTE: вроде как обозначение кт
      }
   }
}

void RouteLinePreparer::restoreLandPath(size_t unit_idx, SV::settings::route& route)
{
   const auto& pathShardList = m_pathStorage.land_path_map.at(unit_idx);
   for (const auto& pathShard : pathShardList)
   {
      auto lvPath = pathShard.second;
      std::reverse(lvPath.begin(), lvPath.end());
      if (lvPath.size() != 0)
         route.route_list.insert(route.route_list.end(), ++lvPath.begin(), lvPath.end());
   }
}