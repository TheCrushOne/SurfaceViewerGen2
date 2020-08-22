#include "stdafx.h"
#include "PathStorageDS.h"

using namespace data_standart;

void PathStorageDataStandart::resolvePathDee()
{

}

size_t PathStorageDataStandart::GetDataHash()
{
   return 1903;
}

iDataStandart* CreatePathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new PathStorageDataStandart(pack, base_folder, pService);
}