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
public:
   Environment(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev);
   ~Environment();

   void LRelease();
   void LRecreate();

   void Update();

   void Render();

   void DLightRedirect();
   void PLightRedirect();

   inline void ToggleTexturing(bool type) { m_texturing = type; }

   void SetSurface(const double**);
private:
   void loadModels();
   void createEmptyTexture();
   void refillVertexBuf();
   void refillIndexBuf();
private:
   std::shared_ptr<Environment> m_this;

   LPDIRECT3DDEVICE9 m_d3dd;
   std::shared_ptr<Engine>& m_engine;
   std::shared_ptr<ShaderUnit>& m_shaderunit;
   std::shared_ptr<EnvLight>& m_envlight;

   D3DXMATRIX m_wTReinterpret;

   DWORD m_hNumMaterials;
   LPD3DXMESH m_hMesh;
   D3DMATERIAL9* m_hMeshMaterials;
   LPDIRECT3DTEXTURE9* m_hMeshTextures, m_emptyTexture;

   DWORD m_cNumMaterials;
   LPD3DXMESH m_cMesh;
   D3DMATERIAL9* m_cMeshMaterials;
   LPDIRECT3DTEXTURE9* m_cMeshTextures;

   DWORD m_xCount, m_zCount;
   float m_xStart, m_zStart;
   float m_xDelta, m_zDelta;

   bool m_texturing;

   float m_transformAngle;
   float m_tAPitch;
   float m_tALimit;
   bool m_transforming;

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





   unsigned int m_vFlags, m_indexCount, m_vertexCount, m_primCount;
   IDirect3DIndexBuffer9* m_indexBuf;
   IDirect3DVertexBuffer9* m_vertexBuf;

   vertex* m_vertexStorage;
   int* m_indexStorage;
   const double** m_data;
};