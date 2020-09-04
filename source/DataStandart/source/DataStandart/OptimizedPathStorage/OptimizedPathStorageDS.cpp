#include "stdafx.h"
#include "OptimizedPathStorageDS.h"

using namespace data_standart;

void OptimizedPathStorageDataStandart::resolvePathDee()
{

}

size_t OptimizedPathStorageDataStandart::GetDataHash()
{
   return 1902;
}

void OptimizedPathStorageDataStandart::SetData(const pathfinder::route_data& paths)
{

}

const pathfinder::route_data& OptimizedPathStorageDataStandart::GetData()
{
   return m_paths;
}

iDataStandart* CreateOptimizedPathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new OptimizedPathStorageDataStandart(pack, base_folder, pService);
}