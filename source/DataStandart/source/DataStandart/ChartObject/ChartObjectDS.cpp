#include "stdafx.h"
#include "ChartObjectDS.h"

using namespace data_standart;

void ChartObjectDataStandart::resolvePathDee()
{

}

void ChartObjectDataStandart::SaveData()
{

}

size_t ChartObjectDataStandart::GetDataHash()
{
   return 0;
}

iDataStandart* CreateChartObjectDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new ChartObjectDataStandart(pack, base_folder, pService);
}