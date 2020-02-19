#pragma once

/**
@file     engine.h
@brief    Engine processing class
@date     Created on 29/10/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class OrderManager;
class Camera;
class Infoboard;
class CarSystem;
class CarLight;
class CarMove;
class Light;
class ShaderUnit;
class Environment;
class EnvLight;
class DirectXApp;
class ConnectionManager;

// ќсновной класс, объедин€ющий все остальные(дл€ корректной работы)
class Engine
{
public:
   Engine(DirectXApp* main, LPDIRECT3DDEVICE9 dev, int i_nW, int i_nH);
   ~Engine();
   
   inline bool CheckShaderUsage() { return m_shaderUsage; }
   inline void ToggleShaderUsage() { if (m_type != MBE_ALPHADEMO) m_shaderUsage = !m_shaderUsage; }
   
   bool StartWTransform();
   void StartWRotate();
   void FinishWTransform(FCTYPE type);

   void CascadeRelease();
   void CascadeRecreate(int, int);

   void CascadeUpdate();
   void CascadeRender();
   void CreateOrderManager();
   void CreateCamera(LPDIRECT3DDEVICE9 dev);
   void CreateCamera(LPDIRECT3DDEVICE9 dev, float eX, float eY, float eZ, float laX, float laY, float laZ);
   void CreateInfoboard(LPDIRECT3D9 d3d9, LPDIRECT3DDEVICE9 dev);
   void CreateCarsystem(LPDIRECT3DDEVICE9 dev, LPCWSTR modelFile, LPCWSTR waypointFile, DWORD stIndex);
   void CreateCarmove(LPCWSTR waypointFile, float ySpike);
   void CreateCarlight(LPDIRECT3DDEVICE9 dev, DWORD stIndex);
   void CreateLightDispatcher(LPDIRECT3DDEVICE9 dev, DWORD count);
   void CreateShaderunit(LPDIRECT3DDEVICE9 dev);
   void CreateEnvironment(LPDIRECT3DDEVICE9 dev);
   void CreateEnvlight(LPDIRECT3DDEVICE9 dev);
   void CreateConnectionManager();
   void DrawDbgSphere(D3DXVECTOR3 pos); 

   inline float GetStartTransformAngle() const  { return m_stTransformAngle; }
   inline WRLDTYPE GetWRLDType() const { return m_type; }
   inline LPDIRECT3DDEVICE9 GetDevice() const { return m_d3dd; }
   inline float GetTAPitch() const { return m_tranformAnglePitch; }
   inline DWORD GetWidth() const { return m_width; }
   inline DWORD GetHeight() const { return m_height; }
   inline std::shared_ptr<DirectXApp>& GetMain() { return m_main; }
   inline std::shared_ptr<OrderManager>& GetOrderManager() { return m_manager; }
   inline std::shared_ptr<Camera>& GetCamera() { return m_camera; }
   inline std::shared_ptr<Infoboard>& GetInfoboard() { return m_infoboard; }
   inline std::shared_ptr<CarSystem>& GetCar() { return m_carsystem; }
   inline std::shared_ptr<CarLight>& GetCarlight() { return m_carlight; }
   inline std::shared_ptr<CarMove>& GetCarmove() { return m_carmove; }
   inline std::shared_ptr<Light>& GetLight() { return m_light; }
   inline std::shared_ptr<ShaderUnit>& GetShader() { return m_shaderunit; }
   inline std::shared_ptr<Environment>& GetEnvironment() { return m_environment; }
   inline std::shared_ptr<EnvLight>& GetEnvlight() { return m_envlight; }
   inline std::shared_ptr<ConnectionManager>& GetConnectionManager() { return m_connectionManager; }
private:
   LPDIRECT3DDEVICE9 m_d3dd;

   WRLDTYPE m_type;

   std::shared_ptr<DirectXApp> m_main;
   std::shared_ptr<OrderManager> m_manager;
   std::shared_ptr<Camera> m_camera;
   std::shared_ptr<Infoboard> m_infoboard;
   std::shared_ptr<CarSystem> m_carsystem;
   std::shared_ptr<CarLight> m_carlight;
   std::shared_ptr<CarMove> m_carmove;
   std::shared_ptr<Light> m_light;
   std::shared_ptr<ShaderUnit> m_shaderunit;
   std::shared_ptr<Environment> m_environment;
   std::shared_ptr<EnvLight> m_envlight;
   std::shared_ptr<ConnectionManager> m_connectionManager;

   std::shared_ptr<Engine> m_this;

   bool m_shaderUsage;
   DWORD m_width;
   DWORD m_height;

   float m_tranformAnglePitch;
   float m_stTransformAngle;
   bool m_carStopping;
   bool m_cTransforming;
   bool m_eTransforming;
};