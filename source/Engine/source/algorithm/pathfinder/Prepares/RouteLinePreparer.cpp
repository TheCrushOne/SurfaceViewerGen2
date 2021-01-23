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

   // NOTE: Миграция чего-то непонятно чего...
   // TODO: разобраться!!!
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
      task.runnable = [this, &task]()
      {
         this->m_pathfinder->FindUniversalPath(task.start, task.finish, task.logic, task.rawdata, task.coverageMatrix, &task.path_found);
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

   for (size_t idx = 0; idx < waypointList.size() - 1; idx++)
   {
      path_finder_task task(landAffilationChecker, landCorrector);
      task.start = waypointList.at(idx);
      task.finish = waypointList.at(idx + 1);
      task.unit_index = unit_idx;
      task.shard_index = idx;
      task.rawdata = m_rawdata;
      task.coverageMatrix = m_coverage;
      task.runnable = [this, &task]()
      {
         this->m_pathfinder->FindUniversalPath(task.start, task.finish, task.logic, task.rawdata, task.coverageMatrix, &task.path_found);
      };
      m_currentTaskList.emplace_back(task);
   }
}