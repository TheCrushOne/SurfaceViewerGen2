/**
  @file     myApp.cpp
  @brief    User's application class
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <zmouse.h>

#include "Library/cglD3D.h"
#include "directxapp.h"

#include "engine/engine.h"
#include "engine/subsystems/ordermanager.h"
#include "environment\envlights.h"
#include "utils/light.h"
#include "utils\camera.h"
#include "environment\environment.h"
#include "car_model\carsystem.h"
#include "engine\shader_processing\shaderunit.h"
#include "utils\infoboard.h"

DWORD DirectXApp::LIGHT_COUNT = 10;
//8 - car; 1 - moomlight; 1 - lantern

// *******************************************************************

namespace
{
   const float s_rMouseMove2Rotate = 0.001f;
   const float s_rMouseWheel2Zoom = 0.001f;
   const float s_rKbd2Rotate = 1.16f;
   const float s_rKbd2Zoom = 1.16f;

   void shiftColor(unsigned &color, float delta, int shift)
   {
      int nComp = (color >> shift) & 0xFF;
      nComp += int(delta * 255.0f);
      if (nComp > 0xFF) 
         nComp = 0xFF;
      else if (nComp < 0)
         nComp = 0;
      color &= ~(0xFF << shift);
      color |= nComp << shift;
   }
}


// *******************************************************************

DirectXApp::~DirectXApp()
{}

void DirectXApp::initService()
{
   m_renderBlock = false;
   //В этом классе стоит навести порядок....(not fixed)
   m_d3dd = getD3Dd(*this);
   m_engine = std::make_shared<Engine>(this, m_d3dd, m_wWidth, m_wHeight);
   m_engine->CreateOrderManager();        //вот тут нафиг не нужен птр на д3дд
   m_engine->GetOrderManager()->InvokeRegistration();
   m_engine->CreateLightDispatcher(m_d3dd, LIGHT_COUNT);
   m_engine->CreateShaderunit(m_d3dd);
   m_engine->CreateEnvironment(m_d3dd);
   m_engine->CreateCarsystem(m_d3dd, L"res/models/Lamborghini.x", L"res/custom/Eightway.wpd", 9);
   m_engine->CreateCamera(m_d3dd);
   m_engine->CreateInfoboard(oD3D9, m_d3dd);
   m_engine->CreateConnectionManager();
   m_engine->GetOrderManager()->WithdrawRegistration();

   m_shaderunit = m_engine->GetShader();
   m_orderManager = m_engine->GetOrderManager();
   m_car = m_engine->GetCar();
   m_camera = m_engine->GetCamera();
   m_info = m_engine->GetInfoboard();
   m_light = m_engine->GetLight();
   m_environment = m_engine->GetEnvironment();
   m_connectionManager = m_engine->GetConnectionManager();

   //под фильтрацию
   m_d3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
   m_d3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
   m_d3dd->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
   //sta_d3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADD);
   //   
   m_d3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
   //sta_d3dd->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
   //sta_d3dd->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
   //настройка частично произведена шейдером
   m_d3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   //sta_d3dd->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
   m_d3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   m_d3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   //sta_d3dd->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
   //sta_d3dd->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
   //sta_d3dd->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
   m_d3dd->SetRenderState(D3DRS_LIGHTING, true);                  //fill без света
   //sta_d3dd->SetRenderState(D3DRS_DITHERENABLE, true); //не факт, что сглаживание поможет
    
   //sta_d3dd->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
   //sta_d3dd->SetRenderState(D3DRS_ALPHAREF, (DWORD)10000);
   //sta_d3dd->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
   //sta_d3dd->SetRenderState(D3DRS_STENCILENABLE, true);
   //sta_d3dd->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
   //sta_d3dd->SetRenderState(D3DRS_POINTSCALEENABLE, true);
   //sta_d3dd->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


   m_d3dd->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
   m_d3dd->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);        //по умолчанию вроде та же фукнция

   //sta_d3dd->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(80, 80, 80)); //аварийный свет

   /*L"17755_Audi_R8_ASCII.x"*/// - резерв(слишком большой масштаб)
   m_hMeshFileName = L"res/models/fw43_lowpoly_n1.x";
   m_cMeshFileName = /*L"17755_Audi_R8_ASCII.x";*/L"res/models/Lamborghini.x";
   m_gMeshFileName = /*L"39pr.png"*/L"res/models/tera_2.x"/*L"tera.x"*//*L"door35.jpg"*/;

   m_filtrationType = D3DTEXF_LINEAR;
   m_minFiltration = D3DTEXF_LINEAR;
   m_magFiltration = D3DTEXF_LINEAR;
   setMipFiltration();
   setMagFiltration();
   setMinFiltration();

   m_mipmapBias = 0.0f;

   m_zTest = false;

   m_vertexBuf = 0;
   //m_nClearColor = 0x00000000;     //пока что общий фон черный
   m_nClearColor = m_engine->GetEnvlight()->GetBackgroundColor();

   m_dLightEnabled = true;
   m_light->SetEnabled(OBL_MOONLIGHT, m_dLightEnabled);
    
   m_pLightEnabled = true;
   m_light->SetEnabled(OBL_LANTERNLIGHT, m_pLightEnabled);
   //sta_texturing = STA_DEFAULT_TEXTURING;
   //sta_car->setTexturing(sta_texturing);

   //eyeX = 1000.6f, eyeY = 1900.4f, eyeZ = 1000.6f, LookatX = 0.0f, LookatY = 0.0f, LookatZ = 0.0f;
   //eyeX = 17.6f, eyeY = 32.4f, eyeZ = -34.6f, LookatX = 0.0f, LookatY = 0.0f, LookatZ = 0.0f;

   for (int i = 0; i < MAX_KEYS; i++)
      m_keysPressed[i] = false;

   //mountData();
}
//==============================

