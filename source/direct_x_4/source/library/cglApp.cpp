/**
  @file     cglapp.cpp
  @brief    Class cglApp definition
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <string>

#include "cglD3D.h"

#include "cglApp.h"



// *******************************************************************
// defines



// *******************************************************************
// local data

namespace
{
  const wchar_t * s_windowClassName = L"D3DBase9";
  const float  s_fpsMeasurementTime = 5.0f;

}

// *******************************************************************
// functions

LRESULT CALLBACK D3DBaseAppCallback(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   // Retrieve our object pointer
   cglApp *pApp = (cglApp*)(GetWindowLong(hWnd, 0));
  
   if (pApp != NULL && pApp->processInput(nMsg, wParam, lParam))
      return 0;
   else
      return DefWindowProc(hWnd, nMsg, wParam, lParam); 
} 

// *******************************************************************
// Methods

cglApp::cglApp(int nW, int nH, void* hInst, int nCmdShow) 
   : m_hWnd(NULL)
   , m_hInstance(hInst)
   , m_nClearColor(0xFF007F00)
   , m_pD3D(NULL)
   , m_nFrameCount(0)
   , m_rPrevTime(0.0f)
{
   // Register window class
   WNDCLASS wndClass;
   wndClass.style          = 0;
   wndClass.lpfnWndProc    = D3DBaseAppCallback;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 4;
   wndClass.hInstance      = HINSTANCE(hInst);
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = HBRUSH(GetStockObject(WHITE_BRUSH));
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = s_windowClassName;
   RegisterClass(&wndClass);

   // Adjust window in regard to client area nW x nH
   int  nStyle = WS_OVERLAPPED | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
   RECT rRect;
   SetRect(&rRect, 0, 0, nW, nH);
   AdjustWindowRect(&rRect, nStyle, TRUE);
  
   // Create main window
   m_hWnd = NULL;
   m_hWnd = (void*)CreateWindow(s_windowClassName, getWindowText(), nStyle,
                                 CW_USEDEFAULT, CW_USEDEFAULT, 
                                 (rRect.right - rRect.left), (rRect.bottom - rRect.top),
                                 NULL, NULL, HINSTANCE(hInst), NULL);
   if (m_hWnd == NULL)
      return;
    
   // Set pointer
   SetWindowLong(HWND(m_hWnd), 0, LONG(this));
   SetWindowLong(HWND(m_hWnd), GWL_STYLE, WS_POPUP);
   SetWindowLong(HWND(m_hWnd), GWL_EXSTYLE, WS_EX_TOPMOST);

   // Show window
   //ShowWindow(HWND(m_hWnd), nCmdShow);
   ShowWindow(HWND(m_hWnd), SW_SHOWMAXIMIZED);
   UpdateWindow(HWND(m_hWnd));
  
   // We need to determine the BPP of desktop
   HDC hDC = GetDC(HWND(m_hWnd));            // Get DC of desktop
   int nBPP = GetDeviceCaps(hDC, BITSPIXEL); // Retrieve BPP
   ReleaseDC(HWND(m_hWnd), hDC);             // Release DC handle

   memset(&m_params, 0, sizeof(D3DPRESENT_PARAMETERS));
   m_isFullScreen = true;
   //m_isFullScreen = false;

   // Create our D3D class
   cglD3D::CreateParams params;
   params.hWnd    = m_hWnd;
   params.nBPP    = (nBPP == 32) ? cglD3D::BPP_32 : cglD3D::BPP_16;
   v_nBPP = params.nBPP;
   params.nWidth  = nW;
   params.nHeight = nH;
   m_wHeight = nH;
   m_wWidth = nW;
   m_pD3D = new cglD3D(params);
  
   oD3D9 = m_pD3D->getD3D9();
   // Check creation result
   if (m_pD3D == NULL || m_pD3D->isFailed())
      return;

   // Init random generator
   srand(0);
}

cglApp::~cglApp()
{
  // Kill D3D
  if (m_pD3D != NULL)
    delete m_pD3D;
  // Kill window
  if (m_hWnd != NULL)
    DestroyWindow(HWND(m_hWnd));
} 

void cglApp::theLoop()
{
  while (TRUE)
  {
    MSG nMsg;
    if (PeekMessage(&nMsg, NULL, 0U, 0U, PM_REMOVE))
    {
      if (nMsg.message == WM_QUIT)
        break;

      TranslateMessage(&nMsg);
      DispatchMessage(&nMsg);
    }
    else
    {
      update();
      render();
    } // end if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
  } // end while (TRUE)
}

bool cglApp::isFailed()
{
  return (m_hWnd == NULL /*|| m_pD3D == NULL*/);
}

bool cglApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
  // Handle input
  switch (nMsg)
  {
  case WM_MASTER_HALT:
  case WM_CLOSE:
    PostQuitMessage(0);
    return true;
  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)  //тестовая версия
        toggleFullScreen();
    break;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
    {
      PostQuitMessage(0);
      return true;
    }
    break;
  }
  
  return false;
}

