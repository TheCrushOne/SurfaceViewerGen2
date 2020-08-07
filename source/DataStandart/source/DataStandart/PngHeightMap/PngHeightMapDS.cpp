#include "stdafx.h"
#include "PngHeightMapDS.h"

#include <filesystem>

using namespace data_standart;

void PngHeightMapDataStandart::resolvePathDee()
{
   std::filesystem::path filePath(m_dataStandartData.file);
   if (filePath.is_relative())
      m_dataStandartData.file = (std::filesystem::path(m_baseFolder) / filePath).generic_wstring().c_str();

   std::string srcPngPath = SVGUtils::wstringToString(m_dataStandartData.file);
   fopen_s(&m_file, srcPngPath.c_str(), "rb");
   ATLASSERT(m_file);
}

void PngHeightMapDataStandart::ReleasePngData()
{
   if (m_file) 
      fclose(m_file);
   m_file = nullptr;
}

iDataStandart* CreatePngHeighMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new PngHeightMapDataStandart(pack, base_folder, pService);
}