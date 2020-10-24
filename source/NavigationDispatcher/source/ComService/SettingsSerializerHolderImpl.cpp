#include "stdafx.h"
#include "SettingsSerializerHolderImpl.h"

using namespace SV;
using namespace SV::navigation_dispatcher;

settings::application_settings& SettingsSerializerHolderImpl::GetSettings(const file_utils::global_path_storage& storage)
{
   if (m_loaded)
   {
      ATLASSERT(false);
      return m_settings;
   }
   load(storage);
   return m_settings;
}

#define STT_DESERIALIZE(gtype, stype, msg_part) \
   if (!storage.gtype.empty()) \
      m_settingsSerializer->Deserialize(SVGUtils::wstringToString(storage.gtype).c_str(), m_settings.stype); \
   else \
      GetPack()->comm->Message(ICommunicator::MessageType::MT_INFO, "type settings skipped to default");


void SettingsSerializerHolderImpl::load(const file_utils::global_path_storage& storage)
{
   STT_DESERIALIZE(environment_settings_path.path, env_stt, "Environment");
   STT_DESERIALIZE(map_settings_path.path, map_stt, "Map");
   STT_DESERIALIZE(pathfinder_settings_path.path, pth_stt, "Pathfinding");
   STT_DESERIALIZE(research_settings_path.path, res_stt, "Research");
   STT_DESERIALIZE(simulation_settings_path.path, sim_stt, "Simulation");
   m_loaded = true;
}

#undef STT_DESERIALIZE