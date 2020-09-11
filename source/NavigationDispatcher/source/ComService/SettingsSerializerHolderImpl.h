#pragma once
#include "settings\SettingsSerializerHolder.h"
#include "common/central_class.h"
#include "settings/settings.h"
#include "navdisp/ComService.h"
#include "colreg/ModuleGuard.h"

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return; \
   }

namespace navigation_dispatcher
{
   class SettingsSerializerHolderImpl
      : public settings::iSettingsSerializerHolder
      , public Central
   {
   public:
      SettingsSerializerHolderImpl(central_pack* pack, iComService* pService)
         : Central(pack)
         , m_services(pService)
      {
         VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateJsonSettingsSerializer", m_settingsSerializer);
      }
      settings::application_settings& GetSettings() override final { return m_settings; }
      settings::application_settings& GetSettings(const file_utils::global_path_storage& filename) override final;
   protected:
      void load(const file_utils::global_path_storage& filename);
   private:
      bool m_loaded = false;
      iComService* m_services;
      settings::application_settings m_settings;
      colreg::ModuleGuard<colreg::iSettingsSerializer> m_settingsSerializer;
   };
}