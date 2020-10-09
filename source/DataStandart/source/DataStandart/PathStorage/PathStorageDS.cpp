#include "stdafx.h"
#include "PathStorageDS.h"
#include "crossdllinterface\ChecksumServiceInterface.h"

#include <filesystem>
#include <fstream>

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
   Json::Value j;
   auto routePointWrite = [](const SVCG::route_point& point)->Json::Value
   {
      Json::Value pt;
      pt[tag::col] = point.col;
      pt[tag::row] = point.row;
      pt[tag::go] = static_cast<unsigned short>(point.go);
      pt[tag::fly] = static_cast<unsigned short>(point.fly);
      pt[tag::height] = point.height;
      pt[tag::is_control] = point.is_control;
      return pt;
   };
   auto routeWrite = [routePointWrite](const settings::route& route)->Json::Value
   {
      Json::Value rt;
      rt[tag::start] = routePointWrite(route.start);
      rt[tag::finish] = routePointWrite(route.finish);
      Json::Value cpl(Json::arrayValue);
      for (const auto& rt : route.control_point_list)
         cpl.append(routePointWrite(rt));
      rt[tag::control_point_list] = cpl;
      Json::Value rl(Json::arrayValue);
      for (const auto& rt : route.route_list)
         rl.append(routePointWrite(rt));
      rt[tag::route_list] = cpl;
      return rt;
   };
   auto pathListWriter = [routeWrite](const std::vector<settings::route>& route_list)->Json::Value
   {
      Json::Value routes(Json::arrayValue);
      for (const auto& route : route_list)
         routes.append(routeWrite(route));
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
   Json::Value j;
   file >> j;
   auto routePointRead = [](const Json::Value& j)->SVCG::route_point
   {
      SVCG::route_point point;
      point.col = j[tag::col].asInt();
      point.row = j[tag::row].asInt();
      point.go = static_cast<pathfinder::GoZoneAffilation>(j[tag::go].asUInt());
      point.fly = static_cast<pathfinder::FlyZoneAffilation>(j[tag::fly].asUInt());
      point.height = j[tag::height].asDouble();
      point.is_control = j[tag::is_control].asBool();
      return point;
   };
   auto routeRead = [routePointRead](const Json::Value& jroute)->settings::route
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
   auto pathListRead = [routeRead](std::vector<settings::route>& route_list, const Json::Value& jroute_list)
   {
      for (const auto& jroute : jroute_list)
         route_list.emplace_back(routeRead(jroute));
   };
   //pathListRead(m_paths.air_routes, j[tag::air_routes]);
   //pathListRead(m_paths.land_routes, j[tag::land_routes]);
}

iDataStandart* CreatePathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new PathStorageDataStandart(pack, base_folder, pService);
}