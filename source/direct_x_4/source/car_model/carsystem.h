#pragma once

#include "utils/utils.h"

class CarLight;
class CarMove;
class Engine;
class ShaderUnit;

/**
@file     carsystem.h
@brief    Car system support class
@date     Created on 24/10/2014
@project  D3DBase
@author   Crush
*/

/*
\class db_carsystem
\brief - Диспетчерский класс для работы параметрами машины
*/
class CarSystem
{
public:
   CarSystem(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev, LPCWSTR modelFile, LPCWSTR waypointFile, DWORD stIndex);
   ~CarSystem();

   void LRelease();
   void LRecreate();

   void Update();

   static void DomRender(void *instance);
   void Render();

   void CarNeonRedirect();
   void CarHMLightRedirect(bool ctrl);
   void CarSignalRedirect(SGNTYPE type);
   void CarTransmissionRedirect(DMSTYPE type);
   void SwitchUserControlRedirect();
   WTYPE GetWPType();
    
   void StartWRotate();
   void CheckWRotate();
   void FinishWRotate();
   //void setTexturing(bool type){ dbc_texturing = type; }
   inline void ToggleTexturing(bool type) { m_texturing = type; }

   void MoveRedirect(float dx, float dy);
   void SetEmptyTexture(LPDIRECT3DTEXTURE9 tex){ m_emptyTexture = tex; }
private:
   void initService();
   void priorSwitch();
   void loadModel(LPCWSTR);
private:
   D3DXVECTOR3 m_pos;
   PRIORTYPE m_priority;
   LPCWSTR m_modelFile;
   LPDIRECT3DDEVICE9 m_d3dd;
   std::shared_ptr<CarLight>& m_light;
   std::shared_ptr<CarMove>& m_move;
   std::shared_ptr<Engine>& m_engine;
   std::shared_ptr<ShaderUnit>& m_shaderunit;

   DWORD m_cNumMaterials;
   LPD3DXMESH m_cMesh;
   D3DMATERIAL9* m_cMeshMaterials;
   LPDIRECT3DTEXTURE9* m_cMeshTextures, m_emptyTexture;
   bool m_texturing;
   LTTYPE m_lightStatus;

   float m_transformAngle;
   float m_tAPitch;
   float m_tALimit;
   bool m_transforming;
public:
   static const float DEFAULT_YSPIKE;
   static const bool DEFAULT_TEXTURING;
   static const float DEFAULT_ZDELTA;
};