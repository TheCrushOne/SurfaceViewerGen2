/**
@file     envlights.cpp
@brief    Static environment light support class
@date     Created on 08/11/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "envlights.h"
#include "utils\light.h"
#include "engine\shader_processing\shaderunit.h"

const DWORD EnvLight::POINT_ENVLIGHT_NUM = 4;

const D3DCOLORVALUE EnvLight::DEFAULT_DLIGHT_COLOR_MOON = { 0.6f, 0.7f, 1.1f, 1.0f };
const D3DCOLORVALUE EnvLight::DEFAULT_DLIGHT_COLOR_SPBDAY = { 1.7f, 1.7f, 1.7f, 1.0f };

EnvLight::EnvLight(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev)
   : m_d3dd(dev)
   , m_engine(eng)
   , m_shaderunit(eng->GetShader())
   , m_light(eng->GetLight())
{
   m_dLightEnabled = true;
   m_pLightEnabled = true;
   createLights();
}

EnvLight::~EnvLight()
{
   delete m_shaderLightIndex;
   delete m_pLight;
}

// Обновление данных(пустая, т.к. объекты статичны)
void EnvLight::Update()
{
   m_light->SetEnabled(OBL_MOONLIGHT, m_dLightEnabled);
   m_light->SetEnabled(OBL_LANTERNLIGHT, m_pLightEnabled);
}

// Отрисовка св. моделей окр. среды(на каждом кадре)
void EnvLight::Render()
{}

// Переключение тумблера направленного или точеного освещения
void EnvLight::ToggleState(ELTYPE type)
{
   if (type == CBE_DLIGHT)
   {
      m_state = static_cast<DLST>((static_cast<int>(m_state) + 1) % 3);
      if (m_state == CBE_SPBDAY)
      {
         m_dLightEnabled = true;
         m_dLight.Diffuse = DEFAULT_DLIGHT_COLOR_SPBDAY;
      }
      else if (m_state == CBE_MOON)
      {
         m_dLightEnabled = true;
         m_dLight.Diffuse = DEFAULT_DLIGHT_COLOR_MOON;
      }
      else if (m_state == CBE_OFF)
         m_dLightEnabled = false;
      else
         ;
      //cbe_dLightEnabled = !cbe_dLightEnabled;
      //cbe_light->resetLight(cbe_dLightIndexS, &cbe_dLight, OBL_MOONLIGHT);
      m_shaderunit->UpdateDLight(m_dLightIndex, m_dLightEnabled);
   }
   else
   {
      m_pLightEnabled = !m_pLightEnabled;
      for (DWORD i = 0; i < POINT_ENVLIGHT_NUM; i++)
         m_shaderunit->UpdatePLight(m_shaderLightIndex[i], m_pLightEnabled);
   }
}

// Создание освещения
void EnvLight::createLights()
{
   static bool created = false;
   if (created)
      return;
   ZeroMemory(&m_dLight, sizeof(D3DLIGHT9));
   m_dLight.Type = D3DLIGHT_DIRECTIONAL;//POINT SPOT
   m_dLight.Diffuse = DEFAULT_DLIGHT_COLOR_SPBDAY;
   //cbe_dLight.Diffuse.r = 0.6f;
   //cbe_dLight.Diffuse.g = 0.7f;
   //cbe_dLight.Diffuse.b = 1.1f;
   //cbe_dLight.Diffuse.a = 1.0f;
   m_dLight.Range = 1000.0f;

   D3DXVECTOR3 vecDir, vecPos;
   vecDir = D3DXVECTOR3(-0.4f, -0.4f, -0.2f);
   D3DXVec3Normalize((D3DXVECTOR3*)&m_dLight.Direction, &vecDir);

   m_dLightIndexS = m_light->SetLight(&m_dLight, OBL_MOONLIGHT);
   m_dLightIndex = m_shaderunit->SetDLight(&m_dLight);

   createHouseLights();        //point light

   created = true;
}

// Создание освещения
void EnvLight::createHouseLights()
{
   m_shaderLightIndex = new DWORD[MAX_LIGHT_COUNT];
   m_pLight = new D3DLIGHT9[MAX_LIGHT_COUNT];
   for (DWORD i = 0; i < MAX_LIGHT_COUNT; i++)
   {
      ZeroMemory(&m_pLight[i], sizeof(D3DLIGHT9));
      m_pLight[i].Type = D3DLIGHT_POINT;//POINT SPOT
      m_pLight[i].Diffuse.r = 0.0f;
      m_pLight[i].Diffuse.g = 1.0f;
      m_pLight[i].Diffuse.b = 0.0f;
      m_pLight[i].Diffuse.a = 1.0f;
      m_pLight[i].Ambient.r = m_pLight[i].Diffuse.r;
      m_pLight[i].Ambient.g = m_pLight[i].Diffuse.g;
      m_pLight[i].Ambient.b = m_pLight[i].Diffuse.b;
      m_pLight[i].Ambient.a = m_pLight[i].Diffuse.a;
      m_pLight[i].Attenuation0 = 0.0f;
      m_pLight[i].Attenuation1 = 0.125f;
      m_pLight[i].Attenuation2 = 0.1f;
      m_pLight[i].Range = 6.0f;
   }
   //D3DXVECTOR3 vecDir;
   //настройка вручную(ориентировочно на стены дома)
   m_pLight[0].Position = { 15.5f, 2.4f, 13.0f };
   m_pLight[1].Position = { 15.5f, 2.4f, 7.0f };
   m_pLight[2].Position = { -13.5f, 2.4f, 13.0f };
   m_pLight[3].Position = { -13.5f, 2.4f, 7.0f };
   //в шейдер идут все, а вот в обычную обработку столько не влезет...
   m_light->SetLight(&m_pLight[0], OBL_LANTERNLIGHT);
   m_shaderLightIndex[0] = m_shaderunit->SetPLight(&m_pLight[0]);
   //cbe_light->setLight(&cbe_pLight[1], OBL_LANTERNLIGHT);
   m_shaderLightIndex[1] = m_shaderunit->SetPLight(&m_pLight[1]);
   //cbe_light->setLight(&cbe_pLight[2], OBL_LANTERNLIGHT);
   m_shaderLightIndex[2] = m_shaderunit->SetPLight(&m_pLight[2]);
   //cbe_light->setLight(&cbe_pLight[3], OBL_LANTERNLIGHT);
   m_shaderLightIndex[3] = m_shaderunit->SetPLight(&m_pLight[3]);
}