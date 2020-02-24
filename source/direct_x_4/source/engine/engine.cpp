/**
@file     engine.cpp
@brief    Engine processing class
@date     Created on 28/10/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "engine.h"
#include "subsystems/ordermanager.h"
#include "utils/camera.h"
#include "utils/infoboard.h"
#include "car_model/carsystem.h"
#include "car_model/carlight.h"
#include "car_model/carmove.h"
#include "utils/light.h"
#include "shader_processing/shaderunit.h"
#include "environment/environment.h"
#include "environment/envlights.h"
#include "DirectXApp.h"
#include "datatransfer\socket\ConnectionManager.h"

Engine::Engine(DirectXApp* main, LPDIRECT3DDEVICE9 dev, int i_nW, int i_nH)
   : m_d3dd(dev)
   , m_shaderUsage(false)
   , m_carStopping(false)
   , m_cTransforming(false)
   , m_eTransforming(false)
   , m_width(static_cast<DWORD>(i_nW))
   , m_height(static_cast<DWORD>(i_nH))
   , m_type(MBE_ALPHADEMO)
   , m_tranformAnglePitch(0.005f)
   , m_stTransformAngle(D3DX_PI)
{
   m_this = std::shared_ptr<Engine>(this);
   m_main = std::shared_ptr<DirectXApp>(main);
}

Engine::~Engine()
{}

// Вызов конструктора класса управления рендером
void Engine::CreateOrderManager()
{
   m_manager = std::make_shared<OrderManager>(m_this);
}

// Вызов конструктора класса камеры
void Engine::CreateCamera(LPDIRECT3DDEVICE9 dev)
{
   m_camera = std::make_shared<Camera>(m_this, dev);
}

// Вызов конструктора класса камеры
void Engine::CreateCamera(LPDIRECT3DDEVICE9 dev, float eX, float eY, float eZ, float laX, float laY, float laZ)
{
   m_camera = std::make_shared<Camera>(m_this, dev, eX, eY, eZ, laX, laY, laZ);
}

// Вызов конструктора класса системы авто
void Engine::CreateCarsystem(LPDIRECT3DDEVICE9 dev, LPCWSTR modelFile, LPCWSTR waypointFile, DWORD stIndex)
{
   CreateCarlight(dev, stIndex);
   CreateCarmove(waypointFile, CarSystem::DEFAULT_YSPIKE);
   m_carsystem = std::make_shared<CarSystem>(m_this, dev, modelFile, waypointFile, stIndex);
}

// Вызов конструктора класса камеры
void Engine::CreateInfoboard(LPDIRECT3D9 d3d9, LPDIRECT3DDEVICE9 dev)
{
   m_infoboard = std::make_shared<Infoboard>(m_this, d3d9, dev);
}

// Вызов конструктора класса освещения машины
void Engine::CreateCarlight(LPDIRECT3DDEVICE9 dev, DWORD stIndex)
{
   m_carlight = std::make_shared<CarLight>(m_this, dev, stIndex);
}

// Вызов конструктора класса освещения машины
void Engine::CreateCarmove(LPCWSTR waypointFile, float ySpike)
{
   m_carmove = std::make_shared<CarMove>(m_this, waypointFile, ySpike);
}

// Вызов конструктора класса освещения машины
void Engine::CreateLightDispatcher(LPDIRECT3DDEVICE9 dev, DWORD count)
{
   m_light = std::make_shared<Light>(m_this, dev, count);
}

// Вызов конструктора класса шейдеров
void Engine::CreateShaderunit(LPDIRECT3DDEVICE9 dev)
{
   m_shaderunit = std::make_shared<ShaderUnit>(m_this, dev);
}

// Вызов конструктора класса окр. среды
void Engine::CreateEnvironment(LPDIRECT3DDEVICE9 dev)
{
   m_environment = std::make_shared<Environment>(m_this, dev);
}

// Вызов конструктора класса освещения окр. среды
void Engine::CreateEnvlight(LPDIRECT3DDEVICE9 dev)
{
   m_envlight = std::make_shared<EnvLight>(m_this, dev);
}

// Вызов конструктора класса менеджера протокола соединения
void Engine::CreateConnectionManager()
{
   m_connectionManager = std::make_shared<ConnectionManager>(m_this);
}

// Отрисовки отладочной сферы
void Engine::DrawDbgSphere(D3DXVECTOR3 pos)
{
   if (m_shaderUsage)
      m_shaderunit->DrawDbgSphere(pos);
}

// Обновление данных по всем объектам(в.т.ч. по не участвующим в сцене)
void Engine::CascadeUpdate()
{
   m_environment->Update();
   m_carsystem->Update();    //там же свет и путь от машины
   m_camera->Update();
   if (!m_shaderUsage)
      m_light->Update();
   else
      m_shaderunit->Update();   //данные + эффекты
   m_infoboard->Update();
}

// Отрисовка объектов по новым данным после каскадного обновления
void Engine::CascadeRender()
{
   //тут через регкласс надо делать
   if (!m_shaderUsage)
   {
      m_manager->Render();
      m_carsystem->Render();
      m_environment->Render();
      m_camera->Render();       //пустой(пока что)
      m_light->Render();
   }
   else
   {
      m_shaderunit->Update();
      m_manager->Render();
      m_carsystem->Render();
      m_environment->Render();
      m_shaderunit->Render();
   }
   m_infoboard->Render();  //а тут нету текстур, взаимодействующих со светом
}

// Каскадное освобождение всех объектов директикса(перед ресетом устройства)
void Engine::CascadeRelease()
{
   m_camera->LRelease();
   m_infoboard->LRelease();
   m_carsystem->LRelease();
   m_carlight->LRelease();
   m_carmove->LRelease();
   m_light->LRelease();
   m_shaderunit->LRelease();
   m_environment->LRelease();
   m_envlight->LRelease();
}

// Каскадное автовосстановление всех объектов директикса(после ресета устройства)
void Engine::CascadeRecreate(int i_nW, int i_nH)
{
   m_width = static_cast<DWORD>(i_nW);
   m_height = static_cast<DWORD>(i_nH);

   m_camera->LRecreate(m_width, m_height);
   m_infoboard->LRecreate(m_width, m_height, true);
   m_carsystem->LRecreate();
   m_carlight->LRecreate();
   m_carmove->LRecreate();
   m_light->LRecreate();
   m_shaderunit->LRecreate();
   m_environment->LRecreate();
   m_envlight->LRecreate();
}

// Запуск трансформации мира в режим демонстрации колонн/обычный режим(тумблер)
bool Engine::StartWTransform()
{
   if (m_shaderUsage)
      m_shaderUsage = false;
   if ((m_eTransforming)
      || (m_cTransforming)
      || (m_carStopping))
      return false;

   m_carStopping = true;
   m_carmove->CarStop();
   //StartWRotate();
   return true;
}

// Каскадный запуск повортных трансформаций мира
void Engine::StartWRotate()
{
   //m_environment->StartWRotate();
   m_carsystem->StartWRotate();
   m_eTransforming = true;
   m_cTransforming = true;
   m_carStopping = false;
}

// Завершения трансформации мира(м-у стартом и финишем повторная трансформация блокирована)
void Engine::FinishWTransform(FCTYPE type)
{
    (type == MBE_ENVIRONMENT) ? m_eTransforming = false : m_cTransforming = false;
    if ((!m_cTransforming)
        && (!m_eTransforming))
       (m_type == MBE_STANDARD) ? (m_type = MBE_ALPHADEMO) : (m_type = MBE_STANDARD, m_carmove->CarLaunch());
}