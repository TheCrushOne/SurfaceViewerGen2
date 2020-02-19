//-----------------------------------------------------------------------------
// XD3D.h: Custom CD3DApplication (CXD3D) header file
//-----------------------------------------------------------------------------
#ifndef XD3D_H
#define XD3D_H

#include "XD3DEnum.h"
#include "XD3DSettings.h"

//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
enum APPMSGTYPE { MSG_NONE, MSGERR_CANNOTCONTINUE, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d
#define D3DAPPERR_INITFAILED	      0x8200000e

//-----------------------------------------------------------------------------
// Utility macros
//-----------------------------------------------------------------------------
// Courtesy of Game Programming Gems 3, Topic 1.3, macro trick #5
#define UBOUND(a) (sizeof(a) / sizeof((a)[0]))

// scale a bool to -1|+1 and save a ?:
#define BSCALE(b) ((b << 1) - 1)


//-----------------------------------------------------------------------------
// CXD3D class: the class a view class will derive from to provide a window
// handle to render into, and that will override the 3D scene rendering.
//-----------------------------------------------------------------------------
class CXD3D
{
protected:
	// internal state variables

	bool m_bActive;				// toggled on Pause, can be queried upon
								// initializing to issue a Create [false]
	
	bool m_bStartFullscreen;	// queried on ChooseInitialSettings [false]
	
	bool m_bShowCursor;			// in fullscreen mode [true]
	bool m_bClipCursor;			// in fullscreen mode [true]

	bool m_bWindowed;			// queried on BuildPresentParamsFromSettings
								// [true]
	bool m_bIgnoreSizeChange;	// queried on HandlePossibleSizeChange [false]
		
	bool m_bDeviceLost;				// true when the device's Present fails
	bool m_bDeviceObjectsInited;	// true if InitDeviceObjects succeeds
	bool m_bDeviceObjectsRestored;	// true if RestoreDeviceObjects succeeds
	
	// internal timing variables

	FLOAT m_fTime;					// absolute time handled by DXUtil_Timer
	FLOAT m_fElapsedTime;			// elapsed time handled by DXUtil_Timer
	FLOAT m_fFPS;					// the frame rate, or frames per second
	
	// statistics

	TCHAR m_strDeviceStats[256];		// device description
	TCHAR m_strFrameStats[16];			// frame statistics
	
	// main objects used for creating and rendering the 3D scene

	HWND					m_hWndRender;		// device window
	HWND					m_hWndFocus;		// focus window

	LPDIRECT3D9				m_pd3d;				// main D3D object
	LPDIRECT3DDEVICE9		m_pd3dDevice;		// D3D rendering device
	D3DPRESENT_PARAMETERS	m_d3dpp;			// presentation parameters
	
	DWORD					m_dwCreateFlags;	// sw/hw VP + pure device

	DWORD					m_dwWindowStyle;	// saved for mode switches
	
	RECT					m_rcWindow;			// window and client rects,
	RECT					m_rcClient;			// saved for mode switches
	
	// setup objects
	CXD3DEnum Enumeration;		// hierarchy of adapters, modes, devices, etc.
	CXD3DSettings Settings;		// current display settings

protected:
	// internal error handling function

	HRESULT DisplayErrorMsg(HRESULT hr, DWORD dwType);
	
	// internal management functions

	void	BuildPresentParamsFromSettings();
	bool	FindBestWindowedMode(bool bHAL, bool bREF);
	bool	FindBestFullscreenMode(bool bHAL, bool bREF);
	HRESULT ChooseInitialSettings();    
	HRESULT InitializeEnvironment();
	HRESULT ResetEnvironment();
	void    CleanupEnvironment();

public:
	HRESULT RenderEnvironment();
	HRESULT HandlePossibleSizeChange();

protected:
	void	UpdateStats();
	
	// Overridable functions for the 3D scene created by the app

	virtual HRESULT OneTimeSceneInit()			{ return S_OK; }
	virtual HRESULT InitDeviceObjects()			{ return S_OK; }
	virtual HRESULT RestoreDeviceObjects()		{ return S_OK; }
	virtual HRESULT FrameMove()					{ return S_OK; }

	virtual HRESULT InvalidateDeviceObjects()	{ return S_OK; }
	virtual HRESULT DeleteDeviceObjects()		{ return S_OK; }
	virtual HRESULT FinalCleanup()				{ return S_OK; }
	
public:
	virtual HRESULT Render()					{ return S_OK; }

	// construct/destruct, create and pause/play

	CXD3D();
	virtual ~CXD3D() { }
	virtual HRESULT CreateD3D();
	virtual void Pause(bool bPause);

	// active state wrapper
	bool IsActive() { return m_bActive; };

	// device and frame statistics wrappers
	LPCTSTR GetDeviceStats() { return m_strDeviceStats; }
	LPCTSTR GetFrameStats()  { return m_strFrameStats; }
};

#endif
