/**
@file     shaderunit.cpp
@brief    Shaders processing class
@date     Created on 07/11/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "shaderunit.h"
#include "engine/engine.h"
#include "utils\camera.h"

const DWORD ShaderUnit::MAX_DIRECTIONAL_LIGHT_COUNT = 1;
const DWORD ShaderUnit::MAX_POINT_LIGHT_COUNT = 4;
const DWORD ShaderUnit::MAX_CAR_VISION_LIGHT_COUNT = 14;
const DWORD ShaderUnit::MAX_SPOT_LIGHT_COUNT = 14;
const DWORD ShaderUnit::MAX_KP_LIGHT_COUNT = 4;
const DWORD ShaderUnit::MAX_POSSIBLE_LIGHT_COUNT = 139;

const int ShaderUnit::LIGHT_OBJECT_SLICES = 32;
const int ShaderUnit::LIGHT_OBJECT_STACKS = 32;
const float ShaderUnit::LIGHT_OBJECT_LAUNCH_ANGLE = 45.0f;
const float ShaderUnit::LIGHT_OBJECT_RADIUS = 0.5f;
const float ShaderUnit::LIGHT_OBJECT_SPEED = 80.0f;
const float ShaderUnit::LIGHT_RADIUS_MAX = 8.5f;
const float ShaderUnit::LIGHT_RADIUS_MIN = 0.0f;
const LPD3DXEFFECT ShaderUnit::DEFAULT_CURRENT_EFFECT = nullptr;

//==============================//Конструктор(вынесен из .h после введения диспетчера)
ShaderUnit::ShaderUnit(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev)
   : m_engine(eng)
   , m_d3dd(dev)
   , m_currentEffect(DEFAULT_CURRENT_EFFECT)
   , m_ambEffect(nullptr)
   , m_bphEffect(nullptr)
{
   m_dLightSource = new mbsDirectionalLight[MAX_DIRECTIONAL_LIGHT_COUNT];
   m_dLightStatus = new bool[MAX_DIRECTIONAL_LIGHT_COUNT];
   m_dLightCount = 0;
   m_sLightSource = new mbsSpotLight[MAX_SPOT_LIGHT_COUNT];
   m_sLightStatus = new bool[MAX_SPOT_LIGHT_COUNT];
   m_sLightCount = 0;
   m_pLightSource = new mbsPointLight[MAX_POINT_LIGHT_COUNT];
   m_pLightStatus = new bool[MAX_POINT_LIGHT_COUNT];
   m_pLightCount = 0;
   m_cVLightSource = new mbsPointLight[MAX_CAR_VISION_LIGHT_COUNT];
   m_cVLightStatus = new bool[MAX_CAR_VISION_LIGHT_COUNT];
   m_cVLightCount = 0;
   m_kpLightSourceEng = new mbsKinematicPair[MAX_KP_LIGHT_COUNT];
   m_kpLightStatus = new bool[MAX_KP_LIGHT_COUNT];
   m_kpLightCount = 0;

   m_lightSourceCluster = new D3DLIGHT9*[MAX_POSSIBLE_LIGHT_COUNT];
   m_totalLightCount = 0;
   m_indexCorrelator = new DWORD[MAX_POSSIBLE_LIGHT_COUNT];
   m_lightSourceType = new SLSTYPE[MAX_POSSIBLE_LIGHT_COUNT];

#ifdef _DEBUG
   m_lightSphereDraw = true;
#else
   m_lightSphereDraw = false;
#endif
   initShaders();
   D3DXCreateSphere(m_d3dd, LIGHT_OBJECT_RADIUS,
      LIGHT_OBJECT_SLICES, LIGHT_OBJECT_STACKS, &m_pLightMesh, 0);

   createSpikeLight();
}

ShaderUnit::~ShaderUnit()
{
   delete m_dLightSource;
   delete m_dLightStatus;

   delete m_sLightSource;
   delete m_sLightStatus;

   delete m_pLightSource;
   delete m_pLightStatus;
	
   delete m_kpLightSourceEng;
   delete m_kpLightStatus;
    
   delete m_cVLightSource;
   delete m_cVLightStatus;
    
   delete m_lightSourceCluster;
   delete m_indexCorrelator;
   delete m_lightSourceType;

   SAFE_RELEASE(m_pLightMesh);

   SAFE_RELEASE(m_ambEffect);
   SAFE_RELEASE(m_bphEffect);
}

//
void ShaderUnit::LostDevice()
{
   m_bphEffect->OnLostDevice();
   m_ambEffect->OnLostDevice();
}

//
void ShaderUnit::ResetDevice()
{
   m_bphEffect->OnResetDevice();
   m_ambEffect->OnResetDevice();
}

// Освобождение объектов директикса(перед ресетом устройства)
void ShaderUnit::LRelease()
{
   SAFE_RELEASE(m_pLightMesh);

   SAFE_RELEASE(m_ambEffect);
   SAFE_RELEASE(m_bphEffect);
}

// Автовосстановление объектов директикса(после ресетом устройства)
void ShaderUnit::LRecreate()
{
   initShaders();
   D3DXCreateSphere(m_d3dd, LIGHT_OBJECT_RADIUS,
      LIGHT_OBJECT_SLICES, LIGHT_OBJECT_STACKS, &m_pLightMesh, 0);
}

// Обновление данных
void ShaderUnit::Update()
{
   if (!m_multiLightShaderAvaible)
      return;     //хотя сюда не должно заходить

   refreshLights();
   static const D3DXMATRIX identity(1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
   static D3DXMATRIX view, proj;
   static D3DXMATRIX rot, xRot, yRot;
   static float tmp_amb[4] = { 0.4f, 0.4f, 0.4f, 1.0f };

   proj = m_engine->GetCamera()->GetMProjection();
   view = m_engine->GetCamera()->GetMView();

   m_bphEffect->SetMatrix("worldMatrix", &identity);
   m_bphEffect->SetMatrix("worldInverseTransposeMatrix", &identity);
   m_bphEffect->SetMatrix("worldViewProjectionMatrix", &(identity*view*proj));

   m_bphEffect->SetFloat("material.shininess", 10.2f);   //spike
   // Set the camera position.

   m_bphEffect->SetValue("cameraPos", &(m_engine->GetCamera()->GetCurrentPosition()), sizeof(m_engine->GetCamera()->GetCurrentPosition()));

   //const static float ads_disabled[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
   const mbsKinematicPair *skpnLight = 0;
   const mbsPointLight *pLight = 0;
   const mbsSpotLight *sLight = 0;
   const mbsDirectionalLight *dLight = 0;
   static D3DXHANDLE hLight;
   static D3DXHANDLE hLightPos1, hLightPos2, hLightPos3;
   static D3DXHANDLE hLightAmbient;
   static D3DXHANDLE hLightDiffuse;
   static D3DXHANDLE hLightSpecular;
   static D3DXHANDLE hLightRadius;
   static D3DXHANDLE hLightRange;
   static D3DXHANDLE hLightDirection;
   static D3DXHANDLE hLightAngle;
   static D3DXHANDLE hLightFalloff;
   static D3DXHANDLE hLightAtten0;
   static D3DXHANDLE hLightAtten1;
   static D3DXHANDLE hLightAtten2;
   //static D3DXHANDLE hLightEnableFlag;   //не пригодился...

   for (DWORD i = 0; i < m_kpLightCount; i++)
   {
      skpnLight = &m_kpLightSourceEng[i];
      hLight = m_bphEffect->GetParameterElement("skpnLight", i);

      hLightPos1 = m_bphEffect->GetParameterByName(hLight, "pos1");
      hLightPos2 = m_bphEffect->GetParameterByName(hLight, "pos2");
      hLightPos3 = m_bphEffect->GetParameterByName(hLight, "pos3");
      hLightAmbient = m_bphEffect->GetParameterByName(hLight, "ambient");
      hLightDiffuse = m_bphEffect->GetParameterByName(hLight, "diffuse");
      hLightSpecular = m_bphEffect->GetParameterByName(hLight, "specular");
      hLightRadius = m_bphEffect->GetParameterByName(hLight, "radius");
      //hLightEnableFlag = mbs_bphEffect->GetParameterByName(hLight, "enabled");

      m_bphEffect->SetValue(hLightPos1, skpnLight->pos1, sizeof(skpnLight->pos1));
      m_bphEffect->SetValue(hLightPos2, skpnLight->pos2, sizeof(skpnLight->pos2));
      m_bphEffect->SetValue(hLightPos3, skpnLight->pos3, sizeof(skpnLight->pos3));

      m_bphEffect->SetValue(hLightAmbient, skpnLight->ambient, sizeof(skpnLight->ambient));
      m_bphEffect->SetValue(hLightDiffuse, skpnLight->diffuse, sizeof(skpnLight->diffuse));
      m_bphEffect->SetValue(hLightSpecular, skpnLight->specular, sizeof(skpnLight->specular));

      m_bphEffect->SetFloat(hLightRadius, skpnLight->radius);
      //mbs_bphEffect->SetBool(hLightEnableFlag, mbs_kpLightStatus[i]);
   }

   for (DWORD i = 0; i < m_pLightCount; i++)
   {
      pLight = &m_pLightSource[i];
      hLight = m_bphEffect->GetParameterElement("pLight", i);

      hLightPos1 = m_bphEffect->GetParameterByName(hLight, "pos");        
      hLightAmbient = m_bphEffect->GetParameterByName(hLight, "ambient");
      hLightDiffuse = m_bphEffect->GetParameterByName(hLight, "diffuse");
      hLightSpecular = m_bphEffect->GetParameterByName(hLight, "specular");
      hLightRadius = m_bphEffect->GetParameterByName(hLight, "radius");
      //hLightEnableFlag = mbs_bphEffect->GetParameterByName(hLight, "enabled");

      m_bphEffect->SetValue(hLightPos1, pLight->pos, sizeof(pLight->pos));
        
      m_bphEffect->SetValue(hLightAmbient, pLight->ambient, sizeof(pLight->ambient));
      m_bphEffect->SetValue(hLightDiffuse, pLight->diffuse, sizeof(pLight->diffuse));
      m_bphEffect->SetValue(hLightSpecular, pLight->specular, sizeof(pLight->specular));
       
      m_bphEffect->SetFloat(hLightRadius, pLight->radius);
      //mbs_bphEffect->SetBool(hLightEnableFlag, mbs_pLightStatus[i]);
   }

   for (DWORD i = 0; i < m_cVLightCount; i++)
   {
      pLight = &m_cVLightSource[i];
      hLight = m_bphEffect->GetParameterElement("cVLight", i);

      hLightPos1 = m_bphEffect->GetParameterByName(hLight, "pos");
      hLightAmbient = m_bphEffect->GetParameterByName(hLight, "ambient");
      hLightDiffuse = m_bphEffect->GetParameterByName(hLight, "diffuse");
      hLightSpecular = m_bphEffect->GetParameterByName(hLight, "specular");
      hLightRadius = m_bphEffect->GetParameterByName(hLight, "radius");
      //hLightEnableFlag = mbs_bphEffect->GetParameterByName(hLight, "enabled");

      m_bphEffect->SetValue(hLightPos1, pLight->pos, sizeof(pLight->pos));

      m_bphEffect->SetValue(hLightAmbient, pLight->ambient, sizeof(pLight->ambient));
      m_bphEffect->SetValue(hLightDiffuse, pLight->diffuse, sizeof(pLight->diffuse));
      m_bphEffect->SetValue(hLightSpecular, pLight->specular, sizeof(pLight->specular));

      m_bphEffect->SetFloat(hLightRadius, pLight->radius);
      //mbs_bphEffect->SetBool(hLightEnableFlag, mbs_pLightStatus[i]);
   }

   for (DWORD i = 0; i < m_sLightCount; i++)
   {
      sLight = &m_sLightSource[i];
      hLight = m_bphEffect->GetParameterElement("sLight", i);

      hLightPos1 = m_bphEffect->GetParameterByName(hLight, "pos");
      hLightAmbient = m_bphEffect->GetParameterByName(hLight, "ambient");
      hLightDiffuse = m_bphEffect->GetParameterByName(hLight, "diffuse");
      hLightDirection = m_bphEffect->GetParameterByName(hLight, "direction");
      hLightAngle = m_bphEffect->GetParameterByName(hLight, "angle");
      hLightFalloff = m_bphEffect->GetParameterByName(hLight, "falloff");

      //hLightAtten0 = m_bphEffect->GetParameterByName(hLight, "att0");
      hLightAtten1 = m_bphEffect->GetParameterByName(hLight, "att1");
      //hLightAtten2 = m_bphEffect->GetParameterByName(hLight, "att2");
      hLightRange = m_bphEffect->GetParameterByName(hLight, "range");

      //hLightEnableFlag = m_bphEffect->GetParameterByName(hLight, "enabled");

      m_bphEffect->SetValue(hLightPos1, sLight->pos, sizeof(sLight->pos));

      m_bphEffect->SetValue(hLightAmbient, sLight->ambient, sizeof(sLight->ambient));
      m_bphEffect->SetValue(hLightDiffuse, sLight->diffuse, sizeof(sLight->diffuse));

      m_bphEffect->SetValue(hLightDirection, sLight->direction, sizeof(sLight->direction));
      m_bphEffect->SetFloat(hLightAngle, sLight->coneAngle);
      m_bphEffect->SetFloat(hLightFalloff, sLight->falloff);

      //m_bphEffect->SetFloat(hLightAtten0, sLight->att0);
      m_bphEffect->SetFloat(hLightAtten1, sLight->att1);
      //m_bphEffect->SetFloat(hLightAtten2, sLight->att2);
      m_bphEffect->SetFloat(hLightRange, sLight->range);
      //m_bphEffect->SetBool(hLightEnableFlag, mbs_sLightStatus[i]);
   }

   for (DWORD i = 0; i < m_dLightCount; i++)
   {
      dLight = &m_dLightSource[i];
      hLight = m_bphEffect->GetParameterElement("dLight", i);

      hLightDirection = m_bphEffect->GetParameterByName(hLight, "direction");
      hLightAmbient = m_bphEffect->GetParameterByName(hLight, "ambient");
      hLightDiffuse = m_bphEffect->GetParameterByName(hLight, "diffuse");
      //hLightEnableFlag = mbs_bphEffect->GetParameterByName(hLight, "enabled");

      m_bphEffect->SetValue(hLightDirection, dLight->direction, sizeof(dLight->direction));

      m_bphEffect->SetValue(hLightAmbient, dLight->ambient, sizeof(dLight->ambient));
      m_bphEffect->SetValue(hLightDiffuse, dLight->diffuse, sizeof(dLight->diffuse));

      //mbs_bphEffect->SetBool(hLightEnableFlag, mbs_dLightStatus[i]);
   }
}

// Отрисовки сфер отладки(временно)
void ShaderUnit::Render()
{
   static UINT totalPasses;
   static D3DXMATRIX world;
   static D3DXMATRIX worldViewProjection;
   static float glight[4] = { 0, 0, 0, 0 };

   if ((!m_lightSphereDraw)
      || (!m_lightSphereShaderAvaible))
   return;

  //  for (DWORD i = 0; i < mbs_lightCount; i++)
  //  {

  //      setTechnique(MBS_AMBIENT, "AmbientLighting");

  //      D3DXMatrixTranslation(&world, mbs_curLight[i].pos[0], mbs_curLight[i].pos[1], mbs_curLight[i].pos[2]);
  //      worldViewProjection = world * mbs_engine->getCameraPtr()->getMView()*mbs_engine->getCameraPtr()->getMProjection();

  //      //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
  //      setShaderMatrix(MBS_AMBIENT, "worldViewProjectionMatrix", worldViewProjection);
  //      setFloat(MBS_AMBIENT, "ambientIntensity", 1.0f);
  //      
  //      glight[0] = mbs_curLight[i].ambient[0]; glight[1] = mbs_curLight[i].ambient[1]; glight[2] = mbs_curLight[i].ambient[2]; glight[3] = mbs_curLight[i].ambient[3];
  //      setValue(MBS_AMBIENT, "ambientColor", glight, sizeof(glight));
  //      //g_pAmbientEffect->SetValue("ambientColor", g_lights[i].ambient, sizeof(g_lights[i].ambient));

		////invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
  //      invokeShaderPassSeries(MBS_AMBIENT, totalPasses);
  //      for (UINT pass = 0; pass < totalPasses; ++pass)
  //      {
		//	//invokeShaderPass(MBS_DIRECTIONAL, pass);
  //          invokeShaderPass(MBS_AMBIENT, pass);
  //          mbs_pLightMesh->DrawSubset(0);
		//	//withdrawShaderPass(MBS_DIRECTIONAL);
  //          withdrawShaderPass(MBS_AMBIENT);
  //      }

		////withdrawShaderPassSeries(MBS_DIRECTIONAL);
  //      withdrawShaderPassSeries(MBS_AMBIENT);
  //  }

   SwapActiveSType(MBS_AMBIENT);
   for (DWORD i = 0; i < m_pLightCount; i++)
   {
      D3DXMatrixTranslation(&world, m_pLightSource[i].pos[0], m_pLightSource[i].pos[1], m_pLightSource[i].pos[2]);
      worldViewProjection = world * m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection();

      //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
      SetShaderMatrix("worldViewProjectionMatrix", worldViewProjection);
      SetFloat("ambientIntensity", 1.0f);

      glight[0] = m_pLightSource[i].diffuse[0]; glight[1] = m_pLightSource[i].diffuse[1]; glight[2] = m_pLightSource[i].diffuse[2]; glight[3] = m_pLightSource[i].diffuse[3];
      SetValue("ambientColor", glight, sizeof(glight));
      //g_pAmbientEffect->SetValue("ambientColor", g_lights[i].ambient, sizeof(g_lights[i].ambient));

      //invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
      InvokeShaderPassSeries(totalPasses);
      for (UINT pass = 0; pass < totalPasses; ++pass)
      {
         //invokeShaderPass(MBS_DIRECTIONAL, pass);
         InvokeShaderPass(pass);
         m_pLightMesh->DrawSubset(0);
         //withdrawShaderPass(MBS_DIRECTIONAL);
         WithdrawShaderPass();
      }

      //withdrawShaderPassSeries(MBS_DIRECTIONAL);
      WithdrawShaderPassSeries();
   }

   for (DWORD i = 0; i < m_cVLightCount; i++)
   {
      D3DXMatrixTranslation(&world, m_cVLightSource[i].pos[0], m_cVLightSource[i].pos[1], m_cVLightSource[i].pos[2]);
      worldViewProjection = world * m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection();

      //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
      SetShaderMatrix("worldViewProjectionMatrix", worldViewProjection);
      SetFloat("ambientIntensity", 1.0f);

      glight[0] = m_cVLightSource[i].diffuse[0]; glight[1] = m_cVLightSource[i].diffuse[1]; glight[2] = m_pLightSource[i].diffuse[2]; glight[3] = m_cVLightSource[i].diffuse[3];
      SetValue("ambientColor", glight, sizeof(glight));
      //g_pAmbientEffect->SetValue("ambientColor", g_lights[i].ambient, sizeof(g_lights[i].ambient));

      //invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
      InvokeShaderPassSeries(totalPasses);
      for (UINT pass = 0; pass < totalPasses; ++pass)
      {
         //invokeShaderPass(MBS_DIRECTIONAL, pass);
         InvokeShaderPass(pass);
         m_pLightMesh->DrawSubset(0);
         //withdrawShaderPass(MBS_DIRECTIONAL);
         WithdrawShaderPass();
      }

      //withdrawShaderPassSeries(MBS_DIRECTIONAL);
      WithdrawShaderPassSeries();
   }

   for (DWORD i = 0; i < m_sLightCount; i++)
   {
      D3DXMatrixTranslation(&world, m_sLightSource[i].pos[0], m_sLightSource[i].pos[1], m_sLightSource[i].pos[2]);
      worldViewProjection = world * m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection();

      //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
      SetShaderMatrix("worldViewProjectionMatrix", worldViewProjection);
      SetFloat("ambientIntensity", 1.0f);

      glight[0] = m_sLightSource[i].diffuse[0]; glight[1] = m_sLightSource[i].diffuse[1]; glight[2] = m_sLightSource[i].diffuse[2]; glight[3] = m_sLightSource[i].diffuse[3];
      SetValue("ambientColor", glight, sizeof(glight));
      //g_pAmbientEffect->SetValue("ambientColor", g_lights[i].ambient, sizeof(g_lights[i].ambient));

      //invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
      InvokeShaderPassSeries(totalPasses);
      for (UINT pass = 0; pass < totalPasses; ++pass)
      {
         //invokeShaderPass(MBS_DIRECTIONAL, pass);
         InvokeShaderPass(pass);
         m_pLightMesh->DrawSubset(0);
         //withdrawShaderPass(MBS_DIRECTIONAL);
         WithdrawShaderPass();
      }

      //withdrawShaderPassSeries(MBS_DIRECTIONAL);
      WithdrawShaderPassSeries();
   }
    
   for (DWORD i = 0; i < m_kpLightCount; i++)
   {
        
      D3DXMatrixTranslation(&world, m_kpLightSourceEng[i].pos1[0], m_kpLightSourceEng[i].pos1[1], m_kpLightSourceEng[i].pos1[2]);
      worldViewProjection = world * m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection();

      //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
      SetShaderMatrix("worldViewProjectionMatrix", worldViewProjection);
      SetFloat("ambientIntensity", 1.0f);

      glight[0] = m_kpLightSourceEng[i].ambient[0]; glight[1] = m_kpLightSourceEng[i].ambient[1]; glight[2] = m_kpLightSourceEng[i].ambient[2]; glight[3] = m_kpLightSourceEng[i].ambient[3];
      SetValue("ambientColor", glight, sizeof(glight));
      //g_pAmbientEffect->SetValue("ambientColor", g_lights[i].ambient, sizeof(g_lights[i].ambient));

      //invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
      InvokeShaderPassSeries(totalPasses);
      for (UINT pass = 0; pass < totalPasses; ++pass)
      {
         //invokeShaderPass(MBS_DIRECTIONAL, pass);
         InvokeShaderPass(pass);
         m_pLightMesh->DrawSubset(0);
         //withdrawShaderPass(MBS_DIRECTIONAL);
         WithdrawShaderPass();
      }

      //withdrawShaderPassSeries(MBS_DIRECTIONAL);
      WithdrawShaderPassSeries();
   }

   for (DWORD i = 0; i < m_kpLightCount; i++)
   {
      D3DXMatrixTranslation(&world, m_kpLightSourceEng[i].pos2[0], m_kpLightSourceEng[i].pos2[1], m_kpLightSourceEng[i].pos2[2]);
      worldViewProjection = world * m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection();

      //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
      SetShaderMatrix("worldViewProjectionMatrix", worldViewProjection);

      //invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
      InvokeShaderPassSeries(totalPasses);
      for (UINT pass = 0; pass < totalPasses; ++pass)
      {
         //invokeShaderPass(MBS_DIRECTIONAL, pass);
         InvokeShaderPass(pass);
         m_pLightMesh->DrawSubset(0);
         //withdrawShaderPass(MBS_DIRECTIONAL);
         WithdrawShaderPass();
      }

      //withdrawShaderPassSeries(MBS_DIRECTIONAL);
      WithdrawShaderPassSeries();
   }

   for (DWORD i = 0; i < m_kpLightCount; i++)
   {
      D3DXMatrixTranslation(&world, m_kpLightSourceEng[i].pos3[0], m_kpLightSourceEng[i].pos3[1], m_kpLightSourceEng[i].pos3[2]);
      worldViewProjection = world * m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection();

      //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
      SetShaderMatrix("worldViewProjectionMatrix", worldViewProjection);

      //invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
      InvokeShaderPassSeries(totalPasses);
      for (UINT pass = 0; pass < totalPasses; ++pass)
      {
         //invokeShaderPass(MBS_DIRECTIONAL, pass);
         InvokeShaderPass(pass);
         m_pLightMesh->DrawSubset(0);
         //withdrawShaderPass(MBS_DIRECTIONAL);
         WithdrawShaderPass();
      }

      //withdrawShaderPassSeries(MBS_DIRECTIONAL);
      WithdrawShaderPassSeries();
   }

   D3DXMatrixTranslation(&world, m_dbgPos.x, m_dbgPos.y, m_dbgPos.z);
   //D3DXMatrixTranslation(&world, pos.x, pos.y, pos.z);
   worldViewProjection = world * m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection();

   //setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", worldViewProjection);
   SetShaderMatrix("worldViewProjectionMatrix", worldViewProjection);

   //setValue("ambientColor", glight, sizeof(glight));
   //g_pAmbientEffect->SetValue("ambientColor", g_lights[i].ambient, sizeof(g_lights[i].ambient));

   //invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
   InvokeShaderPassSeries(totalPasses);
   for (UINT pass = 0; pass < totalPasses; ++pass)
   {
      //invokeShaderPass(MBS_DIRECTIONAL, pass);
      InvokeShaderPass(pass);
      m_pLightMesh->DrawSubset(0);
      //withdrawShaderPass(MBS_DIRECTIONAL);
      WithdrawShaderPass();
   }

   //withdrawShaderPassSeries(MBS_DIRECTIONAL);
   WithdrawShaderPassSeries();
}

// Переключение тумблера отладочных сфер
void ShaderUnit::ToggleLightSphereDraw()
{
   m_lightSphereDraw = !m_lightSphereDraw;
}

// Отрисовка отладочной сферы
void ShaderUnit::DrawDbgSphere(D3DXVECTOR3 pos)
{
   m_dbgPos = pos;
}

// Инициализация эффектов
void ShaderUnit::initShaders()
{
   /* mbe_vShader = 0;
   mbe_pShader = 0;
   DWORD dwShaderFlags = 0;
   LPD3DXBUFFER pCode = 0;
   #ifdef _DEBUG
   dwShaderFlags |= D3DXSHADER_DEBUG;
   #endif
   */
   DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER;
   /*
   // Assemble the vertex shader from the file
   D3DXCompileShaderFromFileW(L"VertexShader.hlsl", NULL, NULL, "main",
   "vs_2_0", dwShaderFlags, &pCode,
   NULL, &mbe_constTable);

   // Create the vertex shader
   mbe_d3dd->CreateVertexShader((DWORD*)pCode->GetBufferPointer(),
   &mbe_vShader);

   // Assemble the vertex shader from the file
   D3DXCompileShaderFromFileW(L"PixelShader.hlsl", NULL, NULL, "main",
   "ps_2_0", dwShaderFlags, &pCode,
   NULL, &mbe_constTable);

   // Create the vertex shader
   mbe_d3dd->CreatePixelShader((DWORD*)pCode->GetBufferPointer(),
   &mbe_pShader);
   */
    
   //D3DXCreateEffectFromFileA(mbe_d3dd, "Empty.fx", 0, 0, dwShaderFlags, 0, &mbe_effect, 0);
   //D3DXCreateEffectFromFileA(mbe_d3dd, "MaterialShader30.fx", 0, 0, 0, 0, &mbe_effect, 0);
   //D3DXCreateEffectFromFileA(mbs_d3dd, "shader.fx", 0, 0, dwShaderFlags, 0, &mbs_dirEffect, 0);
   D3DXCreateEffectFromFileA(m_d3dd, "shader/fx/ambient.fx", 0, 0, dwShaderFlags, 0, &m_ambEffect, 0);
   if (!m_ambEffect)
      throw uts::AMLSS_E(uts::SBC_SHADERLOADFAILURE, "shader/fx/ambient.fx");
   D3DXCreateEffectFromFileA(m_d3dd, /*"blinn_phong_sm20.fx"*/"shader/fx/stn.fx", 0, 0, dwShaderFlags, 0, &m_bphEffect, 0);
   if (!m_bphEffect)
      throw uts::AMLSS_E(uts::SBC_SHADERLOADFAILURE, "shader/fx/stn.fx");
   //пока что оставил...
   m_lightSphereShaderAvaible = m_ambEffect ? 1 : 0;
   m_multiLightShaderAvaible = m_bphEffect ? 1 : 0;
}

