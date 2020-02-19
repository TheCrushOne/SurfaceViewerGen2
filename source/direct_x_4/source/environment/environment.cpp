/**
@file     environment.cpp
@brief    Static environment support class
@date     Created on 08/11/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "environment.h"
#include "engine\engine.h"
#include "environment\envlights.h"
#include "engine/shader_processing/shaderunit.h"
#include "utils\camera.h"
#include "engine\subsystems\ordermanager.h"

const LPCWSTR Environment::DEFAULT_GROUND_NAME = L"res/models/tera_2.x";
const LPCWSTR Environment::DEFAULT_HOUSE_NAME = L"res/models/fw43_lowpoly_n1.x";
const LPCWSTR Environment::DEFAULT_COLUMN_NAME = L"res/models/column.x";

const DWORD Environment::DEFAULT_HOUSE_COUNT = 2;
const DWORD Environment::DEFAULT_COLUMN_COUNT = 18;

const float Environment::DEFAULT_COLUMN_SCALING = 0.01f;

const DWORD Environment::DEFAULT_X_MESH_COUNT = 3;//10;
const DWORD Environment::DEFAULT_Z_MESH_COUNT = 1;//7;
const float Environment::DEFAULT_X_MESH_MIN_START = -31.0f;// -15.5f;
const float Environment::DEFAULT_Z_MESH_MIN_START = 10.0;//-7.5f;
const float Environment::DEFAULT_X_MESH_DELTA = 31.2f;//7.8f;
const float Environment::DEFAULT_Z_MESH_DELTA = 6.25f;
const float Environment::DEFAULT_Y_SPIKE = -0.15f;
const float Environment::DEFAULT_COLUMN_SPIKE = -0.19f;
const bool Environment::DEFAULT_TEXTURING = true;

const float Environment::DEFAULT_ZDELTA = 10.0f;

const float Environment::DEFAULT_COLUMN_COORDINATES[DEFAULT_COLUMN_COUNT][2] = {        //хмм...забавно, если это работает:)
   #include "column_coordinates.txt"
};

Environment::Environment(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev)
   : m_d3dd(dev)
   , m_engine(eng)
   , m_shaderunit(eng->GetShader())
   , m_envlight(eng->GetEnvlight())
   , m_transforming(false)
{
   m_this = std::shared_ptr<Environment>(this);

   m_engine->CreateEnvlight(m_d3dd);

   m_texturing = DEFAULT_TEXTURING;

   m_xCount = DEFAULT_X_MESH_COUNT;
   m_zCount = DEFAULT_Z_MESH_COUNT;
   m_xStart = DEFAULT_X_MESH_MIN_START;
   m_zStart = DEFAULT_Z_MESH_MIN_START;
   m_xDelta = DEFAULT_X_MESH_DELTA;
   m_zDelta = DEFAULT_Z_MESH_DELTA;

   m_hCount = DEFAULT_HOUSE_COUNT;
   m_cCount = DEFAULT_COLUMN_COUNT;

   m_transformAngle = eng->GetStartTransformAngle();

   loadModels();
   mountGPartition();
   mountHPartition();
   mountCPartition();  
}


Environment::~Environment()
{
   LRelease();
   //delete tbe_pLight;
   for (DWORD i = 0; i < m_gCount; i++)
      delete m_groundArray[i];
   delete m_groundArray;
   for (DWORD i = 0; i < m_hCount; i++)
      delete m_houseArray[i];
   delete m_houseArray;
   for (DWORD i = 0; i < m_cCount; i++)
      delete m_columnArray[i];
   delete m_columnArray;
}


Environment::tb_gPart::tb_gPart(D3DXVECTOR3 pos, std::shared_ptr<Environment>& fnd, PRIORTYPE prior = SBM_PRIORMEDIUM)
   : m_pos(pos)
   , m_fnd(fnd)
   , m_priority(prior)
{
   m_fnd->m_engine->GetOrderManager()->RegisterObject(&tb_gPart::domRender, reinterpret_cast<void*>(this), &m_realPos, &m_priority);
}

Environment::tb_hPart::tb_hPart(D3DXVECTOR3 pos, float angle, std::shared_ptr<Environment>& fnd)
   : m_pos(pos)
   , m_fnd(fnd)
   , m_angle(angle)
{
   m_fnd->m_engine->GetOrderManager()->RegisterObject(&tb_hPart::domRender, reinterpret_cast<void*>(this), &m_realPos, &m_priority);
}

Environment::tb_cPart::tb_cPart(D3DXVECTOR3 pos, std::shared_ptr<Environment>& fnd)
   : m_pos(pos)
   , m_fnd(fnd)
{
   m_fnd->m_engine->GetOrderManager()->RegisterObject(&tb_cPart::domRender, reinterpret_cast<void*>(this), &m_realPos, &m_priority);
}

// Освобождение объектов директикса(перед ресетом устройства)
void Environment::LRelease()
{
    for (DWORD i = 0; i < m_hNumMaterials; i++)
        SAFE_RELEASE(m_hMeshTextures[i]);
    SAFE_RELEASE(m_hMesh);
    for (DWORD i = 0; i < m_gNumMaterials; i++)
        SAFE_RELEASE(m_gMeshTextures[i]);
    SAFE_RELEASE(m_gMesh);
    for (DWORD i = 0; i < m_cNumMaterials; i++)
        SAFE_RELEASE(m_cMeshTextures[i]);
    SAFE_RELEASE(m_cMesh);
    SAFE_RELEASE(m_emptyTexture);
    delete m_hMeshMaterials;
    delete m_gMeshMaterials;
    delete m_cMeshMaterials;
    delete m_hMeshTextures;
    delete m_gMeshTextures;
    delete m_cMeshTextures;
}

// Автовосстановление объектов директикса(после ресетом устройства)
void Environment::LRecreate()
{
   loadModels();
}

// Обновление данных(пустая, т.к. объекты статичны)
void Environment::Update()
{
   updateGArrPrior();
   updateHArrPrior();
   updateCArrPrior();
   CheckWRotate();
   m_envlight->Update();
}

// Отрисовка моделей окр. среды(на каждом кадре)
void Environment::Render(void *objInstance, TBE_OBJTYPE type)
{
   updateWTReinterpret();
   //потом одним условием запилить
   (type == TBE_HOUSE) ? drawSingleHouse(reinterpret_cast<tb_gPart*>(objInstance)->m_pos, reinterpret_cast<tb_hPart*>(objInstance)->m_angle) : (type == TBE_GROUND) ? drawSingleGround(reinterpret_cast<tb_hPart*>(objInstance)->m_pos) : drawSingleColumn(reinterpret_cast<tb_cPart*>(objInstance)->m_pos);
   (type == TBE_HOUSE) ? D3DXVec3TransformCoord(&reinterpret_cast<tb_gPart*>(objInstance)->m_realPos, &reinterpret_cast<tb_gPart*>(objInstance)->m_pos, &m_wTReinterpret) : (type == TBE_GROUND) ? D3DXVec3TransformCoord(&reinterpret_cast<tb_hPart*>(objInstance)->m_realPos, &reinterpret_cast<tb_hPart*>(objInstance)->m_pos, &m_wTReinterpret) : D3DXVec3TransformCoord(&reinterpret_cast<tb_cPart*>(objInstance)->m_realPos, &reinterpret_cast<tb_cPart*>(objInstance)->m_pos, &m_wTReinterpret);
}

// Перенаправленик на переключение тумблера направленного освещения
void Environment::DLightRedirect()
{
   m_envlight->ToggleState(CBE_DLIGHT);
}

// Перенаправление на переключение тумблера точечного освещения
void Environment::PLightRedirect()
{
   m_envlight->ToggleState(CBE_PLIGHT);
}

// Обновление приоритета земли(запасная функция)
void Environment::updateGArrPrior()
{
   return;     //у земли всегда медиум приоритет
}

// Обновление приоритета домов
void Environment::updateHArrPrior()
{
   static DWORD i = 0;

   if (((m_engine->GetCamera()->GetCurrentHeight() >= 0)
      && (m_engine->GetWRLDType() == MBE_STANDARD))
      || ((m_engine->GetCamera()->GetCurrentHeight() < 0)
      && (m_engine->GetWRLDType() == MBE_ALPHADEMO)))
      for (i = 0; i < m_hCount; i++)
         m_houseArray[i]->m_priority = SBM_PRIORLOW;
   else
      for (i = 0; i < m_hCount; i++)
         m_houseArray[i]->m_priority = SBM_PRIORHIGH;
}

// Обновление приоритета колонн
void Environment::updateCArrPrior()
{
   static DWORD i = 0;

   if (((m_engine->GetCamera()->GetCurrentHeight() >= 0)
      && (m_engine->GetWRLDType() == MBE_STANDARD))
      || ((m_engine->GetCamera()->GetCurrentHeight() < 0)
      && (m_engine->GetWRLDType() == MBE_ALPHADEMO)))
      for (i = 0; i < m_cCount; i++)
         m_columnArray[i]->m_priority = SBM_PRIORHIGH;
   else
      for (i = 0; i < m_cCount; i++)
         m_columnArray[i]->m_priority = SBM_PRIORLOW;

}

// Обновление матрицы предтрансформации мира
void Environment::updateWTReinterpret()
{
   static D3DXMATRIX c_rtMatrix, c_mvMatrix;
   D3DXMatrixRotationX(&c_rtMatrix, m_transformAngle);
   D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, DEFAULT_ZDELTA);

   //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
   D3DXMatrixMultiply(&m_wTReinterpret, &c_rtMatrix, &c_mvMatrix);
   D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, -DEFAULT_ZDELTA);
   D3DXMatrixMultiply(&m_wTReinterpret, &c_mvMatrix, &m_wTReinterpret);
}

// Загрузка моделей окружения(пока что модели предопределены здесь)
void Environment::loadModels()
{
   loadGround(DEFAULT_GROUND_NAME);
   loadHouse(DEFAULT_HOUSE_NAME);
   loadColumn(DEFAULT_COLUMN_NAME);
   createEmptyTexture();
}

// Загрузка модели поверхности земли
void Environment::loadGround(LPCWSTR name)
{
   //ground
   LPD3DXBUFFER g_materialBuffer = NULL;
   m_gNumMaterials = 0;
   m_gMesh = NULL;
   HRESULT hr = D3DXLoadMeshFromX(name, D3DXMESH_SYSTEMMEM,
      m_d3dd, NULL,
      &g_materialBuffer, NULL,
      &m_gNumMaterials, &m_gMesh);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_MESHLOADFAILURE, (const char*)name);

   // Loading the material buffer
   D3DXMATERIAL* g_d3dxMaterials = (D3DXMATERIAL*)g_materialBuffer->GetBufferPointer();
   // Holding material and texture pointers
   m_gMeshMaterials = new D3DMATERIAL9[m_gNumMaterials];
   m_gMeshTextures = new LPDIRECT3DTEXTURE9[m_gNumMaterials];
   // Filling material and texture arrays
   for (DWORD i = 0; i < m_gNumMaterials; i++)
   {
      // Copy the material
      m_gMeshMaterials[i] = g_d3dxMaterials[i].MatD3D;

      //if (i == 0)
      //sta_gMeshMaterials[i].Diffuse = {0.5f, 0.5f, 0.9f, 1.0f};
      // Set the ambient color for the material (D3DX does not do this)
      m_gMeshMaterials[i].Ambient = m_gMeshMaterials[i].Diffuse;

      // Create the texture if it exists - it may not

      if (FAILED(D3DXCreateTextureFromFileA(m_d3dd, (LPCSTR)g_d3dxMaterials[i].pTextureFilename, &m_gMeshTextures[i])))
         m_gMeshTextures[i] = NULL;
   }

   g_materialBuffer->Release();

   LPDIRECT3DSURFACE9 tmp_surf = 0, tmp_psurf = nullptr;
   LPDIRECT3DTEXTURE9 tmp_tex;

   D3DXCreateTextureFromFile(m_d3dd, L"res/pct/texture_data/ground/ground_1226_g.jpg", &tmp_tex);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_SUBTEXTURELOADFAILURE, "res/pct/texture_data/ground/ground_1226_g.jpg");

   tmp_tex->GetSurfaceLevel(1, &tmp_psurf);

   assert(m_gMeshTextures[0] != 0);

   m_gMeshTextures[0]->GetSurfaceLevel(1, &tmp_surf);

   //D3DXLoadSurfaceFromFile(tmp_surf, 0, 0, (LPCWSTR)"Lamburghini_n.tga", 0, D3DX_DEFAULT, 0, 0);
   D3DXLoadSurfaceFromSurface(tmp_surf, 0, 0, tmp_psurf, 0, 0, D3DX_DEFAULT, 0);
   tmp_surf->Release();

   tmp_psurf->Release();
   tmp_tex->Release();

   tmp_surf = 0, tmp_psurf = nullptr;
   tmp_tex;

   D3DXCreateTextureFromFile(m_d3dd, L"res/pct/texture_data/ground/ground_1226_j.jpg", &tmp_tex);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_SUBTEXTURELOADFAILURE, "res/pct/texture_data/ground/ground_1226_j.jpg");

   tmp_tex->GetSurfaceLevel(2, &tmp_psurf);

   assert(m_gMeshTextures[0] != 0);

   m_gMeshTextures[0]->GetSurfaceLevel(2, &tmp_surf);

   //D3DXLoadSurfaceFromFile(tmp_surf, 0, 0, (LPCWSTR)"Lamburghini_n.tga", 0, D3DX_DEFAULT, 0, 0);
   D3DXLoadSurfaceFromSurface(tmp_surf, 0, 0, tmp_psurf, 0, 0, D3DX_DEFAULT, 0);
   tmp_surf->Release();

   tmp_psurf->Release();
   tmp_tex->Release();
}

// Загрузка модели дома
void Environment::loadHouse(LPCWSTR name)
{
   //house
   LPD3DXBUFFER h_materialBuffer = NULL;
   m_hNumMaterials = 0;
   m_hMesh = NULL;
   HRESULT hr = D3DXLoadMeshFromX(name, D3DXMESH_SYSTEMMEM,
      m_d3dd, NULL,
      &h_materialBuffer, NULL,
      &m_hNumMaterials, &m_hMesh);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_MESHLOADFAILURE, (const char*)name);

   // Loading the material buffer
   D3DXMATERIAL* h_d3dxMaterials = (D3DXMATERIAL*)h_materialBuffer->GetBufferPointer();
   // Holding material and texture pointers
   m_hMeshMaterials = new D3DMATERIAL9[m_hNumMaterials];
   m_hMeshTextures = new LPDIRECT3DTEXTURE9[m_hNumMaterials];
   // Filling material and texture arrays
   for (DWORD i = 0; i < m_hNumMaterials; i++)
   {
      // Copy the material
      m_hMeshMaterials[i] = h_d3dxMaterials[i].MatD3D;

      // Set the ambient color for the material (D3DX does not do this)
      m_hMeshMaterials[i].Ambient = m_hMeshMaterials[i].Diffuse;

      // Create the texture if it exists - it may not

      if (FAILED(D3DXCreateTextureFromFileA(m_d3dd, (LPCSTR)h_d3dxMaterials[i].pTextureFilename, &m_hMeshTextures[i])))
         m_hMeshTextures[i] = NULL;
   }

   h_materialBuffer->Release();
}

// Загрузка модели колонны
void Environment::loadColumn(LPCWSTR name)
{
   //column
   LPD3DXBUFFER h_materialBuffer = NULL;
   m_cNumMaterials = 0;
   m_cMesh = NULL;
   HRESULT hr = D3DXLoadMeshFromX(name, D3DXMESH_SYSTEMMEM,
      m_d3dd, NULL,
      &h_materialBuffer, NULL,
      &m_cNumMaterials, &m_cMesh);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_MESHLOADFAILURE, (const char*)name);

   // Loading the material buffer
   D3DXMATERIAL* h_d3dxMaterials = (D3DXMATERIAL*)h_materialBuffer->GetBufferPointer();
   // Holding material and texture pointers
   m_cMeshMaterials = new D3DMATERIAL9[m_cNumMaterials];
   m_cMeshTextures = new LPDIRECT3DTEXTURE9[m_cNumMaterials];
   // Filling material and texture arrays
    for (DWORD i = 0; i < m_cNumMaterials; i++)
    {
        // Copy the material
        m_cMeshMaterials[i] = h_d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_cMeshMaterials[i].Ambient = m_cMeshMaterials[i].Diffuse;

        // Create the texture if it exists - it may not

        if (FAILED(D3DXCreateTextureFromFileA(m_d3dd, (LPCSTR)h_d3dxMaterials[i].pTextureFilename, &m_cMeshTextures[i])))
            m_cMeshTextures[i] = NULL;
    }

    h_materialBuffer->Release();
}

// Создание пустой текстуры(текстуринг вкл./выкл.)
void Environment::createEmptyTexture()
{
   //empty(fictive)
   HRESULT hr = D3DXCreateTexture(m_d3dd, 128, 64, 0, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_emptyTexture);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_EMPTYTEXTURECREATIONFAILURE, "");
}

// Расчет данных и регистрации для всех частей земли(сама "карта" состоит из нескольких частей)
void Environment::mountGPartition()
{
   m_groundArray = new tb_gPart*[m_xCount*m_zCount];
   for (DWORD i = 0; i < m_xCount; i++)
      for (DWORD j = 0; j < m_zCount; j++)
         m_groundArray[i + j] = new tb_gPart(D3DXVECTOR3(m_xStart + i*m_xDelta, DEFAULT_Y_SPIKE, m_zStart + j*m_zDelta), m_this);
   m_gCount = m_xCount*m_zCount;
}

// Расчет данных и регистрации для всех домов(их всего 2)
void Environment::mountHPartition()
{
   m_houseArray = new tb_hPart*[m_hCount];
   for (DWORD i = 0; i < m_hCount; i++)
      m_houseArray[i] = new tb_hPart(D3DXVECTOR3(16.0f - static_cast<float>(i)*30.0f, 0.0f, 10.0f), D3DX_PI / 2 - static_cast<float>(i)*D3DX_PI, m_this);
}


// Расчет данных и регистрации для всех колонн
void Environment::mountCPartition()
{
   m_columnArray = new tb_cPart*[m_cCount];
   for (DWORD i = 0; i < m_cCount; i++)
      m_columnArray[i] = new tb_cPart(D3DXVECTOR3(DEFAULT_COLUMN_COORDINATES[i][0], DEFAULT_COLUMN_SPIKE, DEFAULT_COLUMN_COORDINATES[i][1]), m_this);
}

// Отрисовка одной части земли, основанной на многократном использовании одного меха
void Environment::drawSingleGround(D3DXVECTOR3 pos)
{
   static UINT totalPasses;
   static D3DXMATRIX c_mvMatrix, c_rtMatrix, c_mgMatrix;
   static float c_mtpr[4] = { 0, 0, 0, 0 };

   static D3DXCOLOR mDiff = D3DXCOLOR(0, 0, 0, 0);

   D3DXMatrixTranslation(&c_mvMatrix, pos.x, pos.y, pos.z);
   D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &m_wTReinterpret);
   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);

   if (m_engine->CheckShaderUsage())
   {
      m_shaderunit->SwapActiveSType(MBS_BLINNPHONG);
      m_shaderunit->SetTechnique("GLP_MULTICAST");

      //D3DXMatrixTranslation(&c_mvMatrix, pos.x, pos.y, pos.z);

      //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
      //D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &c_rtMatrix);

      //tbe_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
      //ground
      //drawGround();
      for (DWORD k = 0; k < m_gNumMaterials; k++)
      {
         //mDiff = tbe_gMeshMaterials[k].Diffuse;
         //tbe_shaderunit->setValue(MBS_DIRECTIONAL, "g_MaterialDiffuseColor", &mDiff, sizeof(D3DXCOLOR));
         m_shaderunit->SetShaderMatrix("worldMatrix", c_mgMatrix);
         m_shaderunit->SetShaderMatrix("worldViewProjectionMatrix", c_mgMatrix*m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection());
         c_mtpr[0] = m_gMeshMaterials[k].Ambient.r; c_mtpr[1] = m_gMeshMaterials[k].Ambient.g; c_mtpr[2] = m_gMeshMaterials[k].Ambient.b; c_mtpr[3] = m_gMeshMaterials[k].Ambient.a;
         m_shaderunit->SetValue("material.ambient", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_gMeshMaterials[k].Diffuse.r; c_mtpr[1] = m_gMeshMaterials[k].Diffuse.g; c_mtpr[2] = m_gMeshMaterials[k].Diffuse.b; c_mtpr[3] = m_gMeshMaterials[k].Diffuse.a;
         m_shaderunit->SetValue("material.diffuse", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_gMeshMaterials[k].Emissive.r; c_mtpr[1] = m_gMeshMaterials[k].Emissive.g; c_mtpr[2] = m_gMeshMaterials[k].Emissive.b; c_mtpr[3] = m_gMeshMaterials[k].Emissive.a;
         m_shaderunit->SetValue("material.emissive", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_gMeshMaterials[k].Specular.r; c_mtpr[1] = m_gMeshMaterials[k].Specular.g; c_mtpr[2] = m_gMeshMaterials[k].Specular.b; c_mtpr[3] = m_gMeshMaterials[k].Specular.a;
         m_shaderunit->SetValue("material.specular", c_mtpr, sizeof(c_mtpr));
         //tbe_shaderunit->setFloat(MBS_BLINNPHONG, "material.shininess", 5.2f);//spike            
         m_shaderunit->SetTexture("colorMapTexture", m_gMeshTextures[k]);
         m_shaderunit->InvokeShaderPassSeries(totalPasses);
         //tbe_shaderunit->setShaderMatrix(MBS_DIRECTIONAL, "g_mWorld", c_mgMatrix);
         //tbe_shaderunit->setShaderMatrix(MBS_DIRECTIONAL, "g_mWorldViewProjection", c_mgMatrix*tbe_engine->getCameraPtr()->getMView()*tbe_engine->getCameraPtr()->getMProjection());
         //tbe_shaderunit->invokeShaderPassSeries(MBS_DIRECTIONAL, totalPasses);
         for (UINT pass = 0; pass < totalPasses; pass++)
         {
            m_shaderunit->InvokeShaderPass(pass);
            // Set the material and texture for this subset
            //tbe_shaderunit->invokeShaderPass(MBS_DIRECTIONAL, pass);
            //tbe_shaderunit->shaderCommitChanges(MBS_DIRECTIONAL);
            m_d3dd->SetMaterial(&m_gMeshMaterials[k]);

            if (m_texturing)
               m_d3dd->SetTexture(0, m_gMeshTextures[k]);
            else
               m_d3dd->SetTexture(0, m_emptyTexture);
            // Draw the mesh subset
            m_gMesh->DrawSubset(k);
            m_shaderunit->WithdrawShaderPass();
            //tbe_shaderunit->withdrawShaderPass(MBS_DIRECTIONAL);
         }
         m_shaderunit->WithdrawShaderPassSeries();
         //tbe_shaderunit->withdrawShaderPassSeries(MBS_DIRECTIONAL);
      }
   }
   else
   {
      //ground
      //drawGround();
      for (DWORD k = 0; k < m_gNumMaterials; k++)
      {
         // Set the material and texture for this subset
         m_d3dd->SetMaterial(&m_gMeshMaterials[k]);

         if (m_texturing)
            m_d3dd->SetTexture(0, m_gMeshTextures[k]);
         else
            m_d3dd->SetTexture(0, m_emptyTexture);
         // Draw the mesh subset
         m_gMesh->DrawSubset(k);
      }
   }

   D3DXMatrixRotationX(&c_rtMatrix, 0);
   D3DXMatrixTranslation(&c_mvMatrix, 0, 0, 0);        //мир обратно:)
   D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
}

// Отрисовка одного дома
void Environment::drawSingleHouse(D3DXVECTOR3 pos, float angle)
{
   static UINT totalPasses;
   static float c_mtpr[4] = { 0, 0, 0, 0 };
   static D3DXMATRIX c_mvMatrix, c_rtMatrix, c_mgMatrix, c_rxMatrix;

   //D3DXMatrixRotationX(&c_rxMatrix, tbe_transformAngle);
   D3DXMatrixRotationY(&c_rtMatrix, angle);
   //D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, TBE_DEFAULT_ZDELTA);

   //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
   //D3DXMatrixMultiply(&c_mgMatrix, &c_rxMatrix, &c_mvMatrix);
   //D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, -TBE_DEFAULT_ZDELTA);
   //D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &c_mgMatrix);

   //D3DXMatrixTranslation(&c_mvMatrix, 16.0f, 0.0f, 10.0f);
   D3DXMatrixTranslation(&c_mvMatrix, pos.x, pos.y, pos.z);

   //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
   D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &m_wTReinterpret);
   D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mgMatrix);

   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);

   //house
   if (m_engine->CheckShaderUsage())
   {
      m_shaderunit->SwapActiveSType(MBS_BLINNPHONG);
      m_shaderunit->SetTechnique("HLP_MULTICAST");
      for (DWORD i = 0; i < m_hNumMaterials; i++)
      {
         m_shaderunit->SetShaderMatrix("worldMatrix", c_mgMatrix);
         m_shaderunit->SetShaderMatrix("worldViewProjectionMatrix", c_mgMatrix*m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection());
         c_mtpr[0] = m_hMeshMaterials[i].Ambient.r; c_mtpr[1] = m_hMeshMaterials[i].Ambient.g; c_mtpr[2] = m_hMeshMaterials[i].Ambient.b; c_mtpr[3] = m_hMeshMaterials[i].Ambient.a;
         m_shaderunit->SetValue("material.ambient", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_hMeshMaterials[i].Diffuse.r; c_mtpr[1] = m_hMeshMaterials[i].Diffuse.g; c_mtpr[2] = m_hMeshMaterials[i].Diffuse.b; c_mtpr[3] = m_hMeshMaterials[i].Diffuse.a;
         m_shaderunit->SetValue("material.diffuse", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_hMeshMaterials[i].Emissive.r; c_mtpr[1] = m_hMeshMaterials[i].Emissive.g; c_mtpr[2] = m_hMeshMaterials[i].Emissive.b; c_mtpr[3] = m_hMeshMaterials[i].Emissive.a;
         m_shaderunit->SetValue("material.emissive", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_hMeshMaterials[i].Specular.r; c_mtpr[1] = m_hMeshMaterials[i].Specular.g; c_mtpr[2] = m_hMeshMaterials[i].Specular.b; c_mtpr[3] = m_hMeshMaterials[i].Specular.a;
         m_shaderunit->SetValue("material.specular", c_mtpr, sizeof(c_mtpr));
         //tbe_shaderunit->setFloat(MBS_BLINNPHONG, "material.shininess", 5.2f);    //spike
         m_shaderunit->SetTexture("colorMapTexture", m_hMeshTextures[i]);
         m_shaderunit->InvokeShaderPassSeries(totalPasses);

         for (UINT pass = 0; pass < totalPasses; pass++)
         {
            m_shaderunit->InvokeShaderPass(pass);
            // Set the material and texture for this subset
            m_d3dd->SetMaterial(&m_hMeshMaterials[i]);
            if (m_texturing)
               m_d3dd->SetTexture(0, m_hMeshTextures[i]);
            else
               m_d3dd->SetTexture(0, m_emptyTexture);
            // Draw the mesh subset
            m_hMesh->DrawSubset(i);
            m_shaderunit->WithdrawShaderPass();
         }
         m_shaderunit->WithdrawShaderPassSeries();
      }
   }
   else
   {
      for (DWORD i = 0; i < m_hNumMaterials; i++)
      {
         // Set the material and texture for this subset
         m_d3dd->SetMaterial(&m_hMeshMaterials[i]);
         if (m_texturing)
            m_d3dd->SetTexture(0, m_hMeshTextures[i]);
         else
            m_d3dd->SetTexture(0, m_emptyTexture);
         // Draw the mesh subset
         m_hMesh->DrawSubset(i);
      }
   }

   D3DXMatrixRotationX(&c_rxMatrix, 0);
   D3DXMatrixRotationY(&c_rtMatrix, 0);
   //D3DXMatrixTranslation(&c_mvMatrix, 16.0f, 0.0f, 10.0f);
   D3DXMatrixTranslation(&c_mvMatrix, 0, 0, 0);

   //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
   D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &c_rxMatrix);
   D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mgMatrix);

   //D3DXMatrixRotationY(&c_rtMatrix, 0);
   //D3DXMatrixTranslation(&c_mvMatrix, 0, 0, 0);        //мир обратно:)
   //D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
}

// Отрисовка одной колонны, основанной на многократном использовании одного меха
void Environment::drawSingleColumn(D3DXVECTOR3 pos)
{
   static UINT totalPasses;
   static float c_mtpr[4] = { 0, 0, 0, 0 };
   static D3DXMATRIX c_mvMatrix, c_rtMatrix, c_mgMatrix, c_scMatrix;

   m_d3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   //D3DXMatrixRotationX(&c_rtMatrix, tbe_transformAngle);
   //D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, TBE_DEFAULT_ZDELTA);

   //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
   //D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
   //D3DXMatrixTranslation(&c_mvMatrix, 0.0f, 0.0f, -TBE_DEFAULT_ZDELTA);
   //D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &c_mgMatrix);

    
   D3DXMatrixScaling(&c_scMatrix, DEFAULT_COLUMN_SCALING, DEFAULT_COLUMN_SCALING, DEFAULT_COLUMN_SCALING);        //резерв
   //D3DXMatrixTranslation(&c_mvMatrix, 16.0f, 0.0f, 10.0f);
   D3DXMatrixTranslation(&c_mvMatrix, pos.x, pos.y, pos.z);

   //D3DXMatrixScaling(&c_rtMatrix, 0.5f, 0.5f, 0.5f);
   D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &m_wTReinterpret);
   D3DXMatrixMultiply(&c_mgMatrix, &c_scMatrix, &c_mgMatrix);
   D3DXMatrixRotationX(&c_rtMatrix, D3DX_PI);
   D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mgMatrix);
   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
   
   //house
   if (m_engine->CheckShaderUsage())
   {
      m_shaderunit->SwapActiveSType(MBS_BLINNPHONG);
      m_shaderunit->SetTechnique("HLP_MULTICAST");
      for (DWORD i = 0; i < m_cNumMaterials; i++)
      {
         m_shaderunit->SetShaderMatrix("worldMatrix", c_mgMatrix);
         m_shaderunit->SetShaderMatrix("worldViewProjectionMatrix", c_mgMatrix*m_engine->GetCamera()->GetMView()*m_engine->GetCamera()->GetMProjection());
         c_mtpr[0] = m_cMeshMaterials[i].Ambient.r; c_mtpr[1] = m_cMeshMaterials[i].Ambient.g; c_mtpr[2] = m_cMeshMaterials[i].Ambient.b; c_mtpr[3] = m_cMeshMaterials[i].Ambient.a;
         m_shaderunit->SetValue("material.ambient", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_cMeshMaterials[i].Diffuse.r; c_mtpr[1] = m_cMeshMaterials[i].Diffuse.g; c_mtpr[2] = m_cMeshMaterials[i].Diffuse.b; c_mtpr[3] = m_cMeshMaterials[i].Diffuse.a;
         m_shaderunit->SetValue("material.diffuse", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_cMeshMaterials[i].Emissive.r; c_mtpr[1] = m_cMeshMaterials[i].Emissive.g; c_mtpr[2] = m_cMeshMaterials[i].Emissive.b; c_mtpr[3] = m_cMeshMaterials[i].Emissive.a;
         m_shaderunit->SetValue("material.emissive", c_mtpr, sizeof(c_mtpr));
         c_mtpr[0] = m_cMeshMaterials[i].Specular.r; c_mtpr[1] = m_cMeshMaterials[i].Specular.g; c_mtpr[2] = m_cMeshMaterials[i].Specular.b; c_mtpr[3] = m_cMeshMaterials[i].Specular.a;
         m_shaderunit->SetValue("material.specular", c_mtpr, sizeof(c_mtpr));
         //tbe_shaderunit->setFloat(MBS_BLINNPHONG, "material.shininess", 5.2f);//spike            
         m_shaderunit->SetTexture("colorMapTexture", m_cMeshTextures[i]);
         m_shaderunit->InvokeShaderPassSeries(totalPasses);

         for (UINT pass = 0; pass < totalPasses; pass++)
         {
            m_shaderunit->InvokeShaderPass(pass);
            // Set the material and texture for this subset
            m_d3dd->SetMaterial(&m_cMeshMaterials[i]);
            if (m_texturing)
               m_d3dd->SetTexture(0, m_cMeshTextures[i]);
            else
               m_d3dd->SetTexture(0, m_emptyTexture);
            // Draw the mesh subset
            m_cMesh->DrawSubset(i);
            m_shaderunit->WithdrawShaderPass();
         }
         m_shaderunit->WithdrawShaderPassSeries();
      }
   }
   else
   {
      for (DWORD i = 0; i < m_cNumMaterials; i++)
      {
         // Set the material and texture for this subset
         m_d3dd->SetMaterial(&m_cMeshMaterials[i]);
         if (m_texturing)
            m_d3dd->SetTexture(0, m_cMeshTextures[i]);
         else
            m_d3dd->SetTexture(0, m_emptyTexture);
         // Draw the mesh subset
         m_cMesh->DrawSubset(i);
      }
   }

   D3DXMatrixRotationX(&c_rtMatrix, 0);
   D3DXMatrixScaling(&c_scMatrix, 0, 0, 0);        //резерв

   D3DXMatrixTranslation(&c_mvMatrix, 0, 0, 0);

   D3DXMatrixMultiply(&c_mgMatrix, &c_mvMatrix, &c_rtMatrix);
   D3DXMatrixMultiply(&c_mgMatrix, &c_scMatrix, &c_mgMatrix);

         //мир обратно:)
   //D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
   m_d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);

   m_d3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

// Запуск поворотной трансформации
void Environment::StartWRotate()
{
   m_transforming = true;
   m_tAPitch = m_engine->GetTAPitch();
   if (m_tAPitch >= 0.0f)
      m_tALimit = m_transformAngle + D3DX_PI;
   else
      m_tALimit = m_transformAngle - D3DX_PI;
}

// Поворот с проверкой(сигнал финиша в движок)
void Environment::CheckWRotate()
{
   if (!m_transforming)
      return;
   if (((m_tALimit - (m_transformAngle + m_tAPitch))*(m_tALimit - m_transformAngle)) < 0.0f)
   {
      m_transformAngle = m_tALimit;
      m_transforming = false;
      m_engine->FinishWTransform(MBE_ENVIRONMENT);
      return;
   }
   m_transformAngle += m_tAPitch;
}