bool DirectXApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
   float dx = 0.0f;
   float dy = 0.0f;
   float dz = 0.0f;

   switch (nMsg)
   {
   case WM_MASTER_RELEASE:
      m_renderBlock = true;
      m_engine->CascadeRelease();
      break;
   case WM_MASTER_RECREATE:
      m_engine->CascadeRecreate(wParam, lParam);
      m_renderBlock = false;
      break;
   default:
      break;
   }

   if (m_renderBlock)
      return cglApp::processInput(nMsg, wParam, lParam);
   // Handle input
   switch (nMsg)
   {
      // Process mouse move message
      case WM_MOUSEMOVE:
      {
         // Get mouse x & y (to the upper-left corner of the client area)
         int xPos = GET_X_LPARAM(lParam);
         int yPos = GET_Y_LPARAM(lParam);

         if ((wParam & MK_LBUTTON) != 0 && m_prevMouseX >= 0) // 
         {
            dx = s_rMouseMove2Rotate*(xPos - m_prevMouseX);
            dy = s_rMouseMove2Rotate*(yPos - m_prevMouseY);
         }

         m_prevMouseX = xPos;
         m_prevMouseY = yPos;

         m_camera->CtrlMouseMove(dx, dy);

         break;
      }
      case WM_MOUSEWHEEL:
      {
         int zDelta = (int)((signed short)(HIWORD(wParam)));
         dz = zDelta * s_rMouseWheel2Zoom;
         m_camera->CtrlZoom(dz);
         break;
      }
      case WM_KEYDOWN:
      {
         if (wParam < MAX_KEYS)
            m_keysPressed[wParam] = true;
         break;
      }
      case WM_KEYUP:
      {
         if (wParam < MAX_KEYS)
            m_keysPressed[wParam] = false;
         processVKInput(wParam);
         break;
      }
    }
  
    return cglApp::processInput(nMsg, wParam, lParam);
}

// Бинды
bool DirectXApp::processVKInput(int wParam)
{
   switch (wParam)
   {
   case 0x31:  //(vk 1)
      m_environment->DLightRedirect();
      m_nClearColor = m_engine->GetEnvlight()->GetBackgroundColor();
      break;
   case 0x32:  //(vk 2)
      m_car->CarHMLightRedirect(m_keysPressed[VK_CONTROL]);
      break;
   case 0x33:  //(vk 3)
      m_environment->PLightRedirect();
      break;
   case 0x34:  //(vk 4)
      m_car->CarNeonRedirect();
      break;
   case VK_OEM_4://(vk [)
      m_car->CarSignalRedirect(DBC_LEFT);
      break;
   case VK_OEM_6://(vk ])
      m_car->CarSignalRedirect(DBC_RIGHT);
      break;
   case VK_OEM_MINUS:
      m_car->CarSignalRedirect(DBC_WARNING);
      break;
   case 0x4D:	//(vk M)
      toggleMipFiltration();
      setMipFiltration();
      break;
   case 0x6B:	//(vk +)
      if (m_keysPressed[VK_SHIFT])
      {
         m_mipmapBias += 0.2f;
         setMipmapBias();
      }
      break;
   case 0x6D:	//(vk -)
      if (m_keysPressed[VK_SHIFT])
      {
         m_mipmapBias -= 0.2f;
         setMipmapBias();
      }
      break;
   case VK_SPACE/*0x43*/:  //(vk Space)
      m_camera->ToggleCameraMoveType();
      break;
   case 0x54:  //(vk T)
      toggleTexturing();
      //sta_car->setTexturing(sta_texturing);
      break;
   case VK_F1: //(vk F1)
      m_camera->ToggleCameraCollisionMod();
      break;
   case VK_F2: //(vk F2)
      m_camera->ToggleRenderStateFillMode();
      break;
   case 0x48:  //(vk H)
      m_info->ToggleShowMainInfo();
      break;
   case 0x4B:  //(vk K)
      m_info->ToggleShowDbgInfo();
      break;
   case 0x49:  //(vk I)
      m_info->ToggleShowServiceInfo();
      break;
   case 0x4A:  //(vk J)
      m_info->ToggleShowHUD();
      break;
   case 0x51:  //(vk Q)
      m_camera->ChangeMovementSpeed(Camera::SPEED_CHANGE_STEP);
      break;
   case 0x45:  //(vk E)
      m_camera->ChangeMovementSpeed(-Camera::SPEED_CHANGE_STEP);
      break;
   case 0x46:  //(vk F)
      toggleMinFiltration();
      setMinFiltration();
      break;
   case 0x47:  //(vk G)
      toggleMagFiltration();
      setMagFiltration();
      break;
   case 0x53:  //(vk S)    //опасно!
      if (m_camera->GetCameraMoveType() == VBC_CM_ORBITAL)
         m_orderManager->ToggleRenderType();
      break;
   case 0x58:  //(vk X)
      m_car->CarTransmissionRedirect(WBC_UP);
      break;
   case 0x5A:  //(vk Z)
      m_car->CarTransmissionRedirect(WBC_DOWN);
      break;
   case VK_F3:	//(vk F3)
      m_engine->ToggleShaderUsage();
      break;
   case VK_F4:	//(vk F4)
      m_shaderunit->ToggleLightSphereDraw();
      break;
   case VK_F5:	//(vk F5)
      toggleZTestUsage();
      break;
   case VK_F6:	//(vk F6)   //дублировано на случай другого типа камеры
      m_orderManager->ToggleRenderType();
      break;
   case VK_F7: //(vk F7)
      m_engine->StartWTransform();
      break;
   case VK_F12:
      if (m_keysPressed[VK_CONTROL])
         m_car->SwitchUserControlRedirect();
      break;
   }
   return true;
}