// Создание костыльного источника света(абс. черного)
void ShaderUnit::createSpikeLight()
{
   D3DCOLORVALUE diff = { 0.0f, 0.0f, 0.0f, 1.0f };
   ZeroMemory(&m_spikeLight, sizeof(D3DLIGHT9));
   m_spikeLight.Type = D3DLIGHT_SPOT;
   m_spikeLight.Diffuse = diff;
   m_spikeLight.Specular = diff;
   m_spikeLight.Ambient = diff;
   m_spikeLight.Attenuation0 = 0.0f;
   m_spikeLight.Attenuation1 = 0.0f;
   m_spikeLight.Attenuation2 = 0.0f;
   m_spikeLight.Falloff = 0.0f;
   m_spikeLight.Range = 0.0f;
   m_spikeLight.Phi = 0.0f;
   m_spikeLight.Theta = 0.0f;

   D3DXVECTOR3 vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
   D3DXVec3Normalize((D3DXVECTOR3*)&m_spikeLight.Direction, &vecDir);
   D3DXVec3Normalize((D3DXVECTOR3*)&m_spikeLight.Position, &vecDir);
}

// Установка света на позицию в массиве источников
DWORD ShaderUnit::SetDLight(D3DLIGHT9 *light)
{
   static float *tmp = 0;
   m_indexCorrelator[m_totalLightCount] = m_dLightCount;
   m_lightSourceType[m_totalLightCount] = MBS_DIRECTIONAL;
   m_lightSourceCluster[m_totalLightCount++] = light;
   m_dLightStatus[m_dLightCount] = true;    //лень обрабатывать сразу...
   tmp = m_dLightSource[m_dLightCount].ambient;
   tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   tmp = m_dLightSource[m_dLightCount].diffuse;
   tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   tmp = m_dLightSource[m_dLightCount].direction;
   tmp[0] = -light->Direction.x; tmp[1] = -light->Direction.y; tmp[2] = -light->Direction.z;       //костыль(там где-то знак перепутан был...)
   //tmp = mbs_dLightSource[mbs_dLightCount].specular;
   //tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   //tmp = mbs_dLightSource[mbs_dLightCount].pos;
   //tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   //mbs_lightSource[mbs_lightCount].radius = light->Range;
   //mbs_lightSource[mbs_lightCount].velocity = D3DXVECTOR3(0, 0, 0);
   return m_dLightCount++;
}

