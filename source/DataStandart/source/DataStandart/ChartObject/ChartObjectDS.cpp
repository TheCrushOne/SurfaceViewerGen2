#include "stdafx.h"
#include "ChartObjectDS.h"

#include <filesystem>

using namespace data_standart;

void ChartObjectDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

void ChartObjectDataStandart::SaveData()
{

}

iDataStandart* CreateChartObjectDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new ChartObjectDataStandart(pack, base_folder, pService);
}