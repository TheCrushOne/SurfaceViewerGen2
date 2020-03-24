#pragma once

#include "common/communicator.h"
#include "common/file_storage.h"

struct central_pack
{
   central_pack(ICommunicator* comm, file_utils::global_path_storage paths, settings::application_settings stt)
      : comm(comm)
      , paths(std::make_shared<file_utils::global_path_storage>(paths))
      , settings(std::make_shared<settings::application_settings>(stt))
   {}
   std::shared_ptr<ICommunicator> comm = nullptr;
   std::shared_ptr<file_utils::global_path_storage> paths = nullptr;
   std::shared_ptr<settings::application_settings> settings = nullptr;
};

class Central : public Communicable 
{
public:
   void Init(central_pack* pack) { m_pack = pack; }

   const std::shared_ptr<file_utils::global_path_storage>& GetPathStorage() { checkPack(); return m_pack->paths; }
   std::shared_ptr<file_utils::global_path_storage>& GetPathStorageModify() { checkPack(); return m_pack->paths; }
   const std::shared_ptr<settings::application_settings>& GetSettings() { checkPack();  return m_pack->settings; }
   std::shared_ptr<settings::application_settings>& GetSettingsModify() { checkPack();  return m_pack->settings; }

   void SetCommunicator(ICommunicator* comm) override final { checkPack(); m_pack->comm.reset(comm); }
   ICommunicator* GetCommunicator() override final { checkPack(); return m_pack->comm.get(); }

   central_pack* GetPack() const { return m_pack; };
protected:
   void checkPack() { _ASSERT(m_pack); }
private:
   central_pack* m_pack;
};