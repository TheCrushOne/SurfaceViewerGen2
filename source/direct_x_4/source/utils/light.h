#pragma once

/**
@file     light.h
@brief    Light processing class
@date     Created on 30/10/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class Engine;

// Основной класс для работы со светом
class Light
{
public:
   Light(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev, DWORD count);
   ~Light();

   void LRelease() {}
   void LRecreate() {}

   void Update();
   void Render();
   void SetEnabled(LTYPE type, bool status);
   DWORD SetLight(D3DLIGHT9 *light, LTYPE type);
   //bool resetLight(DWORD index, D3DLIGHT9 *light, LTYPE type);
private:
   D3DLIGHT9** m_lightSource;
   LTYPE* m_lightType;
   bool m_lightEnabled[4];
   DWORD m_lightCount;
   LPDIRECT3DDEVICE9 m_d3dd;

   DWORD m_curLight;

   std::shared_ptr<Engine>& m_engine;
};