#pragma once

#include "common/communicator.h"
#include "common/file_storage.h"

class Central : public Communicable 
{
public:
   Central(ICommunicator* comm)
      : Communicable(comm)
   {}
   void Init(ICommunicator* comm)
   {
      SetCommunicator(comm);
   }
   const std::shared_ptr<file_utils::global_path_storage>& GetPathStorage() { checkPaths(); return m_paths; }
   std::shared_ptr<file_utils::global_path_storage>& GetPathStorageModify() { checkPaths(); return m_paths; }
   const std::shared_ptr<settings::application_settings>& GetSettings() { checkSettings();  return m_settings; }
   std::shared_ptr<settings::application_settings>& GetSettingsModify() { checkSettings();  return m_settings; }
protected:
   void checkPaths() { _ASSERT(m_paths); }
   void checkSettings() { _ASSERT(m_settings); }
private:
   std::shared_ptr<file_utils::global_path_storage> m_paths;
   std::shared_ptr<settings::application_settings> m_settings;
};