#include "stdafx.h"
#include "ResearchExternal.h"
#include "navdisp\OrderCreation.h"

using namespace SV;
using namespace SV::engine;

std::condition_variable cv;
std::mutex cv_m;

ResearchExternal::ResearchExternal(central_pack* pack, navigation_dispatcher::iComService* pService)
   : OrderBase(pack, pService)
   , m_engine(std::make_shared<engine::Engine>(pack))
{}

bool ResearchExternal::processCommand()
{
   auto* src = GetService()->GetDataStandartFactory()->GetDataStandart(m_commandData.source.AsString());
   auto* dst = GetService()->GetDataStandartFactory()->GetDataStandart(m_commandData.destination.AsString());

   if (!needToProcess())
      return true;

   if (!readFromSource(reinterpret_cast<data_standart::iSurfaceVieverGenMapDataStandart*>(src)))
      return false;

   if (!processData())
      return false;

   // NOTE: локер, который стопает главный поток, пока все пути не будут рассчитаны
   // NOTE: анлок прокинут из processData 
   std::unique_lock<std::mutex> lk(cv_m);
   cv.wait(lk);

   if (!writeToDestination(reinterpret_cast<data_standart::iResearchResultDataStandart*>(dst)))
      return false;

   if (!recordOrderHashResult())
      return false;

   return true;
}

bool ResearchExternal::readFromSource(data_standart::iSurfaceVieverGenMapDataStandart* src)
{
   m_data = src->GetData();
   m_indata.unit_data = src->GetUnitData();
   m_settings = std::make_shared<settings::application_settings>(GetService()->GetSettingsSerializerHolder()->GetSettings());
   return true;
}

bool ResearchExternal::writeToDestination(data_standart::iResearchResultDataStandart* dst)
{
   //dst->SetData(m_engine->GetLastProcessedPaths(), m_engine->GetLandUnitExplication(), m_engine->GetAirUnitExplication(), m_engine->GetCoverageHistory());
   return true;
}

bool ResearchExternal::processData()
{
   m_engine->ProcessPathFind(m_indata, m_data, m_settings, [this]() { cv.notify_all(); });
   return true;
}

navigation_dispatcher::iOrder* CreateResearcher(central_pack* pack, navigation_dispatcher::iComService* pService)
{
   return new ResearchExternal(pack, pService);
}