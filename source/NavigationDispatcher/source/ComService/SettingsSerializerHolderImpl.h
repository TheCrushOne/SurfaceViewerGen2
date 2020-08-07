#pragma once
#include "settings\SettingsSerializerHolder.h"
#include "common/central_class.h"
#include "settings/settings.h"
#include "navdisp/ComService.h"

namespace navigation_dispatcher
{
   class SettingsSerializerHolderImpl : public settings::iSettingsSerializerHolder, public Central
   {
   public:
      SettingsSerializerHolderImpl(central_pack* pack, iComService* pService)
         : Central(pack)
         , m_services(pService)
      {}
      settings::application_settings& GetSettings() override final;
   protected:
      void load();
   private:
      bool m_loaded = false;
      iComService* m_services;
      settings::application_settings m_settings;
   };
}