#ifndef __CGLAPP_H__632619789338281250
#define __CGLAPP_H__632619789338281250

/**
  @file     cglapp.h
  @brief    Class cglApp definition
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes
#include "cglTimer.h"
#include <d3d9.h>

// *******************************************************************
// defines & constants

#define WM_MASTER_RELEASE 0x6212
#define WM_MASTER_RECREATE 0x6213

#define WM_MASTER_HALT 0x4102

// *******************************************************************
// classes 

// Application class
class cglApp
{
public:
  // Constructor
  cglApp(int nW, int nH, void* hInst, int nCmdShow);
  // Destructor
  virtual ~cglApp();
  // Allows to check if constructor failed
  bool isFailed(); 
  // Contains main application loop
  void theLoop();
  // This function performs input processing. Returns true if input is handled
  virtual bool processInput(unsigned int nMsg, int wParam, long lParam);
protected:
  //
  virtual wchar_t const *getWindowText() { return L"D3D Simple World Processing"; }
  // This function should contain all update issues
  virtual void update();
  // This function should contain all render issues
  virtual void renderInternal() {}
  // Perform rendering
  void render(); 

  // Window handle. void* is used to not include <windows.h>
  void*        m_hWnd;
  // Application instance. void* is used for the same purpose
  void*        m_hInstance;
  // Color to clear the back buffer
  unsigned int m_nClearColor;
  // D3D wrapper class
  class cglD3D *m_pD3D;
  // Timer
  cglTimer     m_timer;
  //DIRECT3D9 outside(added by Crush)  
  LPDIRECT3D9 oD3D9;
  // For fps counting
  int          m_nFrameCount;
  float        m_rPrevTime;

  int v_nBPP;

  bool m_isFullScreen;
  void toggleFullScreen();
  DWORD m_wWidth;
  DWORD m_wHeight;

  D3DPRESENT_PARAMETERS        m_params;
  virtual void onLostDeviceCascade() {}
  virtual void onResetDeviceCascade() {}
};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789338281250