// Установка света на позицию в массиве источников
DWORD ShaderUnit::SetPLight(D3DLIGHT9 *light)
{
   static float *tmp = 0;
   m_indexCorrelator[m_totalLightCount] = m_pLightCount;
   m_lightSourceType[m_totalLightCount] = MBS_POINT;
   m_lightSourceCluster[m_totalLightCount++] = light;
   m_pLightStatus[m_pLightCount] = true;    //лень обрабатывать сразу...
   tmp = m_pLightSource[m_pLightCount].ambient;
   tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   tmp = m_pLightSource[m_pLightCount].diffuse;
   tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   tmp = m_pLightSource[m_pLightCount].specular;
   tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   tmp = m_pLightSource[m_pLightCount].pos;
   tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   m_pLightSource[m_pLightCount].radius = light->Range;
   //mbs_pLightSource[mbs_pLightCount].velocity = D3DXVECTOR3(0, 0, 0);
   return m_pLightCount++;
}

// Установка света на позицию в массиве источников
DWORD ShaderUnit::SetCVLight(D3DLIGHT9 *light)
{
   static float *tmp = 0;
   m_indexCorrelator[m_totalLightCount] = m_cVLightCount;
   m_lightSourceType[m_totalLightCount] = MBS_CAR_VISION;
   m_lightSourceCluster[m_totalLightCount++] = light;
   m_cVLightStatus[m_cVLightCount] = true;    //лень обрабатывать сразу...
   tmp = m_cVLightSource[m_cVLightCount].ambient;
   tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   tmp = m_cVLightSource[m_cVLightCount].diffuse;
   tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   tmp = m_cVLightSource[m_cVLightCount].specular;
   tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   tmp = m_cVLightSource[m_cVLightCount].pos;
   tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   m_cVLightSource[m_cVLightCount].radius = light->Range;
   //mbs_pLightSource[mbs_pLightCount].velocity = D3DXVECTOR3(0, 0, 0);
   return m_cVLightCount++;
}

