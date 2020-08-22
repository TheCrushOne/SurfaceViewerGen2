#include "stdafx.h"
#include "OptimizedPathStorageDS.h"

using namespace data_standart;

void OptimizedPathStorageDataStandart::resolvePathDee()
{

}

size_t OptimizedPathStorageDataStandart::GetDataHash()
{
   return 0;
}

iDataStandart* CreateOptimizedPathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new OptimizedPathStorageDataStandart(pack, base_folder, pService);
}