void cglApp::update(void)
{
  m_timer.update();
  float rTime = m_timer.getTime() - m_rPrevTime;
  if (rTime > s_fpsMeasurementTime)
  {
    // Calculate FPS
    float rFPS = float(m_nFrameCount) / rTime;
	
    // Show fps
    //char fpsString[50];
    //sprintf_s(fpsString, "%s FPS = %3.6f", getWindowText(), rFPS);
    std::wostringstream fpsStream;
    fpsStream << getWindowText();
    fpsStream << L"FPS = " << std::setprecision(1) << rFPS;
    
    SetWindowText(HWND(m_hWnd), fpsStream.str().c_str());
    // Drop
    m_rPrevTime   = m_timer.getTime();
    m_nFrameCount = 0;
  }
  m_nFrameCount++;
} 

void cglApp::render()
{
  if (m_pD3D->beginRender())
  {
    // Clear screen with current color
    m_pD3D->clear(m_nClearColor);
    
    // Perform rendering internals
    renderInternal();
    
    m_pD3D->endRender();
  } 
} 


void cglApp::toggleFullScreen()
{ 
    static DWORD SavedWindowedWidth;        // Saved windowed width
    static DWORD SavedWindowedHeight;       // Saved windowed height
    static HMENU hMenu;                    // Handle to store menu between toggles
    static RECT ClientRect;                // Saved client rectangle
    static RECT WindowRect;                // Windowed windows rectangle
    static DWORD FullscreenWidth;           // Fullscreen width
    static DWORD FullscreenHeight;          // Fullscreen height
	
    
    static long WindowStyle = WS_OVERLAPPED | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;            // Windowed window style

    // Are we in windowed mode at the moment
    if (!m_isFullScreen)
    {
        // Set the fullscreen mode style, clear menu if attached
        SetWindowLong(HWND(m_hWnd), GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
        if (hMenu == NULL)
        {
            hMenu = GetMenu(HWND(m_hWnd));
            SetMenu(HWND(m_hWnd), NULL);
        }
		
		
		

		
        GetClientRect(HWND(m_hWnd), &ClientRect);
        GetWindowRect(HWND(m_hWnd), &WindowRect);
        // Save the windowed client dimensions
        SavedWindowedWidth = ClientRect.right - ClientRect.left;
        SavedWindowedHeight = ClientRect.bottom - ClientRect.top;

        FullscreenWidth = GetSystemMetrics(SM_CXSCREEN);
        FullscreenHeight = GetSystemMetrics(SM_CYSCREEN);

        m_params.hDeviceWindow = HWND(m_hWnd);
        // Set the Presentation Parameters, specifically
        m_params.Windowed = FALSE;
        m_params.BackBufferWidth = FullscreenWidth;
        m_params.BackBufferHeight = FullscreenHeight;

        // Other params ... upto you!!!

        // Reset D3D device, any device dependent objects
        // that aren't managed or in system memory have
        // to pe released before you call Reset and recreated
        // after the reset, ditto for going back to windowed
        // mode. You also have to reset render states and matrices
        // after the call to Reset.
        SendMessage(HWND(m_hWnd), WM_MASTER_RELEASE, 0, 0);
        (m_pD3D->ResetDevice(m_params, v_nBPP));
        SendMessage(HWND(m_hWnd), WM_MASTER_RECREATE, static_cast<int>(FullscreenWidth), static_cast<int>(FullscreenHeight));
        m_isFullScreen = true;
    }
    // We're in fullscreen mode at the moment
    else
    {
        // Set the windowed mode style, reset menu if needed
		SetWindowLong(HWND(m_hWnd), 0, LONG(this));
        if (hMenu != NULL)
        {
            SetMenu(HWND(m_hWnd), hMenu);
            hMenu = NULL;
        }

        m_params.hDeviceWindow = HWND(m_hWnd);
        // Set the Presentation Parameters, specifically
        m_params.Windowed = TRUE;
        m_params.BackBufferWidth = SavedWindowedWidth;
        m_params.BackBufferHeight = SavedWindowedHeight;

        // Other params ... upto you!!!

        // Reset D3D device
        SendMessage(HWND(m_hWnd), WM_MASTER_RELEASE, 0, 0);
        m_pD3D->ResetDevice(m_params, v_nBPP);
        SendMessage(HWND(m_hWnd), WM_MASTER_RECREATE, static_cast<int>(SavedWindowedWidth), static_cast<int>(SavedWindowedHeight));

        // Set the window position
        SetWindowPos(HWND(m_hWnd), HWND_NOTOPMOST,
            WindowRect.left, WindowRect.top,
            (WindowRect.right - WindowRect.left),
            (WindowRect.bottom - WindowRect.top),
            SWP_SHOWWINDOW);

        m_isFullScreen = false;
    }

    // Get new client rectangle    
    UpdateWindow(HWND(m_hWnd));
      

    //onLostDeviceCascade();
    //m_pD3D->ResetDevice(m_params);
    //onResetDeviceCascade();
}