// Установка света на позицию в массиве источников
DWORD ShaderUnit::SetSLight(D3DLIGHT9 *light)
{
   static float *tmp = 0;
   m_indexCorrelator[m_totalLightCount] = m_sLightCount;
   m_lightSourceType[m_totalLightCount] = MBS_SPOT;
   m_lightSourceCluster[m_totalLightCount++] = light;
   m_sLightStatus[m_sLightCount] = true;    //лень обрабатывать сразу...
   tmp = m_sLightSource[m_sLightCount].ambient;
   tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   tmp = m_sLightSource[m_sLightCount].diffuse;
   tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   tmp = m_sLightSource[m_sLightCount].specular;
   tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   tmp = m_sLightSource[m_sLightCount].pos;
   tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   tmp = m_sLightSource[m_sLightCount].direction;
   tmp[0] = light->Direction.x; tmp[1] = light->Direction.y; tmp[2] = light->Direction.z;
   m_sLightSource[m_sLightCount].falloff = light->Falloff;
   m_sLightSource[m_sLightCount].range = light->Range;
   m_sLightSource[m_sLightCount].coneAngle = light->Phi;   //не факт
   //mbs_sLightSource[mbs_sLightCount].att0 = light->Attenuation0;
   m_sLightSource[m_sLightCount].att1 = light->Attenuation1;
   //mbs_sLightSource[mbs_sLightCount].att2 = light->Attenuation2;
   //mbs_sLightSource[mbs_sLightCount].velocity = D3DXVECTOR3(0, 0, 0);
   return m_sLightCount++;
}

