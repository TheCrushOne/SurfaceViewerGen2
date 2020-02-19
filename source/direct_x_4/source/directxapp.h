#pragma once
/**
  @file     myApp.h
  @brief    Class myApp definition
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include "library/cglApp.h"
#include "library/cglD3D.h"
#include "utils/gmtr.h"   //utils уже там есть

#include <d3d9.h>

#include <d3dx9.h>	//is его не видит(win8)...

class Engine;
class OrderManager;
class Camera;
class Infoboard;
class CarSystem;
class Light;
class Environment;
class ShaderUnit;
class ConnectionManager;

// *******************************************************************
// defines & constants

// *******************************************************************
// classes 

// Application class

// Основной класс для работы
class DirectXApp : public cglApp
{
   enum { MAX_KEYS = 128 };
public:
  // Constructor
   DirectXApp(int nW, int nH, void* hInst, int nCmdShow)
        : cglApp(nW, nH, hInst, nCmdShow)
        , m_prevMouseX(-100)
        , m_prevMouseY(-100)
   {
      try
      {
         initService();
      }
      catch (uts::AMLSS_E & exception)
      {
         WCHAR buf[200];
         MultiByteToWideChar(CP_ACP, 0, exception.what(), 200, buf, 200);
         m_renderBlock = true;
         MessageBox(NULL, buf, L"Critical error", MB_OK | MB_ICONERROR);
         SendMessageW(HWND(m_hWnd), WM_MASTER_HALT, 0, 0);
      }
   }
   virtual ~DirectXApp();
   virtual bool processInput(unsigned int nMsg, int wParam, long lParam);
   bool processVKInput(int wParam);
protected:
   virtual void update();
   virtual void renderInternal();
public:
   inline bool GetTexturing() { return m_texturing; }
   inline DWORD GetMipFiltration() { return m_filtrationType; }
   inline DWORD GetMinFiltration() { return m_minFiltration; }
   inline DWORD GetMagFiltration() { return m_magFiltration; }
   inline float GetMipmapBias() { return m_mipmapBias; }
   inline bool GetZTestUsage() { return m_zTest; }
private:
   void initService();
   void mountData();
   void drawMultiHouse();
   void drawMultiGround();
   void createHouseLights();
   void loadObjectScene();
   void drawGround();
   void createGround();
   void mountVertices();
   void createLights();
   inline void toggleTexturing();
   inline void toggleZTestUsage();
   void toggleMipFiltration() { m_filtrationType = (m_filtrationType == D3DTEXF_LINEAR) ? D3DTEXF_POINT : (m_filtrationType == D3DTEXF_POINT) ? D3DTEXF_NONE : D3DTEXF_LINEAR; }
   void toggleMagFiltration() { m_magFiltration = (m_magFiltration == D3DTEXF_LINEAR) ? D3DTEXF_POINT : D3DTEXF_LINEAR; }
   void toggleMinFiltration() { m_minFiltration = (m_minFiltration == D3DTEXF_LINEAR) ? D3DTEXF_POINT : D3DTEXF_LINEAR; }
   inline void setMipFiltration() { m_d3dd->SetSamplerState(0, D3DSAMP_MIPFILTER, m_filtrationType); }
   inline void setMagFiltration() { m_d3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, m_magFiltration); }
   inline void setMinFiltration() { m_d3dd->SetSamplerState(0, D3DSAMP_MINFILTER, m_minFiltration); }
   inline void setMipmapBias() { m_d3dd->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *((DWORD *)&m_mipmapBias)); }

   static LPDIRECT3DDEVICE9 getD3Dd(const DirectXApp &m_a) { return (m_a.m_pD3D)->getDevice(); }
private:
   bool m_renderBlock;
   static DWORD LIGHT_COUNT;

   int   m_prevMouseX;
   int   m_prevMouseY;
   bool  m_keysPressed[MAX_KEYS];

   unsigned int m_indexCount, m_vertexCount, m_primCount, m_vFlags;
   LPDIRECT3DDEVICE9 m_d3dd;
   IDirect3DVertexBuffer9* m_vertexBuf;

   float m_mipmapBias;
   DWORD m_filtrationType, m_magFiltration, m_minFiltration;
   DWORD m_cameraMod;

   bool m_zTest;

   std::shared_ptr<Engine> m_engine;
   std::shared_ptr<OrderManager> m_orderManager;
   std::shared_ptr<Camera> m_camera;
   std::shared_ptr<Infoboard> m_info;
   std::shared_ptr<CarSystem> m_car;
   std::shared_ptr<Light> m_light;
   std::shared_ptr<Environment> m_environment;
   std::shared_ptr<ShaderUnit> m_shaderunit;
   std::shared_ptr<ConnectionManager> m_connectionManager;

   bool m_texturing;

   LPCWSTR m_hMeshFileName, m_cMeshFileName, m_gMeshFileName;

   bool m_dLightEnabled, m_sLightEnabled, m_pLightEnabled;
};





// *******************************************************************
// functions 
