#include "stdafx.h"
#include "PathStorageDS.h"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace data_standart;

void PathStorageDataStandart::resolvePathDee()
{

}

size_t PathStorageDataStandart::GetDataHash()
{
   return 1903;
}


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
      pt["col"] = point.col;
      pt["row"] = point.row;
      pt["go"] = point.go;
      pt["fly"] = point.fly;
      pt["height"] = point.height;
      pt["is_control"] = point.is_control;
      return pt;
   };
   auto routeWrite = [routePointWrite](const settings::route& route)->json
   {
      json rt;
      rt["start"] = routePointWrite(route.start);
      rt["finish"] = routePointWrite(route.finish);
      json cpl;
      for (auto& rp : route.control_point_list)
         cpl.emplace_back(routePointWrite(rp));
      rt["control_point_list"] = cpl;
      json rl;
      for (auto& rp : route.route_list)
         rl.emplace_back(routePointWrite(rp));
      rt["route_list"] = cpl;
      return rt;
   };
   auto pathListWriter = [routeWrite](const std::vector<settings::route>& route_list)->json
   {
      json routes;
      for (auto& route : route_list)
         routes.emplace_back(routeWrite(route));
      return routes;
   };
   j["air_routes"] = pathListWriter(m_paths.air_routes);
   j["land_routes"] = pathListWriter(m_paths.land_routes);
   file << j;
}

iDataStandart* CreatePathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new PathStorageDataStandart(pack, base_folder, pService);
}