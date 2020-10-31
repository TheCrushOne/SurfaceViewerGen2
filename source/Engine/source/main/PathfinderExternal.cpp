#include "stdafx.h"
#include "PathfinderExternal.h"
#include "navdisp\OrderCreation.h"

using namespace SV;
using namespace SV::engine;

std::condition_variable cv;
std::mutex cv_m;

PathfinderExternal::PathfinderExternal(central_pack* pack, navigation_dispatcher::iComService* pService)
   : OrderBase(pack, pService)
   , m_engine(std::make_shared<engine::Engine>(pack))
{}

bool PathfinderExternal::processCommand()
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

   if (!writeToDestination(reinterpret_cast<data_standart::iPathStorageDataStandart*>(dst)))
      return false;

   if (!recordOrderHashResult())
      return false;

   return true;
}

bool PathfinderExternal::readFromSource(data_standart::iSurfaceVieverGenMapDataStandart* src)
{
   m_data = src->GetData();
   m_indata.unit_data = src->GetUnitData();
   m_settings = std::make_shared<settings::application_settings>(GetService()->GetSettingsSerializerHolder()->GetSettings());
   return true;
}

bool PathfinderExternal::writeToDestination(data_standart::iPathStorageDataStandart* dst)
{
   dst->SetData(m_engine->GetLastProcessedPaths());
   return true;
}

bool PathfinderExternal::processData()
{
   m_engine->ProcessPathFind(m_indata, m_data, m_settings, [this]() { cv.notify_all(); });
   return true;
}

navigation_dispatcher::iOrder* CreatePathfinder(central_pack* pack, navigation_dispatcher::iComService* pService)
{
   return new PathfinderExternal(pack, pService);
}