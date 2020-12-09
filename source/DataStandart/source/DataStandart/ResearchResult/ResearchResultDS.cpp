#include "stdafx.h"
#include "ResearchResultDS.h"

using namespace SV;
using namespace SV::data_standart;

void ResearchResultDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.folder);
   // TODO: check!!!
   if (filePath.is_relative())
      m_dataStandartData.folder = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

iDataStandart* CreateResearchResultDataStandart(SV::central_pack* pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService* pService)
{
   return new ResearchResultDataStandart(pack, base_folder, pService);
}