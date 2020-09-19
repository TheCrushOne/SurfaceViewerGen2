#include "stdafx.h"
#include "OptimizedPathfinderExternal.h"
#include "common/simulation_structs.h"
#include "navdisp\OrderCreation.h"

using namespace engine;

std::condition_variable opt_cv;
std::mutex opt_cv_m;

OptPathfinderExternal::OptPathfinderExternal(central_pack* pack, navigation_dispatcher::iComService* pService)
   : OrderBase(pack, pService)
   , m_engine(std::make_shared<engine::Engine>(pack))
{

}

bool OptPathfinderExternal::processCommand()
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
   std::unique_lock<std::mutex> lk(opt_cv_m);
   opt_cv.wait(lk);

   if (!writeToDestination(reinterpret_cast<data_standart::iOptimizedPathStorageDataStandart*>(dst)))
      return false;

   if (!recordOrderHashResult())
      return false;

   return true;
}

bool OptPathfinderExternal::readFromSource(data_standart::iSurfaceVieverGenMapDataStandart* src)
{
   m_data = src->GetData();
   m_scenarioData.unit_data = src->GetUnitData();
   m_settings = std::make_shared<settings::application_settings>(GetService()->GetSettingsSerializerHolder()->GetSettings());
   return true;
}

bool OptPathfinderExternal::writeToDestination(data_standart::iOptimizedPathStorageDataStandart* dst)
{
   dst->SetData(m_engine->GetLastProcessedPaths());
   return true;
}

bool OptPathfinderExternal::processData()
{
   m_engine->ProcessPathFind(m_scenarioData, m_data, m_settings, [this]() { opt_cv.notify_all(); });
   return true;
}

navigation_dispatcher::iOrder* CreateOptimizedPathfinder(central_pack* pack, navigation_dispatcher::iComService* pService)
{
   return new OptPathfinderExternal(pack, pService);
}