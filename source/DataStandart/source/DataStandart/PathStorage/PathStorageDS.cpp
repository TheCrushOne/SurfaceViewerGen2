#include "stdafx.h"
#include "PathStorageDS.h"
#include "crossdllinterface\ChecksumServiceInterface.h"

#include <filesystem>
#include <fstream>

using namespace SV;
using namespace SV::data_standart;

void PathStorageDataStandart::resolvePathDee()
{}

const pathfinder::route_data& PathStorageDataStandart::GetData() const
{
   // HACK: ну...как обычно
   const_cast<PathStorageDataStandart*>(this)->readPathDataFromFile();
   return m_paths;
}

const pathfinder::UnsignedMatrix& PathStorageDataStandart::GetLandUnitExplication() const
{
   const_cast<PathStorageDataStandart*>(this)->readLandExplicationDataFromFile();
   return m_landExplication;
}

const pathfinder::UnsignedMatrix& PathStorageDataStandart::GetAirUnitExplication() const
{
   const_cast<PathStorageDataStandart*>(this)->readAirExplicationDataFromFile();
   return m_airExplication;
}

const std::vector<pathfinder::UnsignedMatrix>& PathStorageDataStandart::GetCoverageHistory() const
{
   const_cast<PathStorageDataStandart*>(this)->readCoverageDataFromFile();
   return m_coverageHistory;
}

void PathStorageDataStandart::SetData(const pathfinder::route_data& paths, const pathfinder::UnsignedMatrix& land, const pathfinder::UnsignedMatrix& air, const std::vector<pathfinder::SharedUnsignedMatrix>& coverages)
{
   m_paths = paths;
   m_landExplication = land;
   m_airExplication = air;
   for (const auto& coverage : coverages)
      m_coverageHistory.emplace_back(pathfinder::UnsignedMatrix(*coverage.get()));
   std::filesystem::path path(getPath());
   std::filesystem::create_directories(path);
   m_meta.row_count = land.GetRowCount();
   m_meta.col_count = land.GetColCount();
   ATLASSERT(land.GetRowCount() == air.GetRowCount());
   ATLASSERT(land.GetColCount() == air.GetColCount());
   saveMetaToFile();
   savePathDataToFile();
   saveCoverageDataToFile();
   saveLandExplicationDataToFile();
   saveAirExplicationDataToFile();
}

void PathStorageDataStandart::readMetaFromFile()
{
   std::string metaFilePath = getMetaFilePath();
   std::ifstream file(metaFilePath);
   Json::Value j;
   file >> j;
   m_meta.col_count = j[tag::col_count].asUInt();
   m_meta.row_count = j[tag::row_count].asUInt();
}

void PathStorageDataStandart::readPathDataFromFile()
{
   readMetaFromFile();
   m_paths.air_routes.clear();
   m_paths.land_routes.clear();
   std::string dataFilePath = getDataFilePath();
   std::ifstream file(dataFilePath);
   Json::Value j;
   file >> j;
   pathListRead(m_paths.air_routes, j[tag::air_routes]);
   pathListRead(m_paths.land_routes, j[tag::land_routes]);
}

void PathStorageDataStandart::readCoverageDataFromFile()
{
   readMetaFromFile();
   size_t covIdx = 0;
   std::string covFileName = getCoverageFilePath(covIdx++);
   while (std::filesystem::exists(covFileName))
   {
      m_coverageHistory.emplace_back(binary_load_helper::from_file<size_t>(covFileName.c_str(), m_meta.row_count, m_meta.col_count));
      covFileName = getCoverageFilePath(covIdx++);
   }
}

void PathStorageDataStandart::readLandExplicationDataFromFile(size_t idx)
{
   readMetaFromFile();
   std::string dataFilePath = getLandExplicationFilePath();
   m_landExplication = binary_load_helper::from_file<size_t>(dataFilePath.c_str(), m_meta.row_count, m_meta.col_count);
}

