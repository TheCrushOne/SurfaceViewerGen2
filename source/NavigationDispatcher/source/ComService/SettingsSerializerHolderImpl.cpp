#include "stdafx.h"
#include "SettingsSerializerHolderImpl.h"

using namespace navigation_dispatcher;

settings::application_settings& SettingsSerializerHolderImpl::GetSettings()
{
   if (!m_loaded)
      load();
   return m_settings;
}

void SettingsSerializerHolderImpl::load()
{
   m_loaded = true;
}