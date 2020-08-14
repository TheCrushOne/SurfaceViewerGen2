#include "stdafx.h"
#include "HeightMapConverter.h"
#include "common/utils.h"
#include "navdisp\ComService.h"
#include "navdisp\OrderCreation.h"

#include <stdlib.h>
#include <stdio.h>
#include <png.h>

using namespace converter;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      m_lock = true; \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return; \
   }// \
   //guard->Init(GetPack());

HeightMapConverter::HeightMapConverter(central_pack * pack, navigation_dispatcher::iComService * pService)
   : OrderBase(pack, pService)
{
   VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController);
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateJsonSettingsSerializer", m_settingsSerializer);
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateUnitDataSerializer", m_unitDataSerializer);
}

bool HeightMapConverter::processCommand()
{
   if (m_lock)
      return false;

   auto* src = m_pService->GetDataStandartFactory()->GetDataStandart(m_commandData.source.c_str());
   auto* dst = m_pService->GetDataStandartFactory()->GetDataStandart(m_commandData.destination.c_str());
   if (!readFromSource(reinterpret_cast<data_standart::iPngHeightMapDataStandart*>(src)))
      return false;

   if (!processData())
      return false;

   if (!writeToDestination(reinterpret_cast<data_standart::iSurfaceVieverGenMapDataStandart*>(dst)))
      return false;

   //auto ps = GetPathStorage();
   //auto stt = GetSettings();

   //m_settingsSerializer->Deserialize(SVGUtils::wstringToString(ps->pathfinder_settings_path).c_str(), stt->pth_stt);
   //m_settingsSerializer->Deserialize(SVGUtils::wstringToString(ps->research_settings_path).c_str(), stt->res_stt);
   //m_settingsSerializer->Deserialize(SVGUtils::wstringToString(ps->environment_settings_path).c_str(), stt->env_stt);
   //m_unitDataSerializer->Deserialize(SVGUtils::wstringToString(ps->unit_data_path).c_str(), m_unitData);

   // NOTE: share provider вызываетcя из базы
   //m_databaseController->Init(GetPack());
   //m_databaseController->SaveScenarioData(m_unitData, m_rawData);

   return true;
}

bool HeightMapConverter::readFromSource(data_standart::iPngHeightMapDataStandart* src)
{
   m_srcRawData = src->GetPngData();
   src->ReleasePngData();
   return true;
}

bool HeightMapConverter::processData()
{
   ATLASSERT(m_srcRawData.raw_data.size() == m_srcRawData.row_count);
   ATLASSERT(m_srcRawData.raw_data.size() > 0 ? m_srcRawData.raw_data.at(0).size() == m_srcRawData.col_count : 1);
   // NOTE: количество данных на данном этапе уже лишнее и является пережитками ридера png
   m_dstRawData = pathfinder::GeoMatrix(m_srcRawData.raw_data);
   // TODO: постобработка, если нужна
   return true;
}

bool HeightMapConverter::writeToDestination(data_standart::iSurfaceVieverGenMapDataStandart* dst)
{
   dst->SetData(m_dstRawData);
   return true;
}

navigation_dispatcher::iOrder* CreatePngHeightmapConverter(central_pack* pack, navigation_dispatcher::iComService* pService)
{
   return new HeightMapConverter(pack, pService);
}