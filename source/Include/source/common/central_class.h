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
   static const file_utils::global_path_storage& GetPathStorage() { return Central::m_paths; }
   static file_utils::global_path_storage& GetPathStorageModify() { return Central::m_paths; }
   static const settings::application_settings& GetSettings() { return Central::m_settings; }
   static settings::application_settings& GetSettingsModify() { return Central::m_settings; }
private:
   static file_utils::global_path_storage m_paths;
   static settings::application_settings m_settings;
};