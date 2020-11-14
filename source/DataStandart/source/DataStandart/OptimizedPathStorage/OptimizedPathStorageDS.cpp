#include "stdafx.h"
#include "OptimizedPathStorageDS.h"

using namespace SV;
using namespace SV::data_standart;

void OptimizedPathStorageDataStandart::resolvePathDee()
{}

void OptimizedPathStorageDataStandart::SetData(const pathfinder::route_data& paths, const pathfinder::UnsignedMatrix& land, const pathfinder::UnsignedMatrix& air, const std::vector<pathfinder::SharedUnsignedMatrix>& coverages)
{}

const pathfinder::route_data& OptimizedPathStorageDataStandart::GetData() const
{
   return m_paths;
}

const pathfinder::UnsignedMatrix& OptimizedPathStorageDataStandart::GetLandUnitExplication() const
{
   return m_landExplication;
}

const pathfinder::UnsignedMatrix& OptimizedPathStorageDataStandart::GetAirUnitExplication() const
{
   return m_airExplication;
}

const std::vector<pathfinder::UnsignedMatrix>& OptimizedPathStorageDataStandart::GetCoverageHistory() const
{
   return m_coverageHistory;
}

iDataStandart* CreateOptimizedPathStorageDataStandart(central_pack* pack, LPCSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new OptimizedPathStorageDataStandart(pack, base_folder, pService);
}