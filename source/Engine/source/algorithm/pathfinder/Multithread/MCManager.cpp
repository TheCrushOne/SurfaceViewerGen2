#include "stdafx.h"
#include "MCManager.h"

using namespace pathfinder;

MultithreadComputingManager::MultithreadComputingManager()
{}

MultithreadComputingManager::~MultithreadComputingManager()
{}

void MultithreadComputingManager::SetTaskPacketFinishCallback(std::function<void(void)> callback)
{
   m_callback = callback;
   TaskHolder::SetTaskPacketFinishCallback(m_callback);
}

void MultithreadComputingManager::SetHolderCount(size_t count)
{
   m_holders.clear();
   m_holders.resize(count, std::pair<TaskHolder, TaskStatus>(TaskHolder(), TaskStatus::TS_IDLE));   // TODO: чекнуть, вызывается ли конструктор
   for (auto& holder : m_holders)
      holder.first.Init(GetPack());
}

void MultithreadComputingManager::LaunchTaskPacket(std::shared_ptr<TaskStorage> taskPacket)
{
   TaskHolder::SetTaskPacket(taskPacket);
   Finale();
   //GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "Init sync");
   TaskHolder::InitSynchronizer();
   TaskHolder::ForceInnerLock();
   for (auto& holder : m_holders)
      std::thread([&holder] { holder.first.Launch(); }).detach();
   TaskHolder::ForceInnerUnlock();
}

void MultithreadComputingManager::Finale()
{
   //GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "DeInit sync");
   TaskHolder::DeInitSynchronizer();
}