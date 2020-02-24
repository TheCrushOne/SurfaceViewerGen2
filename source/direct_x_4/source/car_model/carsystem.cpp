/**
@file     carsystem.cpp
@brief    Car model processing class
@date     Created on 24/10/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "carsystem.h"
#include "engine/engine.h"
#include "car_model\carlight.h"
#include "car_model/carmove.h"
#include "engine/shader_processing/shaderunit.h"
#include "engine\subsystems\ordermanager.h"
#include "utils\camera.h"

const float CarSystem::DEFAULT_YSPIKE = 0.72f;
const bool CarSystem::DEFAULT_TEXTURING = true;
const float CarSystem::DEFAULT_ZDELTA = 10.0f;

CarSystem::CarSystem(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev, LPCWSTR modelFile, LPCWSTR waypointFile, DWORD stIndex)
   : m_engine(eng)
   , m_light(eng->GetCarlight())
   , m_move(eng->GetCarmove())
   , m_shaderunit(eng->GetShader())
   , m_transforming(false)
{
   //m_engine->GetOrderManager()->RegisterObject(&CarSystem::DomRender, reinterpret_cast<void*>(this), &m_pos, &m_priority);
   m_d3dd = dev;
   m_texturing = DEFAULT_TEXTURING;
   initService();      //хотя он пустой...
   loadModel(modelFile); 
   m_modelFile = modelFile;
   m_lightStatus = DBC_NONE;

   m_transformAngle = m_engine->GetStartTransformAngle();
}


CarSystem::~CarSystem()
{
   LRelease();
}

// Освобождение объектов директикса(перед ресетом устройства)
void CarSystem::LRelease()
{
   for (DWORD i = 0; i < m_cNumMaterials; i++)
      SAFE_RELEASE(m_cMeshTextures[i]);
   SAFE_RELEASE(m_cMesh);
   SAFE_RELEASE(m_emptyTexture);
   delete m_cMeshMaterials;
   delete m_cMeshTextures;
}

// Автовосстановление объектов директикса(после ресетом устройства)
void CarSystem::LRecreate()
{
   loadModel(m_modelFile);
}

// Обновление данных машины(в.т.ч. свет и путь)
void CarSystem::Update()
{
   priorSwitch();
   CheckWRotate();
   m_pos = m_move->GetCoordinates();
   m_light->Update(m_move->GetCoordinatesWOMove(), m_move->GetAngleYaw());
}

// Отрисовка модели машины(на каждом кадре)
void CarSystem::DomRender(void *instance)
{
   reinterpret_cast<CarSystem*>(instance)->Render();
}

// Отрисовка модели машины(на каждом кадре)
void CarSystem::Render()
{
   static UINT totalPasses;
    
   static float c_mtpr[4] = {0, 0, 0, 0};
   static D3DXMATRIX c_mvMatrix, c_rtMatrix, c_mgMatrix, c_rxMatrix;   //статик, чтоб не создавать каждый раз с нуля
   static D3DXVECTOR3 c_pos = D3DXVECTOR3(0, 0, 0);
   static float c_angleYaw = 0.0f;

   c_pos = m_pos;
   //dbc_pos = dbc_move->getCoordinatesWOMove();
   c_angleYaw = m_move->GetAngleYaw();

   D3DXMatrixRotationX(&c_rtMatrix, m_transformAngle);
   D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, DEFAULT_ZDELTA);

   //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
   D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
   D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, -DEFAULT_ZDELTA);
   D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &c_mgMatrix);


   D3DXMatrixTranslation(&c_mvMatrix, c_pos.x, c_pos.y, c_pos.z);        //тут был подгон на 2.2 по Z
   D3DXMatrixRotationY(&c_rtMatrix, c_angleYaw); 
   //шайтан
   D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &c_mgMatrix);
   D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mgMatrix);

   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);

   static D3DXCOLOR mDiff = D3DXCOLOR(0, 0, 0, 0);
   static D3DXVECTOR3 vLightDir[3] = {D3DXVECTOR3(1, 0, 0),
      D3DXVECTOR3(1, 1, 0), 
      D3DXVECTOR3(0, 0, 1) };
   static D3DXVECTOR4 vLightDiffuse[3] = { D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
      D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f),
      D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f) };
   //light recount был здесь изначально
   //car
   if (m_engine->CheckShaderUsage())
   {
      //dbc_shaderunit->setValue(MBS_DIRECTIONAL, "g_LightDir", vLightDir, sizeof(D3DXVECTOR3)* 3);
      //dbc_shaderunit->setValue(MBS_DIRECTIONAL, "g_LightDiffuse", vLightDiffuse, sizeof(D3DXVECTOR4)* 3);

      // Update the effect's variables.  Instead of using strings, it would 
      // be more efficient to cache a handle to the parameter by calling 
      // ID3DXEffect::GetParameterByName

      //D3DXCOLOR vWhite = D3DXCOLOR(1, 1, 1, 1);
      m_shaderunit->SwapActiveSType(MBS_BLINNPHONG);
      //dbc_shaderunit->setFloat(MBS_DIRECTIONAL, "g_fTime", (float)0);
      //dbc_shaderunit->setInt(MBS_DIRECTIONAL, "g_nNumLights", 3);
      //dbc_shaderunit->setTechnique(MBS_DIRECTIONAL, "RenderSceneWithTexture3Light");
      m_shaderunit->SetTechnique("ALP_MULTICAST");

      for (DWORD i = 0; i < m_cNumMaterials; i++)
      {
         //mDiff = dbc_cMeshMaterials[i].Diffuse;
         //dbc_shaderunit->setValue(MBS_DIRECTIONAL, "g_MaterialDiffuseColor", &mDiff, sizeof(D3DXCOLOR));
         m_shaderunit->SetShaderMatrix("worldMatrix", c_mgMatrix);
         m_shaderunit->SetShaderMatrix("worldViewProjectionMatrix", c_mgMatrix*m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection());
         //dbc_shaderunit->setShaderMatrix(MBS_DIRECTIONAL, "g_mWorld", c_mgMatrix);
         //dbc_shaderunit->setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", c_mgMatrix*dbc_engine->getCameraPtr()->getMView()*dbc_engine->getCameraPtr()->getMProjection());
         c_mtpr[0] = m_cMeshMaterials[i].Ambient.r; c_mtpr[1] = m_cMeshMaterials[i].Ambient.g; c_mtpr[2] = m_cMeshMaterials[i].Ambient.b; c_mtpr[3] = m_cMeshMaterials[i].Ambient.a;          
         m_shaderunit->SetValue("material.ambient", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_cMeshMaterials[i].Diffuse.r; c_mtpr[1] = m_cMeshMaterials[i].Diffuse.g; c_mtpr[2] = m_cMeshMaterials[i].Diffuse.b; c_mtpr[3] = m_cMeshMaterials[i].Diffuse.a;         
         m_shaderunit->SetValue("material.diffuse", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_cMeshMaterials[i].Emissive.r; c_mtpr[1] = m_cMeshMaterials[i].Emissive.g; c_mtpr[2] = m_cMeshMaterials[i].Emissive.b; c_mtpr[3] = m_cMeshMaterials[i].Emissive.a;           
         m_shaderunit->SetValue("material.emissive", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_cMeshMaterials[i].Specular.r; c_mtpr[1] = m_cMeshMaterials[i].Specular.g; c_mtpr[2] = m_cMeshMaterials[i].Specular.b; c_mtpr[3] = m_cMeshMaterials[i].Specular.a;          
         m_shaderunit->SetValue("material.specular", c_mtpr, sizeof(c_mtpr));

         m_shaderunit->SetTexture("colorMapTexture", m_cMeshTextures[i]);

         m_shaderunit->InvokeShaderPassSeries(totalPasses);
         //dbc_shaderunit->invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
         for (UINT pass = 0; pass < totalPasses; pass++)
         {
            m_shaderunit->InvokeShaderPass(pass);
            m_shaderunit->ShaderCommitChanges();
            //dbc_shaderunit->invokeShaderPass(MBS_DIRECTIONAL, pass);
            //dbc_shaderunit->shaderCommitChanges(MBS_DIRECTIONAL);
            //Set the material and texture for this subset
            m_d3dd->SetMaterial(&m_cMeshMaterials[i]);
            if (m_texturing)
               m_d3dd->SetTexture(0, m_cMeshTextures[i]);
            else
               m_d3dd->SetTexture(0, m_emptyTexture);
            // Draw the mesh subset
            m_cMesh->DrawSubset(i);
            m_shaderunit->WithdrawShaderPass();
            //dbc_shaderunit->withdrawShaderPass(MBS_DIRECTIONAL);
         }
         m_shaderunit->WithdrawShaderPassSeries();
         //dbc_shaderunit->withdrawShaderPassSeries(MBS_DIRECTIONAL);
      }
   }
   else
   {
      for (DWORD i = 0; i < m_cNumMaterials; i++)
      {
         m_d3dd->SetMaterial(&m_cMeshMaterials[i]);
         if (m_texturing)
            m_d3dd->SetTexture(0, m_cMeshTextures[i]);
         else
            m_d3dd->SetTexture(0, m_emptyTexture);
         m_cMesh->DrawSubset(i);
      }
   }

   D3DXMatrixRotationY(&c_rtMatrix, 0);
   D3DXMatrixTranslation(&c_mvMatrix, 0, 0, 0);        //мир обратно:)
   D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
}

// Перенаправление на переключение тумблера основного освещения машины
void CarSystem::CarHMLightRedirect(bool ctrl)
{
   if (ctrl)
   {
      m_light->ToggleState(CBN_FRONT_BEAM_HEADLIGHTS);
      return;
   }
   if (m_lightStatus == DBC_NONE)
   {
      m_lightStatus = DBC_MARKER;
      m_light->ToggleState(CBN_REAR_MARKER_LIGHTS, true);
   }
   else if (m_lightStatus == DBC_MARKER)
   {
      m_lightStatus = DBC_ALL;
      m_light->ToggleState(CBN_FRONT_DIPPED_HEADLIGHTS, true);
   }
   else if (m_lightStatus == DBC_ALL)
   {
      m_lightStatus = DBC_NONE;
      m_light->ToggleState(CBN_REAR_MARKER_LIGHTS, false);
      m_light->ToggleState(CBN_FRONT_DIPPED_HEADLIGHTS, false);
   }
}

// Перенаправление на другое перенаправление переключения тумблера неонового освещения машины
void CarSystem::CarNeonRedirect()
{
   m_light->NeonRedirect();
}

// Перенаправление на другое перенаправление переключения тумблера неонового освещения машины
void CarSystem::CarSignalRedirect(SGNTYPE type)
{
   static bool en = false;

   if (type == DBC_LEFT)
      m_light->ToggleState(CBN_LEFT_SIGNAL_LIGHT);
   else if (type == DBC_RIGHT)
      m_light->ToggleState(CBN_RIGHT_SIGNAL_LIGHT);
   else if (type == DBC_ALL)
   {
      en = !en;
      m_light->ToggleState(CBN_LEFT_SIGNAL_LIGHT, en);
      m_light->ToggleState(CBN_RIGHT_SIGNAL_LIGHT, en);
   }
   else
      ;
}

// Перенаправление на другое перенаправление переключения тумблера контроля машины
void CarSystem::SwitchUserControlRedirect()
{
   if (m_engine->GetWRLDType() != MBE_ALPHADEMO)
      m_move->SwitchUserControl();
}

// Перенаправление на функцию определния типа траектории машины
WTYPE CarSystem::GetWPType()
{
   return m_move->GetWPType();
}

// Перенаправление на обработку ручного управления машиной
void CarSystem::MoveRedirect(float dx, float dy)
{
   m_move->ProcessMoveControl(dx, dy);
}

void CarSystem::CarTransmissionRedirect(DMSTYPE type)
{
   m_move->ToggleTransmissionDMode(type);
}

// Инициализация переменных параметров класса машины(пока что по default'у)
void CarSystem::initService()
{}

// Инициализация переменных с данными машины(из .х файла)
void CarSystem::loadModel(LPCWSTR modelFile)
{
   //empty(fictive)
   HRESULT hr = D3DXCreateTexture(m_d3dd, 128, 64, 0, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_emptyTexture);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_EMPTYTEXTURECREATIONFAILURE, "");

   //car
   LPD3DXBUFFER c_materialBuffer = NULL;
   m_cNumMaterials = 0;
   m_cMesh = NULL;
   hr = D3DXLoadMeshFromX(modelFile, D3DXMESH_SYSTEMMEM,
      m_d3dd, NULL,
      &c_materialBuffer, NULL,
      &m_cNumMaterials, &m_cMesh);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_MESHLOADFAILURE, (const char*)modelFile);

   // Loading the material buffer
   D3DXMATERIAL* c_d3dxMaterials = (D3DXMATERIAL*)c_materialBuffer->GetBufferPointer();
   // Holding material and texture pointers
   //dbc_cNumMaterials *= 2;
   m_cMeshMaterials = new D3DMATERIAL9[m_cNumMaterials];
   m_cMeshTextures = new LPDIRECT3DTEXTURE9[m_cNumMaterials];
   // Filling material and texture arrays
    
   for (DWORD i = 0; i < m_cNumMaterials; i++)
   {
      // Copy the material
      m_cMeshMaterials[i] = c_d3dxMaterials[i].MatD3D;

      // Set the ambient color for the material (D3DX does not do this)
      m_cMeshMaterials[i].Ambient = m_cMeshMaterials[i].Diffuse;

      // Create the texture if it exists - it may not

      if (FAILED(D3DXCreateTextureFromFileA(m_d3dd, (LPCSTR)c_d3dxMaterials[i].pTextureFilename, &m_cMeshTextures[i])))
         m_cMeshTextures[i] = NULL;
   }

   //магия! не трогать!
   LPDIRECT3DSURFACE9 tmp_surf = nullptr, tmp_psurf = nullptr;
   LPDIRECT3DTEXTURE9 tmp_tex;

   hr = D3DXCreateTextureFromFileW(m_d3dd, L"res/pct/texture_data/lamborghini/Lamborghini_b.tga", &tmp_tex);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_SUBTEXTURELOADFAILURE, "res/pct/texture_data/lamborghini/Lamborghini_b.tga");

   tmp_tex->GetSurfaceLevel(1, &tmp_psurf);

   assert(m_cMeshTextures[1] != 0);

   m_cMeshTextures[1]->GetSurfaceLevel(1, &tmp_surf);

   //D3DXLoadSurfaceFromFile(tmp_surf, 0, 0, (LPCWSTR)"Lamburghini_n.tga", 0, D3DX_DEFAULT, 0, 0);
   D3DXLoadSurfaceFromSurface(tmp_surf, 0, 0, tmp_psurf, 0, 0, D3DX_DEFAULT, 0);
   tmp_surf->Release();

   tmp_psurf->Release();
   tmp_tex->Release();

   tmp_surf = nullptr, tmp_psurf = nullptr;

   hr = D3DXCreateTextureFromFile(m_d3dd, L"res/pct/texture_data/lamborghini/Lamborghini_r.tga", &tmp_tex);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_SUBTEXTURELOADFAILURE, "res/pct/texture_data/lamborghini/Lamborghini_r.tga");

   tmp_tex->GetSurfaceLevel(2, &tmp_psurf);

   assert(m_cMeshTextures[1] != 0);

   m_cMeshTextures[1]->GetSurfaceLevel(2, &tmp_surf);

   //D3DXLoadSurfaceFromFile(tmp_surf, 0, 0, (LPCWSTR)"Lamburghini_n.tga", 0, D3DX_DEFAULT, 0, 0);
   D3DXLoadSurfaceFromSurface(tmp_surf, 0, 0, tmp_psurf, 0, 0, D3DX_DEFAULT, 0);
   tmp_surf->Release();

   tmp_psurf->Release();
   tmp_tex->Release();

   c_materialBuffer->Release();
}

// Запуск поворотной трансформации
void CarSystem::StartWRotate()
{
   m_transforming = true;
   m_tAPitch = m_engine->GetTAPitch();
   if (m_tAPitch >= 0.0f)
      m_tALimit = m_transformAngle + D3DX_PI;
   else
      m_tALimit = m_transformAngle - D3DX_PI;
}

// Поворот с проверкой(сигнал финига в движок)
void CarSystem::CheckWRotate()
{
   if (!m_transforming)
      return;
   if (((m_tALimit - (m_transformAngle + m_tAPitch))*(m_tALimit - m_transformAngle)) < 0.0f)
   {
      m_transformAngle = m_tALimit;
      m_transforming = false;
      m_engine->FinishWTransform(MBE_CARSYSTEM);
      return;
   }
   m_transformAngle += m_tAPitch;
}

// Проверка приоритета
void CarSystem::priorSwitch()
{
   if (((m_engine->GetCamera()->GetCurrentHeight() >= 0)
      && (m_engine->GetWRLDType() == MBE_STANDARD))
      || ((m_engine->GetCamera()->GetCurrentHeight() < 0)
      && (m_engine->GetWRLDType() == MBE_ALPHADEMO)))
      m_priority = SBM_PRIORLOW;
   else
      m_priority = SBM_PRIORHIGH;
}