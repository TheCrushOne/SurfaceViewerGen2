#pragma once

/**
@file     environment.h
@brief    Static environment support class
@date     Created on 08/11/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class Engine;
class ShaderUnit;
class EnvLight;

class Environment
{
   //подструктура для хранения данных об объектах типа "земля"
   struct tb_gPart
   {
      D3DXVECTOR3 m_pos;
      D3DXVECTOR3 m_realPos;
      PRIORTYPE m_priority;
   
      std::shared_ptr<Environment>& m_fnd;

      tb_gPart(D3DXVECTOR3 pos, std::shared_ptr<Environment>& fnd, PRIORTYPE prior);
      static void domRender(void *instance)
      {
         reinterpret_cast<tb_gPart*>(instance)->m_fnd->Render(instance, TBE_GROUND);
      }
   };

   //подструктура для хранения данных об объектах типа "дом"
   struct tb_hPart
   {
      D3DXVECTOR3 m_pos;
      D3DXVECTOR3 m_realPos;
      PRIORTYPE m_priority;

      float m_angle;
      std::shared_ptr<Environment>& m_fnd;
      tb_hPart(D3DXVECTOR3 pos, float angle, std::shared_ptr<Environment>& fnd);
      static void domRender(void *instance)
      {
         reinterpret_cast<tb_hPart*>(instance)->m_fnd->Render(instance, TBE_HOUSE);
      }
   };

    //подструктура для хранения данных об объектах типа "колонна"
   struct tb_cPart
   {
      D3DXVECTOR3 m_pos;
      D3DXVECTOR3 m_realPos;
      PRIORTYPE m_priority;

      std::shared_ptr<Environment>& m_fnd;
      tb_cPart(D3DXVECTOR3 pos, std::shared_ptr<Environment>& fnd);
      static void domRender(void *instance)
      {
         reinterpret_cast<tb_cPart*>(instance)->m_fnd->Render(instance, TBE_COLUMN);
      }
   };
public:
   Environment(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev);
   ~Environment();

   void LRelease();
   void LRecreate();

   void StartWRotate();
   void CheckWRotate();

   void Update();

   void Render(void *, TBE_OBJTYPE);

   void DLightRedirect();
   void PLightRedirect();

   inline void ToggleTexturing(bool type) { m_texturing = type; }
private:
   void updateGArrPrior();
   void updateHArrPrior();
   void updateCArrPrior();

   void updateWTReinterpret();

   void loadModels();
   void loadGround(LPCWSTR name);
   void loadHouse(LPCWSTR name);
   void loadColumn(LPCWSTR name);
   void createEmptyTexture();

   void mountGPartition();
   void mountHPartition();
   void mountCPartition();

   void drawSingleGround(D3DXVECTOR3);
   void drawSingleHouse(D3DXVECTOR3, float);
   void drawSingleColumn(D3DXVECTOR3);
private:
   std::shared_ptr<Environment> m_this;

   LPDIRECT3DDEVICE9 m_d3dd;
   std::shared_ptr<Engine>& m_engine;
   std::shared_ptr<ShaderUnit>& m_shaderunit;
   std::shared_ptr<EnvLight>& m_envlight;

   tb_gPart** m_groundArray;
   tb_hPart** m_houseArray;
   tb_cPart** m_columnArray;

   D3DXMATRIX m_wTReinterpret;

   DWORD m_hNumMaterials;
   LPD3DXMESH m_hMesh;
   D3DMATERIAL9* m_hMeshMaterials;
   LPDIRECT3DTEXTURE9* m_hMeshTextures, m_emptyTexture;

   DWORD m_gNumMaterials;
   LPD3DXMESH m_gMesh;
   D3DMATERIAL9* m_gMeshMaterials;
   LPDIRECT3DTEXTURE9* m_gMeshTextures;

   DWORD m_cNumMaterials;
   LPD3DXMESH m_cMesh;
   D3DMATERIAL9* m_cMeshMaterials;
   LPDIRECT3DTEXTURE9* m_cMeshTextures;

   DWORD m_xCount, m_zCount;
   float m_xStart, m_zStart;
   float m_xDelta, m_zDelta;

   DWORD m_hCount;
   DWORD m_cCount;
   DWORD m_gCount;       //не исп.

   bool m_texturing;

   float m_transformAngle;
   float m_tAPitch;
   float m_tALimit;
   bool m_transforming;

   static const LPCWSTR DEFAULT_GROUND_NAME;
   static const LPCWSTR DEFAULT_HOUSE_NAME;
   static const LPCWSTR DEFAULT_COLUMN_NAME;

   static const DWORD DEFAULT_HOUSE_COUNT;
   static const DWORD DEFAULT_COLUMN_COUNT;

   static const float DEFAULT_COLUMN_SCALING;

   static const DWORD DEFAULT_X_MESH_COUNT;
   static const DWORD DEFAULT_Z_MESH_COUNT;
   static const float DEFAULT_X_MESH_MIN_START;
   static const float DEFAULT_Z_MESH_MIN_START;
   static const float DEFAULT_X_MESH_DELTA;
   static const float DEFAULT_Z_MESH_DELTA;
   static const float DEFAULT_Y_SPIKE;
   static const float DEFAULT_COLUMN_SPIKE;
   static const bool DEFAULT_TEXTURING;

   static const float DEFAULT_ZDELTA;

   static const float DEFAULT_COLUMN_COORDINATES[][2];
};