// Установка неонового источника света на позицию в массиве источников
DWORD ShaderUnit::SetKPLight(D3DLIGHT9 *ls1, D3DLIGHT9 *ls2, D3DLIGHT9 *ls3)
{
   static float *tmp = 0;
   m_indexCorrelator[m_totalLightCount] = m_kpLightCount;
   m_lightSourceType[m_totalLightCount] = MBS_KINEMATIC_PAIR_LS1;
   m_lightSourceCluster[m_totalLightCount++] = ls1;
   m_kpLightStatus[m_kpLightCount] = true;    //лень обрабатывать сразу...
   tmp = m_kpLightSourceEng[m_kpLightCount].ambient;
   tmp[0] = ls1->Ambient.r; tmp[1] = ls1->Ambient.g; tmp[2] = ls1->Ambient.b; tmp[3] = ls1->Ambient.a;
   tmp = m_kpLightSourceEng[m_kpLightCount].diffuse;
   tmp[0] = ls1->Diffuse.r; tmp[1] = ls1->Diffuse.g; tmp[2] = ls1->Diffuse.b; tmp[3] = ls1->Diffuse.a;
   tmp = m_kpLightSourceEng[m_kpLightCount].specular;
   tmp[0] = ls1->Specular.r; tmp[1] = ls1->Specular.g; tmp[2] = ls1->Specular.b; tmp[3] = ls1->Specular.a;
   tmp = m_kpLightSourceEng[m_kpLightCount].pos1;
   tmp[0] = ls1->Position.x; tmp[1] = ls1->Position.y; tmp[2] = ls1->Position.z;
   m_kpLightSourceEng[m_kpLightCount].radius = ls1->Range;
   //mbs_kpLightCount++;

   m_indexCorrelator[m_totalLightCount] = m_kpLightCount;
   m_lightSourceType[m_totalLightCount] = MBS_KINEMATIC_PAIR_LS2;
   m_lightSourceCluster[m_totalLightCount++] = ls2;
   //mbs_kpLightStatus[mbs_kpLightCount] = true;    //лень обрабатывать сразу...
   //tmp = mbs_kpLightSourceEng[mbs_kpLightCount].ambient;
   //tmp[0] = ls2->Ambient.r; tmp[1] = ls2->Ambient.g; tmp[2] = ls2->Ambient.b; tmp[3] = ls2->Ambient.a;
   //tmp = mbs_kpLightSourceEng[mbs_kpLightCount].diffuse;
   //tmp[0] = ls2->Diffuse.r; tmp[1] = ls2->Diffuse.g; tmp[2] = ls2->Diffuse.b; tmp[3] = ls2->Diffuse.a;
   //tmp = mbs_kpLightSourceEng[mbs_kpLightCount].specular;
   //tmp[0] = ls2->Specular.r; tmp[1] = ls2->Specular.g; tmp[2] = ls2->Specular.b; tmp[3] = ls2->Specular.a;
   tmp = m_kpLightSourceEng[m_kpLightCount].pos2;
   tmp[0] = ls2->Position.x; tmp[1] = ls2->Position.y; tmp[2] = ls2->Position.z;    
   //mbs_kpLightSourceEng[mbs_kpLightCount].radius = ls2->Range;
   //mbs_kpLightCount++;

   m_indexCorrelator[m_totalLightCount] = m_kpLightCount;
   m_lightSourceType[m_totalLightCount] = MBS_KINEMATIC_PAIR_LS3;
   m_lightSourceCluster[m_totalLightCount++] = ls3;
   //mbs_kpLightStatus[mbs_kpLightCount] = true;    //лень обрабатывать сразу...
   //tmp = mbs_kpLightSourceEng[mbs_kpLightCount].ambient;
   //tmp[0] = ls3->Ambient.r; tmp[1] = ls3->Ambient.g; tmp[2] = ls3->Ambient.b; tmp[3] = ls3->Ambient.a;
   //tmp = mbs_kpLightSourceEng[mbs_kpLightCount].diffuse;
   //tmp[0] = ls3->Diffuse.r; tmp[1] = ls3->Diffuse.g; tmp[2] = ls3->Diffuse.b; tmp[3] = ls3->Diffuse.a;
   //tmp = mbs_kpLightSourceEng[mbs_kpLightCount].specular;
   //tmp[0] = ls3->Specular.r; tmp[1] = ls3->Specular.g; tmp[2] = ls3->Specular.b; tmp[3] = ls3->Specular.a;
   tmp = m_kpLightSourceEng[m_kpLightCount].pos3;
   tmp[0] = ls3->Position.x; tmp[1] = ls3->Position.y; tmp[2] = ls3->Position.z;
   //mbs_kpLightSourceEng[mbs_kpLightCount].radius = ls3->Range;

   //mbs_lightSourceEng[mbs_lightCount].velocity = D3DXVECTOR3(0, 0, 0);
   return m_kpLightCount++;      //на возврат - индекс последнего
}


