#include "stdafx.h"
#include "PathStorageDS.h"
#include "crossdllinterface\ChecksumServiceInterface.h"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace data_standart;

void PathStorageDataStandart::resolvePathDee()
{}

void PathStorageDataStandart::SetData(const pathfinder::route_data& paths)
{
   m_paths = paths;
   std::filesystem::path path(getPath());
   std::filesystem::create_directories(path);
   savePathData();
}

void PathStorageDataStandart::savePathData()
{
   std::string dataFilePath = getDataFilePath();
   std::ofstream file(dataFilePath);
   json j;
   auto routePointWrite = [](const SVCG::route_point& point)->json
   {
      json pt;
      pt[tag::col] = point.col;
      pt[tag::row] = point.row;
      pt[tag::go] = static_cast<unsigned short>(point.go);
      pt[tag::fly] = static_cast<unsigned short>(point.fly);
      pt[tag::height] = point.height;
      pt[tag::is_control] = point.is_control;
      return pt;
   };
   auto routeWrite = [routePointWrite](const settings::route& route)->json
   {
      json rt;
      rt[tag::start] = routePointWrite(route.start);
      rt[tag::finish] = routePointWrite(route.finish);
      json cpl;
      for (auto& rp : route.control_point_list)
         cpl.emplace_back(routePointWrite(rp));
      rt[tag::control_point_list] = cpl;
      json rl;
      for (auto& rp : route.route_list)
         rl.emplace_back(routePointWrite(rp));
      rt[tag::route_list] = cpl;
      return rt;
   };
   auto pathListWriter = [routeWrite](const std::vector<settings::route>& route_list)->json
   {
      json routes;
      for (auto& route : route_list)
         routes.emplace_back(routeWrite(route));
      return routes;
   };
   j[tag::air_routes] = pathListWriter(m_paths.air_routes);
   j[tag::land_routes] = pathListWriter(m_paths.land_routes);
   file << j;
}

const pathfinder::route_data& PathStorageDataStandart::GetData()
{
   readPathData();
   return m_paths;
}

void PathStorageDataStandart::readPathData()
{
   m_paths.air_routes.clear();
   m_paths.land_routes.clear();
   std::string dataFilePath = getDataFilePath();
   std::ifstream file(dataFilePath);
   json j;
   file >> j;
   auto routePointRead = [](const json& j)->SVCG::route_point
   {
      SVCG::route_point point;
      point.col = j[tag::col].get<int>();
      point.row = j[tag::row].get<int>();
      point.go = static_cast<pathfinder::GoZoneAffilation>(j[tag::go].get<unsigned short>());
      point.fly = static_cast<pathfinder::FlyZoneAffilation>(j[tag::fly].get<unsigned short>());
      point.height = j[tag::height].get<double>();
      point.is_control = j[tag::is_control].get<bool>();
      return point;
   };
   auto routeRead = [routePointRead](const json& jroute)->settings::route
   {
      settings::route route;
      route.start = routePointRead(jroute[tag::start]);
      route.finish = routePointRead(jroute[tag::finish]);
      for (auto& jrp : jroute[tag::control_point_list])
         route.control_point_list.emplace_back(routePointRead(jrp));
      for (auto& jrp : jroute[tag::route_list])
         route.route_list.emplace_back(routePointRead(jrp));
      return route;
   };
   auto pathListRead = [routeRead](std::vector<settings::route>& route_list, const json& jroute_list)
   {
      for (auto& jroute : jroute_list)
         route_list.emplace_back(routeRead(jroute));
   };
   pathListRead(m_paths.air_routes, j[tag::air_routes]);
   pathListRead(m_paths.land_routes, j[tag::land_routes]);
}

iDataStandart* CreatePathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new PathStorageDataStandart(pack, base_folder, pService);
}