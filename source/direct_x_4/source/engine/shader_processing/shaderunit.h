#pragma once

/**
@file     shaderunit.h
@brief    Shaders processing class
@date     Created on 07/11/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class Engine;

struct mbsDirectionalLight
{
    float direction[3];
    float ambient[4];
    float diffuse[4];
};

struct mbsSpotLight
{
    float pos[3];
    float direction[3];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float falloff; 
    float range;
    float coneAngle;        //пока что один угол
    //float att0;
    float att1;
    //float att2;
};

struct mbsPointLight
{
    float pos[3];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float radius;
    //D3DXVECTOR3 velocity;

    //void init();
    //void update(float elapsedTimeSec);
};

struct mbsKinematicPair
{
    float pos1[3];
    float pos2[3];
    float pos3[3];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float radius;
};


#define SHADER_TYPE_DYNAMIC_RETVAL_CAST (type == MBS_AMBIENT ? m_ambEffect : (type == MBS_BLINNPHONG ? m_bphEffect : m_dirEffect))
//юнит гвард можно убрать..
#define UNIT_GUARD static const 


// Специализированный класс для работы с шейдерами
class ShaderUnit
{
public:
   ShaderUnit(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev);
   ~ShaderUnit();

   void LostDevice();
   void ResetDevice();

   void LRelease();
   void LRecreate();

   void Update();
   void Render();

   DWORD SetDLight(D3DLIGHT9 *light);
   DWORD SetPLight(D3DLIGHT9 *light);
   DWORD SetCVLight(D3DLIGHT9 *light);
   DWORD SetSLight(D3DLIGHT9 *light);
   DWORD SetKPLight(D3DLIGHT9 *ls1, D3DLIGHT9 *ls2, D3DLIGHT9 *ls3);

   void UpdateDLight(DWORD index, bool status);
   void UpdatePLight(DWORD index, bool status);
   void UpdateCVLight(DWORD index, bool status);
   void UpdateSLight(DWORD index, bool status);
   void UpdateKPLight(DWORD index, bool status);

   void ToggleLightSphereDraw();

   void SwapActiveSType(STYPE type){ m_currentEffect = SHADER_TYPE_DYNAMIC_RETVAL_CAST; }

   void DrawDbgSphere(D3DXVECTOR3 pos);

   inline bool CheckMultiLightShader() { return m_multiLightShaderAvaible; }
   inline bool CheckLightSphereShader() { return m_lightSphereShaderAvaible; }
   inline void InvokeShaderPassSeries(UINT &total) { m_currentEffect->Begin(&total, 0); }
   inline void InvokeShaderPass(UINT pass) { m_currentEffect->BeginPass(pass); }
   inline void WithdrawShaderPass() { m_currentEffect->EndPass(); }
   inline void WithdrawShaderPassSeries() { m_currentEffect->End(); }
   inline void SetShaderMatrix(LPCSTR name, D3DXMATRIX matrix) { m_currentEffect->SetMatrix(name, &(matrix)); }
   inline void ShaderCommitChanges() { m_currentEffect->CommitChanges(); }
   inline void SetTechnique(LPCSTR name) { m_currentEffect->SetTechnique(m_currentEffect->GetTechniqueByName(name)); }
   inline void SetValue(LPCSTR name, LPCVOID data, UINT size) { m_currentEffect->SetValue(name, data, size); }
   inline void SetFloat(LPCSTR name, float data) { m_currentEffect->SetFloat(name, data); }
   inline void SetInt(LPCSTR name, int data) { m_currentEffect->SetInt(name, data); }
   inline void SetTexture(LPCSTR name, LPDIRECT3DTEXTURE9 tex) { m_currentEffect->SetTexture(name, tex); }
private:
   void initShaders();
   void refreshLights();
   void createSpikeLight();

   void refreshDLight(DWORD ind);
   void refreshPLight(DWORD ind);
   void refreshCVLight(DWORD ind);
   void refreshSLight(DWORD ind);
   void refreshKPLight(DWORD ind, SLSTYPE type);
private:
   LPDIRECT3DDEVICE9 m_d3dd;

   ID3DXEffect* m_ambEffect, * m_bphEffect, * m_dirEffect;
   //LPDIRECT3DVERTEXSHADER9 mbe_vShader;
   //LPDIRECT3DPIXELSHADER9 mbe_pShader;
   //LPD3DXCONSTANTTABLE mbe_constTable;
   D3DLIGHT9** m_lightSourceCluster;
   D3DLIGHT9 m_spikeLight;
   DWORD m_totalLightCount;
   DWORD* m_indexCorrelator;
   SLSTYPE* m_lightSourceType;
   LPD3DXEFFECT m_currentEffect;

   mbsDirectionalLight* m_dLightSource;
   bool* m_dLightStatus;
   DWORD m_dLightCount;
   mbsSpotLight* m_sLightSource;
   bool* m_sLightStatus;
   DWORD m_sLightCount;
   mbsPointLight* m_pLightSource;
   bool* m_pLightStatus;
   DWORD m_pLightCount;
   mbsPointLight* m_cVLightSource;
   bool* m_cVLightStatus;
   DWORD m_cVLightCount;
   mbsKinematicPair* m_kpLightSourceEng;
   bool* m_kpLightStatus;
   DWORD m_kpLightCount;
   LPD3DXMESH m_pLightMesh;
   bool m_multiLightShaderAvaible;

   bool m_lightSphereDraw;
   bool m_lightSphereShaderAvaible;

   D3DXVECTOR3 m_dbgPos;

   std::shared_ptr<Engine>& m_engine;

   UNIT_GUARD DWORD MAX_DIRECTIONAL_LIGHT_COUNT;
   UNIT_GUARD DWORD MAX_POINT_LIGHT_COUNT;
   UNIT_GUARD DWORD MAX_SPOT_LIGHT_COUNT;
   UNIT_GUARD DWORD MAX_CAR_VISION_LIGHT_COUNT;
   UNIT_GUARD DWORD MAX_KP_LIGHT_COUNT;
   UNIT_GUARD DWORD MAX_POSSIBLE_LIGHT_COUNT;

   static const int LIGHT_OBJECT_SLICES;
   static const int LIGHT_OBJECT_STACKS;
   static const float LIGHT_OBJECT_LAUNCH_ANGLE;
   static const float LIGHT_OBJECT_RADIUS;
   static const float LIGHT_OBJECT_SPEED;
   static const float LIGHT_RADIUS_MAX;
   static const float LIGHT_RADIUS_MIN;
   static const LPD3DXEFFECT DEFAULT_CURRENT_EFFECT;
};