// Обновление информации о направленных источниках света(позиция и прочие параметры)
void ShaderUnit::UpdateDLight(DWORD index, bool status)
{
   //static float *tmp = 0;
   m_dLightStatus[index] = status;
   //tmp = mbs_dLightSource[index].ambient;
   //tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   //tmp = mbs_dLightSource[index].diffuse;
   //tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   //tmp = mbs_dLightSource[index].direction;
   //tmp[0] = -light->Direction.x; tmp[1] = -light->Direction.y; tmp[2] = -light->Direction.z;
   //tmp = mbs_dLightSource[mbs_dLightCount].specular;
   //tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   //tmp = mbs_dLightSource[mbs_dLightCount].pos;
   //tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   //mbs_lightSource[mbs_lightCount].radius = light->Range;
   //mbs_lightSource[mbs_lightCount].velocity = D3DXVECTOR3(0, 0, 0);
}


// Обновление информации о точечных источниках света(позиция и прочие параметры)
void ShaderUnit::UpdatePLight(DWORD index, bool status)
{
   //static float *tmp = 0;
   m_pLightStatus[index] = status;
   //tmp = mbs_pLightSource[index].ambient;
   //tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   //tmp = mbs_pLightSource[index].diffuse;
   //tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   //tmp = mbs_pLightSource[index].specular;
   //tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   //tmp = mbs_pLightSource[index].pos;
   //tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   //mbs_pLightSource[index].radius = light->Range;
   //mbs_pLightSource[mbs_pLightCount].velocity = D3DXVECTOR3(0, 0, 0);
}

