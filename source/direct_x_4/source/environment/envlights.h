#pragma once

/**
@file     envlights.h
@brief    Static environment light support class
@date     Created on 08/11/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"
#include "engine/engine.h"

class EnvLight
{
public:
   EnvLight(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev);
   ~EnvLight();

   void LRelease() {}
   void LRecreate() {}

   void Update();
   void Render();

   void ToggleState(ELTYPE type);
   inline unsigned int GetBackgroundColor()
   {
      if (!m_dLightEnabled)
         return 0x00000000;
      if (m_state == CBE_MOON)
         return 0x0022222A;
      if (m_state == CBE_SPBDAY)
         return 0x00AAAAAA;
      return 0;
   }
private:
   void createLights();
   void createHouseLights();
private:
   LPDIRECT3DDEVICE9 m_d3dd;
   std::shared_ptr<Engine>& m_engine;
   std::shared_ptr<ShaderUnit>& m_shaderunit;
   std::shared_ptr<Light>& m_light;
   DWORD m_dLightIndexS;

   DLST m_state;

   D3DLIGHT9 m_dLight;
   DWORD m_dLightIndex;
   bool m_pLightEnabled;
   D3DLIGHT9* m_pLight;
   DWORD* m_shaderLightIndex;
   bool m_dLightEnabled;

   static const DWORD POINT_ENVLIGHT_NUM;

   static const D3DCOLORVALUE DEFAULT_DLIGHT_COLOR_MOON;
   static const D3DCOLORVALUE DEFAULT_DLIGHT_COLOR_SPBDAY;
};