// Обработка поступающих сигналов с ув
void DirectXApp::update()
{
    // Call predecessor update
    cglApp::update();
  
    if (m_renderBlock)
        return;
    // Process keyboard
    static float dx = 0.0f;
    static float dy = 0.0f;
    static float dr = 0.0f;
    static float adx = 0.0f;
    static float ady = 0.0f;

    dx = dy = dr = adx = ady = 0.0f;

    if (m_keysPressed[VK_LEFT])
        dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_RIGHT])
        dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_UP])
        dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_DOWN])
        dy += s_rKbd2Rotate * m_timer.getDelta();
    if ((m_keysPressed[VK_SUBTRACT])
        && (!m_keysPressed[VK_SHIFT]))
        dr -= s_rKbd2Zoom * m_timer.getDelta();
    if ((m_keysPressed[VK_ADD])
        && (!m_keysPressed[VK_SHIFT]))
        dr += s_rKbd2Zoom * m_timer.getDelta();

    //ктр.
    if (m_camera->GetCameraMoveType() == VBC_CM_FOLLOW)
    {
        if (m_car->GetWPType() == WBC_USERCONTROLLED)
        {
            if (m_keysPressed[VK_NUMPAD4])  //(vk numpad 4)
                adx -= s_rKbd2Rotate * m_timer.getDelta();
            if (m_keysPressed[VK_NUMPAD6])  //(vk numpad 6)
                adx += s_rKbd2Rotate * m_timer.getDelta();
            if (m_keysPressed[VK_NUMPAD8])  //(vk numpad 8)
                ady -= s_rKbd2Rotate * m_timer.getDelta();
            if (m_keysPressed[VK_NUMPAD2])  //(vk numpad 2)
                ady += s_rKbd2Rotate * m_timer.getDelta();
        }
      

        if (m_keysPressed[0x41])    //(vk A)
            dx -= s_rKbd2Rotate * m_timer.getDelta();
        if (m_keysPressed[0x44])    //(vk D)
            dx += s_rKbd2Rotate * m_timer.getDelta();
        if (m_keysPressed[0x57])   //(vk W)
            dy -= s_rKbd2Rotate * m_timer.getDelta();
        if (m_keysPressed[0x53]) //(vk S)
            dy += s_rKbd2Rotate * m_timer.getDelta();
    }

    if (m_car->GetWPType() == WBC_USERCONTROLLED)
    {
        m_camera->CtrlForwardSide(adx, ady);
        m_car->MoveRedirect(dx, dy);
    }
    else
      m_camera->CtrlForwardSide(dx, dy);
    
    m_camera->CtrlZoom(dr);
}

// Функция рендера
void DirectXApp::renderInternal()
{
    if (m_renderBlock)
        return;
    //общая схема(какая-то путаница возникла...зачем вообще DirectXApp нужен, если Engine есть?...)
    m_engine->CascadeUpdate();
    m_engine->CascadeRender();
}

// Функция переключения тумблера текстурирования
inline void DirectXApp::toggleTexturing()
{
    m_texturing = !m_texturing;
    m_engine->GetEnvironment()->ToggleTexturing(m_texturing);
    m_engine->GetCar()->ToggleTexturing(m_texturing);
}

// Функция переключения тумблера з-теста
inline void DirectXApp::toggleZTestUsage()
{
	m_zTest = !m_zTest;	
	m_d3dd->SetRenderState(D3DRS_ZENABLE, m_zTest);
}