// Обновление информации о точеных источниках света систем машины(позиция и прочие параметры)
void ShaderUnit::UpdateCVLight(DWORD index, bool status)
{
   //static float *tmp = 0;
   m_cVLightStatus[index] = status;
   //tmp = mbs_pLightSource[index].ambient;
   //tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   //tmp = mbs_pLightSource[index].diffuse;
   //tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   //tmp = mbs_pLightSource[index].specular;
   //tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   //tmp = mbs_pLightSource[index].pos;
   //tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   //mbs_pLightSource[index].radius = light->Range;
   //mbs_pLightSource[mbs_pLightCount].velocity = D3DXVECTOR3(0, 0, 0);
}

// Обновление информации о направленных источниках света(позиция и прочие параметры)
void ShaderUnit::UpdateSLight(DWORD index, bool status)
{
   //static float *tmp = 0;
   m_sLightStatus[index] = status;    //лень обрабатывать сразу...
   //tmp = mbs_sLightSource[index].ambient;
   //tmp[0] = light->Ambient.r; tmp[1] = light->Ambient.g; tmp[2] = light->Ambient.b; tmp[3] = light->Ambient.a;
   //tmp = mbs_sLightSource[index].diffuse;
   //tmp[0] = light->Diffuse.r; tmp[1] = light->Diffuse.g; tmp[2] = light->Diffuse.b; tmp[3] = light->Diffuse.a;
   //tmp = mbs_sLightSource[index].specular;
   //tmp[0] = light->Specular.r; tmp[1] = light->Specular.g; tmp[2] = light->Specular.b; tmp[3] = light->Specular.a;
   //tmp = mbs_sLightSource[index].pos;
   //tmp[0] = light->Position.x; tmp[1] = light->Position.y; tmp[2] = light->Position.z;
   //tmp = mbs_sLightSource[index].direction;
   //tmp[0] = light->Direction.x; tmp[1] = light->Direction.y; tmp[2] = light->Direction.z;
   //mbs_sLightSource[index].falloff = light->Falloff;
   //mbs_sLightSource[index].coneAngle = light->Theta;   //не факт
   //mbs_sLightSource[mbs_sLightCount].velocity = D3DXVECTOR3(0, 0, 0);
}

// Обновление информации неоновых источников света на позицию в массиве источников
void ShaderUnit::UpdateKPLight(DWORD index, bool status)
{
   //assert((index + 1) % 3 == 0);
   //static float *tmp = 0;
   m_kpLightStatus[index] = status;
   //mbs_kpLightStatus[index - 1] = status;
   //mbs_kpLightStatus[index - 2] = status;
   //tmp = mbs_kpLightSourceEng[index].ambient;
   //tmp[0] = ls1->Ambient.r; tmp[1] = ls1->Ambient.g; tmp[2] = ls1->Ambient.b; tmp[3] = ls1->Ambient.a;
   //tmp = mbs_kpLightSourceEng[index].diffuse;
   //tmp[0] = ls1->Diffuse.r; tmp[1] = ls1->Diffuse.g; tmp[2] = ls1->Diffuse.b; tmp[3] = ls1->Diffuse.a;
   //tmp = mbs_kpLightSourceEng[index].specular;
   //tmp[0] = ls1->Specular.r; tmp[1] = ls1->Specular.g; tmp[2] = ls1->Specular.b; tmp[3] = ls1->Specular.a;
   //tmp = mbs_kpLightSourceEng[index].pos1;
   //tmp[0] = ls1->Position.x; tmp[1] = ls1->Position.y; tmp[2] = ls1->Position.z;
   //tmp = mbs_kpLightSourceEng[index].pos2;
   //tmp[0] = ls2->Position.x; tmp[1] = ls2->Position.y; tmp[2] = ls2->Position.z;
   //tmp = mbs_kpLightSourceEng[index].pos3;
   //tmp[0] = ls3->Position.x; tmp[1] = ls3->Position.y; tmp[2] = ls3->Position.z;
   //mbs_kpLightSourceEng[index].radius = ls1->Range;
   //mbs_lightSourceEng[mbs_lightCount].velocity = D3DXVECTOR3(0, 0, 0);
   //return mbs_kpLightCount++;
}


// Каскадное автообновления источников света
void ShaderUnit::refreshLights()
{
   for (DWORD i = 0; i < m_totalLightCount; i++)
      (m_lightSourceType[i] == MBS_DIRECTIONAL) ? refreshDLight(i) : (m_lightSourceType[i] == MBS_POINT) ? refreshPLight(i) : (m_lightSourceType[i] == MBS_CAR_VISION) ? refreshCVLight(i) :(m_lightSourceType[i] == MBS_SPOT) ? refreshSLight(i) : refreshKPLight(i, m_lightSourceType[i]);
}

// Автообновление направленного источника света
void ShaderUnit::refreshDLight(DWORD ind)
{
   static D3DLIGHT9 *dmp = 0;
   static float *tmp = 0;
   if (m_dLightStatus[m_indexCorrelator[ind]])
      dmp = m_lightSourceCluster[ind];
   else
      dmp = &m_spikeLight;
   tmp = m_dLightSource[m_indexCorrelator[ind]].ambient;
   tmp[0] = dmp->Ambient.r, tmp[1] = dmp->Ambient.g, tmp[2] = dmp->Ambient.b, tmp[3] = dmp->Ambient.a;
   tmp = m_dLightSource[m_indexCorrelator[ind]].diffuse;
   tmp[0] = dmp->Diffuse.r, tmp[1] = dmp->Diffuse.g, tmp[2] = dmp->Diffuse.b, tmp[3] = dmp->Diffuse.a;
   tmp = m_dLightSource[m_indexCorrelator[ind]].direction;
   tmp[0] = -dmp->Direction.x, tmp[1] = -dmp->Direction.y, tmp[2] = -dmp->Direction.z;
}