void PathStorageDataStandart::readAirExplicationDataFromFile(size_t idx)
{
   readMetaFromFile();
   std::string dataFilePath = getAirExplicationFilePath();
   m_airExplication = binary_load_helper::from_file<size_t>(dataFilePath.c_str(), m_meta.row_count, m_meta.col_count);
}

CG::route_point PathStorageDataStandart::routePointRead(const Json::Value& j)
{
   CG::route_point point;
   point.col = j[tag::col].asInt();
   point.row = j[tag::row].asInt();
   //point.go = static_cast<pathfinder::GoZoneAffilation>(j[tag::go].asUInt());
   //point.fly = static_cast<pathfinder::FlyZoneAffilation>(j[tag::fly].asUInt());
   //point.height = j[tag::height].asDouble();
   //point.is_control = j[tag::is_control].asBool();
   return point;
}

settings::route PathStorageDataStandart::routeRead(const Json::Value& jroute)
{
   settings::route route;
   route.start = routePointRead(jroute[tag::start]);
   route.finish = routePointRead(jroute[tag::finish]);
   for (auto& jrp : jroute[tag::control_point_list])
      route.control_point_list.emplace_back(routePointRead(jrp));
   for (auto& jrp : jroute[tag::route_list])
      route.route_list.emplace_back(routePointRead(jrp));
   return route;
}

void PathStorageDataStandart::saveMetaToFile()
{
   std::string metaDataPath = getMetaFilePath();
   std::ofstream o(metaDataPath);
   Json::Value j;
   j[tag::row_count] = m_meta.row_count;
   j[tag::col_count] = m_meta.col_count;
   o << j;
}

void PathStorageDataStandart::savePathDataToFile()
{
   std::string dataFilePath = getDataFilePath();
   std::ofstream file(dataFilePath);
   Json::Value j;
   j[tag::air_routes] = pathListWriter(m_paths.air_routes);
   j[tag::land_routes] = pathListWriter(m_paths.land_routes);
   file << j;
}

void PathStorageDataStandart::saveCoverageDataToFile()
{
   size_t covIdx = 0;
   for (const auto& coverage : m_coverageHistory)
   {
      std::string dataFilePath = getCoverageFilePath(covIdx++);
      binary_save_helper::to_file(dataFilePath.c_str(), coverage);
   }
}

void PathStorageDataStandart::saveLandExplicationDataToFile(size_t idx)
{
   std::string dataFilePath = getLandExplicationFilePath(idx);
   binary_save_helper::to_file(dataFilePath.c_str(), m_landExplication);
}

void PathStorageDataStandart::saveAirExplicationDataToFile(size_t idx)
{
   std::string dataFilePath = getAirExplicationFilePath(idx);
   binary_save_helper::to_file(dataFilePath.c_str(), m_airExplication);
}

Json::Value PathStorageDataStandart::routePointWrite(const CG::route_point& point)
{
   Json::Value pt;
   pt[PathStorageDataStandart::tag::col] = point.col;
   pt[tag::row] = point.row;
   //pt[tag::go] = static_cast<unsigned short>(point.go);
   //pt[tag::fly] = static_cast<unsigned short>(point.fly);
   //pt[tag::height] = point.height;
   //pt[tag::is_control] = point.is_control;
   return pt;
}

Json::Value PathStorageDataStandart::routeWrite(const settings::route& route)
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
}

Json::Value PathStorageDataStandart::pathListWriter(const std::vector<settings::route>& route_list)
{
   Json::Value routes(Json::arrayValue);
   for (const auto& route : route_list)
      routes.append(routeWrite(route));
   return routes;
}

void PathStorageDataStandart::pathListRead(std::vector<settings::route>& route_list, const Json::Value& jroute_list)
{
   for (const auto& jroute : jroute_list)
      route_list.emplace_back(routeRead(jroute));
}

iDataStandart* CreatePathStorageDataStandart(central_pack* pack, LPCSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new PathStorageDataStandart(pack, base_folder, pService);
}