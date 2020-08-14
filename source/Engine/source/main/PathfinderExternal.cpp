#include "stdafx.h"
#include "PathfinderExternal.h"
#include "common/simulation_structs.h"

using namespace engine;

std::condition_variable cv;
std::mutex cv_m;

PathfinderExternal::PathfinderExternal(central_pack* pack, navigation_dispatcher::iComService* pService)
   : OrderBase(pack, pService)
   , m_engine(std::make_shared<engine::Engine>(pack))
{

}

bool PathfinderExternal::processCommand()
{
   auto* src = m_pService->GetDataStandartFactory()->GetDataStandart(m_commandData.source.c_str());
   //auto* dst = m_pService->GetDataStandartFactory()->GetDataStandart(m_commandData.destination.c_str());
   if (!readFromSource(reinterpret_cast<data_standart::iSurfaceVieverGenMapDataStandart*>(src)))
      return false;

   if (!processData())
      return false;

   // NOTE: локер, который стопает главный поток, пока все пути не будут рассчитаны
   std::unique_lock<std::mutex> lk(cv_m);
   cv.wait(lk);

   return true;
}

bool PathfinderExternal::readFromSource(data_standart::iSurfaceVieverGenMapDataStandart* src)
{
   m_data = src->GetData();
   return true;
}

bool PathfinderExternal::writeToDestination(data_standart::iPathStorageDataStandart* dst)
{
   return true;
}

bool PathfinderExternal::processData()
{
   m_engine->ProcessPathFind(m_scenarioData, m_data, [this]() { cv.notify_all(); });
   return true;
}

navigation_dispatcher::iOrder* CreatePathfinder(central_pack* pack, navigation_dispatcher::iComService* pService)
{
   return new PathfinderExternal(pack, pService);
}