// Автообновление точечного источника света
void ShaderUnit::refreshPLight(DWORD ind)
{
   static D3DLIGHT9 *dmp = 0;
   static float *tmp = 0;
   if (m_pLightStatus[m_indexCorrelator[ind]])
      dmp = m_lightSourceCluster[ind];
   else
      dmp = &m_spikeLight;
   tmp = m_pLightSource[m_indexCorrelator[ind]].ambient;
   tmp[0] = dmp->Ambient.r, tmp[1] = dmp->Ambient.g, tmp[2] = dmp->Ambient.b, tmp[3] = dmp->Ambient.a;
   tmp = m_pLightSource[m_indexCorrelator[ind]].diffuse;
   tmp[0] = dmp->Diffuse.r, tmp[1] = dmp->Diffuse.g, tmp[2] = dmp->Diffuse.b, tmp[3] = dmp->Diffuse.a;
   tmp = m_pLightSource[m_indexCorrelator[ind]].specular;
   tmp[0] = dmp->Specular.r, tmp[1] = dmp->Specular.g, tmp[2] = dmp->Specular.b, tmp[3] = dmp->Specular.a;
   tmp = m_pLightSource[m_indexCorrelator[ind]].pos;
   tmp[0] = dmp->Position.x, tmp[1] = dmp->Position.y, tmp[2] = dmp->Position.z;
   m_pLightSource[m_indexCorrelator[ind]].radius = dmp->Range;
}

// Автообновление точечного источника света
void ShaderUnit::refreshCVLight(DWORD ind)
{
   static D3DLIGHT9 *dmp = 0;
   static float *tmp = 0;
   if (m_cVLightStatus[m_indexCorrelator[ind]])
      dmp = m_lightSourceCluster[ind];
   else
      dmp = &m_spikeLight;
   tmp = m_cVLightSource[m_indexCorrelator[ind]].ambient;
   tmp[0] = dmp->Ambient.r, tmp[1] = dmp->Ambient.g, tmp[2] = dmp->Ambient.b, tmp[3] = dmp->Ambient.a;
   tmp = m_cVLightSource[m_indexCorrelator[ind]].diffuse;
   tmp[0] = dmp->Diffuse.r, tmp[1] = dmp->Diffuse.g, tmp[2] = dmp->Diffuse.b, tmp[3] = dmp->Diffuse.a;
   tmp = m_cVLightSource[m_indexCorrelator[ind]].specular;
   tmp[0] = dmp->Specular.r, tmp[1] = dmp->Specular.g, tmp[2] = dmp->Specular.b, tmp[3] = dmp->Specular.a;
   tmp = m_cVLightSource[m_indexCorrelator[ind]].pos;
   tmp[0] = dmp->Position.x, tmp[1] = dmp->Position.y, tmp[2] = dmp->Position.z;
   m_cVLightSource[m_indexCorrelator[ind]].radius = dmp->Range;
}

// Автообновление направленного источника света
void ShaderUnit::refreshSLight(DWORD ind)
{
   static D3DLIGHT9 *dmp = 0;
   static float *tmp = 0;
   if (m_sLightStatus[m_indexCorrelator[ind]])
      dmp = m_lightSourceCluster[ind];
   else
      dmp = &m_spikeLight;
   tmp = m_sLightSource[m_indexCorrelator[ind]].ambient;
   tmp[0] = dmp->Ambient.r, tmp[1] = dmp->Ambient.g, tmp[2] = dmp->Ambient.b, tmp[3] = dmp->Ambient.a;
   tmp = m_sLightSource[m_indexCorrelator[ind]].diffuse;
   tmp[0] = dmp->Diffuse.r, tmp[1] = dmp->Diffuse.g, tmp[2] = dmp->Diffuse.b, tmp[3] = dmp->Diffuse.a;
   tmp = m_sLightSource[m_indexCorrelator[ind]].specular;
   tmp[0] = dmp->Specular.r, tmp[1] = dmp->Specular.g, tmp[2] = dmp->Specular.b, tmp[3] = dmp->Specular.a;
   tmp = m_sLightSource[m_indexCorrelator[ind]].pos;
   tmp[0] = dmp->Position.x, tmp[1] = dmp->Position.y, tmp[2] = dmp->Position.z;
   tmp = m_sLightSource[m_indexCorrelator[ind]].direction;
   tmp[0] = dmp->Direction.x, tmp[1] = dmp->Direction.y, tmp[2] = dmp->Direction.z;
   m_sLightSource[m_indexCorrelator[ind]].falloff = dmp->Falloff;
   m_sLightSource[m_indexCorrelator[ind]].range = dmp->Range;
   m_sLightSource[m_indexCorrelator[ind]].coneAngle = dmp->Phi;
   m_sLightSource[m_indexCorrelator[ind]].att1 = dmp->Attenuation1;
}

// Автообновление источника света типа кинематической пары
void ShaderUnit::refreshKPLight(DWORD ind, SLSTYPE type)
{
   static D3DLIGHT9 *dmp = 0;
   static float *tmp = 0;
   if (m_kpLightStatus[m_indexCorrelator[ind]])
      dmp = m_lightSourceCluster[ind];
   else
      dmp = &m_spikeLight;

   //"мегакостыль"
   if (type == MBS_KINEMATIC_PAIR_LS1)
   {
      tmp = m_kpLightSourceEng[m_indexCorrelator[ind]].ambient;
      tmp[0] = dmp->Ambient.r, tmp[1] = dmp->Ambient.g, tmp[2] = dmp->Ambient.b, tmp[3] = dmp->Ambient.a;
      tmp = m_kpLightSourceEng[m_indexCorrelator[ind]].diffuse;
      tmp[0] = dmp->Diffuse.r, tmp[1] = dmp->Diffuse.g, tmp[2] = dmp->Diffuse.b, tmp[3] = dmp->Diffuse.a;
      tmp = m_kpLightSourceEng[m_indexCorrelator[ind]].specular;
      tmp[0] = dmp->Specular.r, tmp[1] = dmp->Specular.g, tmp[2] = dmp->Specular.b, tmp[3] = dmp->Specular.a;
      m_kpLightSourceEng[m_indexCorrelator[ind]].radius = dmp->Range;
      tmp = m_kpLightSourceEng[m_indexCorrelator[ind]].pos1;
   }
   else if (type == MBS_KINEMATIC_PAIR_LS2)
      tmp = m_kpLightSourceEng[m_indexCorrelator[ind]].pos2;
   else if (type == MBS_KINEMATIC_PAIR_LS3)
      tmp = m_kpLightSourceEng[m_indexCorrelator[ind]].pos3;
   else
      ;
   tmp[0] = dmp->Position.x, tmp[1] = dmp->Position.y, tmp[2] = dmp->Position.z;
}