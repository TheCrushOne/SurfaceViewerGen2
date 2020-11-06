#include "stdafx.h"
#include "OptimizedPathStorageDS.h"

using namespace SV;
using namespace SV::data_standart;

void OptimizedPathStorageDataStandart::resolvePathDee()
{}

void OptimizedPathStorageDataStandart::SetData(const pathfinder::route_data& paths)
{}

const pathfinder::route_data& OptimizedPathStorageDataStandart::GetData()
{
   return m_paths;
}

iDataStandart* CreateOptimizedPathStorageDataStandart(central_pack* pack, LPCSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new OptimizedPathStorageDataStandart(pack, base_folder, pService);
}