/**
@file     light.cpp
@brief    Light processing class
@date     Created on 30/10/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "light.h"
#include "engine/engine.h"

Light::Light(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev, DWORD count)
   : m_engine(eng)
   , m_d3dd(dev)
   , m_lightCount(count)
{
   for (DWORD i = 0; i < 4; i++)
      m_lightEnabled[i] = false;
   m_curLight = 0;
   m_lightSource = new D3DLIGHT9*[m_lightCount];
   m_lightType = new LTYPE[m_lightCount];
}

Light::~Light()
{
   delete m_lightSource;
   delete m_lightType;
}

// Обновление(на каждом кадре)
void Light::Update()
{
   for (DWORD i = 0; i < m_lightCount; i++)
   {
      m_d3dd->SetLight(i, m_lightSource[i]);
      m_d3dd->LightEnable(i, m_lightEnabled[m_lightType[i]]);
   }
}

// Отрисовка освещения(пустая пока что)
void Light::Render()
{}

// Переключение тумблеров освещения
void Light::SetEnabled(LTYPE type, bool status)
{
   m_lightEnabled[type] = status;
}

// Установка света на позицию в массиве источников
DWORD Light::SetLight(D3DLIGHT9 *light, LTYPE type)
{
   m_lightSource[m_curLight] = light;
   m_lightType[m_curLight++] = type;
      return m_curLight;
}

// Переустановка света
//bool ob_light::resetLight(DWORD index, D3DLIGHT9 *light, LTYPE type)
//{
//    if (index > obl_curLight)
//        return false;
//    obl_lightSource[index] = light;
//    obl_lightType[index] = type;    //ну мало ли тип сменить надо ><
//    return true;
//}
//==============================