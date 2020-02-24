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

Environment::Environment(std::shared_ptr<Engine>& eng, LPDIRECT3DDEVICE9 dev)
   : m_d3dd(dev)
   , m_engine(eng)
   , m_shaderunit(eng->GetShader())
   , m_envlight(eng->GetEnvlight())
   , m_transforming(false)
{
   m_this = std::shared_ptr<Environment>(this);
   m_vFlags = D3DFVF_XYZ | D3DFVF_DIFFUSE;

   m_engine->CreateEnvlight(m_d3dd);

   m_texturing = DEFAULT_TEXTURING;

   m_xCount = DEFAULT_X_MESH_COUNT;
   m_zCount = DEFAULT_Z_MESH_COUNT;
   m_xStart = DEFAULT_X_MESH_MIN_START;
   m_zStart = DEFAULT_Z_MESH_MIN_START;
   m_xDelta = DEFAULT_X_MESH_DELTA;
   m_zDelta = DEFAULT_Z_MESH_DELTA;

   m_transformAngle = eng->GetStartTransformAngle();

   loadModels();
}


Environment::~Environment()
{
   LRelease();
   //delete tbe_pLight;
}

// Освобождение объектов директикса(перед ресетом устройства)
void Environment::LRelease()
{
}

// Автовосстановление объектов директикса(после ресетом устройства)
void Environment::LRecreate()
{
   loadModels();
}

// Обновление данных(пустая, т.к. объекты статичны)
void Environment::Update()
{
   m_envlight->Update();
}

// Отрисовка моделей окр. среды(на каждом кадре)
void Environment::Render()
{
   m_d3dd->SetFVF(m_vFlags);
   m_d3dd->SetStreamSource(0, m_vertexBuf, 0, sizeof(vertex));
   m_d3dd->SetIndices(m_indexBuf);
   m_d3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_primCount);

   //потом одним условием запилить
   //(type == TBE_HOUSE) ? drawSingleHouse(reinterpret_cast<tb_gPart*>(objInstance)->m_pos, reinterpret_cast<tb_hPart*>(objInstance)->m_angle) : (type == TBE_GROUND) ? drawSingleGround(reinterpret_cast<tb_hPart*>(objInstance)->m_pos) : drawSingleColumn(reinterpret_cast<tb_cPart*>(objInstance)->m_pos);
   //(type == TBE_HOUSE) ? D3DXVec3TransformCoord(&reinterpret_cast<tb_gPart*>(objInstance)->m_realPos, &reinterpret_cast<tb_gPart*>(objInstance)->m_pos, &m_wTReinterpret) : (type == TBE_GROUND) ? D3DXVec3TransformCoord(&reinterpret_cast<tb_hPart*>(objInstance)->m_realPos, &reinterpret_cast<tb_hPart*>(objInstance)->m_pos, &m_wTReinterpret) : D3DXVec3TransformCoord(&reinterpret_cast<tb_cPart*>(objInstance)->m_realPos, &reinterpret_cast<tb_cPart*>(objInstance)->m_pos, &m_wTReinterpret);
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

// Загрузка моделей окружения(пока что модели предопределены здесь)
void Environment::loadModels()
{
   createEmptyTexture();
}

// Создание пустой текстуры(текстуринг вкл./выкл.)
void Environment::createEmptyTexture()
{
   //empty(fictive)
   HRESULT hr = D3DXCreateTexture(m_d3dd, 128, 64, 0, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_emptyTexture);
   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_EMPTYTEXTURECREATIONFAILURE, "");
}

// Загрузка поверхности в виде поля координат
void Environment::SetSurface(const double** data)
{
   m_data = data;
   int cellCountL = 300;
   m_vertexStorage = new vertex[cellCountL * cellCountL * 8];
   m_indexStorage = new int[cellCountL * cellCountL * 12];
   //mountFigure();
   m_d3dd->CreateVertexBuffer((2 * m_vertexCount) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &m_vertexBuf, NULL);
   m_d3dd->CreateIndexBuffer((2 * m_indexCount) * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuf, NULL);
   refillVertexBuf();
   refillIndexBuf();
}

// 
void Environment::refillVertexBuf()
{
   m_vertexStorage;
}

// 
void Environment::refillIndexBuf()
{
   m_vertexBuf;
}