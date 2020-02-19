//-----------------------------------------------------------------------------
// XD3D.cpp: custom Direct3D (CXD3D) Implementation file
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "XD3D.h"

//-----------------------------------------------------------------------------
// CXD3D Constructor
//-----------------------------------------------------------------------------
CXD3D::CXD3D()
{
	m_bActive					= false;
	
	m_bStartFullscreen			= false;    
	
	m_bShowCursor = true;
	m_bClipCursor = true;
	
	m_bWindowed					= true;
	m_bIgnoreSizeChange			= false;
	
	m_bDeviceLost				= false;
	m_bDeviceObjectsInited		= false;
	m_bDeviceObjectsRestored	= false;
	
	m_fTime			= 0.0f;
	m_fElapsedTime	= 0.0f;
	m_fFPS			= 0.0f;
	
	m_strDeviceStats[0] = _T('\0');
	m_strFrameStats[0]  = _T('\0');

	m_hWndRender        = NULL;
	m_hWndFocus         = NULL;
	
	m_pd3d              = NULL;
	m_pd3dDevice        = NULL;
	
	m_dwCreateFlags     = 0L;
	
	Pause(true); // until we're ready to render
}

//-----------------------------------------------------------------------------
// CreateD3D(): provided m_hWnd has been initialised, it instantiates the d3d
// object, chooses initial d3d settings and initializes the d3d stuff.
//-----------------------------------------------------------------------------
HRESULT CXD3D::CreateD3D()
{		
	HRESULT hr;

	// check for a window to render to
    if (m_hWndRender == NULL)
        return DisplayErrorMsg(D3DAPPERR_NOWINDOW, MSGERR_CANNOTCONTINUE);
	
	// instantiate a D3D Object
    if ((m_pd3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return DisplayErrorMsg(D3DAPPERR_NODIRECT3D, MSGERR_CANNOTCONTINUE);

    // build a list of D3D adapters, modes and devices
	if (FAILED(hr = Enumeration.Enumerate(m_pd3d)))
	{
        SAFE_RELEASE(m_pd3d);
        
		return DisplayErrorMsg(hr, MSGERR_CANNOTCONTINUE);
    }

    // use the device window as the focus window, unless otherwise specified
    if (m_hWndFocus == NULL) m_hWndFocus = m_hWndRender;

    // save some window properties into class members
    m_dwWindowStyle = GetWindowLong(m_hWndRender, GWL_STYLE);
    
	GetWindowRect(m_hWndRender, &m_rcWindow);
   GetClientRect(m_hWndRender, &m_rcClient);

    // choose the best settings to render
	if (FAILED(hr = ChooseInitialSettings()))
	{
        SAFE_RELEASE(m_pd3d);
    
		return DisplayErrorMsg(hr, MSGERR_CANNOTCONTINUE);
    }

    // initialize the timer
    DXUtil_Timer(TIMER_START);

    // initialize the app's custom (pre-device creation) stuff
    if (FAILED(hr = OneTimeSceneInit()))
    {
        SAFE_RELEASE(m_pd3d);
        
		return DisplayErrorMsg(hr, MSGERR_CANNOTCONTINUE);
    }

    // initialize the 3D environment, creating the device
    if (FAILED(hr = InitializeEnvironment()))
	{
		SAFE_RELEASE(m_pd3d);
        
		return DisplayErrorMsg(hr, MSGERR_CANNOTCONTINUE);
    }

    // D3D is ready to go so unpause it
    Pause(false);

    return S_OK;
}

//-----------------------------------------------------------------------------
// FindBestFullscreenMode(): Adjust settings with the best available fullscreen
// mode, subject to the HAL and REF constraints; returns false if no such mode
// can be found.
//-----------------------------------------------------------------------------
bool CXD3D::FindBestFullscreenMode(bool bHAL, bool bREF)
{
	// for fullscreen, default to the first HAL device combo that supports the
	// current desktop display mode, to any display mode if HAL is incompatible
	// with the desktop mode, or to a non-HAL if no HAL is available
	D3DDISPLAYMODE dmDesktop;
	D3DDISPLAYMODE dmDesktopBest;
	D3DDISPLAYMODE dmBest;
	
	// fortunately for us, D3DFMT_UNKNOWN == 0
	ZeroMemory(&dmDesktopBest, sizeof(D3DDISPLAYMODE));
	ZeroMemory(&dmBest, sizeof(D3DDISPLAYMODE));
	
	// 'best' storage
	AdapterInfo* paiBest = NULL;
	DeviceInfo*  pdiBest = NULL;
	DeviceCombo* pdcBest = NULL;

	// iterators
	AdapterInfo* pai;
	DeviceInfo*  pdi;
	DeviceCombo* pdc;
				
	// success flags
	bool bBetter, bBest;

	UINT i, j, k;

	// traverse the adpater infos
	for (i = 0; i < Enumeration.AdapterInfos.Length(); i++)
	{
		pai = &Enumeration.AdapterInfos[i];
		
		// get the current display mode of each adapter
		m_pd3d->GetAdapterDisplayMode(pai->AdapterOrdinal, &dmDesktop);
		
		// traverse device infos on each adapter info
		for (j = 0; j < pai->DeviceInfos.Length(); j++)
		{			
			pdi = &pai->DeviceInfos[j];
			
			// skip devices with other than the requested type
			if (bHAL && pdi->DevType != D3DDEVTYPE_HAL)
				continue;
			
			if (bREF && pdi->DevType != D3DDEVTYPE_REF)
				continue;
			
			// traverse device combos for each device info
			for (k = 0; k < pdi->DeviceCombos.Length(); k++)
			{
				pdc = &pdi->DeviceCombos[k];
				
				// skip the windowed combos
				if (pdc->Windowed)
					continue;
				
				// this device combo is 'better' than the current best if:
				//  (a) there's no best yet;
				//  (b) it's a HAL and the current best is not;
				//  (c) it's a HAL matching the desktop's format, while the
				//      current best does not;
				//  (d) it's a HAL and both the display and backbuffer formats
				//      match the desktop's, in which case it is also the best

				bBetter = pdcBest == NULL ||
						  
						  pdc->DevType     == D3DDEVTYPE_HAL &&
						  pdcBest->DevType != D3DDEVTYPE_HAL ||
						  
						  pdc->DevType           == D3DDEVTYPE_HAL &&
						  pdc->DisplayFormat     == dmDesktop.Format &&
						  pdcBest->DisplayFormat != dmDesktop.Format;
						  
				bBest = pdc->DevType          == D3DDEVTYPE_HAL &&
						pdc->DisplayFormat    == dmDesktop.Format &&
						pdc->BackBufferFormat == dmDesktop.Format;

				bBetter |= bBest;

				if (bBetter)
				{
					// make it the best so far
					dmDesktopBest = dmDesktop;
					paiBest = pai;
					pdiBest = pdi;
					pdcBest = pdc;
					
					// this one looks great -- take it
					if (bBest)
						goto DoneSearchingFDC;
				}
			}
		}
	}

DoneSearchingFDC:

	// no suitable dc found!
	if (pdcBest == NULL)
		return false;
	
	// now we need to find a display mode on the best ai that uses the best
	// dc's display format and is as close to the best desktop display mode
	// as possible	
	D3DDISPLAYMODE dm;
		
	for (i = 0; i < paiBest->DisplayModes.Length(); i++)
	{
		dm = paiBest->DisplayModes[i];
		
		// formats must match
		if (dm.Format != dmDesktopBest.Format)
			continue;
		
		// compare other properties
		if (dm.Width	   == dmDesktopBest.Width &&
			dm.Height	   == dmDesktopBest.Height && 
			dm.RefreshRate == dmDesktopBest.RefreshRate)
		{
			// perfect match, break out
			dmBest = dm;
			break;
		}
		else if (dm.Width      == dmDesktopBest.Width &&
			     dm.Height     == dmDesktopBest.Height && 
			     dm.RefreshRate > dmBest.RefreshRate)
		{
			// faster
			dmBest = dm;
		}
		else if (dm.Width == dmDesktopBest.Width)
		{
			// same width
			dmBest = dm;
		}
		else if (dmBest.Width == 0)
		{
			// we don't have anything better yet
			dmBest = dm;
		}
	}
	
	// save these settings
	Settings.Windowed = 0;	
	Settings.AdapterInfos[0] = paiBest;

	// index to the best dm within the ai
	Settings.ndm[0] = paiBest->DisplayModes.Find(dm);

	// indices to the best di and dc
	if (bBest)
	{
		Settings.ndi[0] = j;
		Settings.ndc[0] = k;
	}
	else
	{
		// retract to the 'better' di and dc
		Settings.ndi[0] = (UINT)paiBest->DeviceInfos.Find(*pdiBest);
		Settings.ndc[0] = (UINT)pdiBest->DeviceCombos.Find(*pdcBest);
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// FindBestWindowedMode(): adjusts settings with best available windowed mode,
// subject to the HAL and REF constraints; returns false if no such mode can be
// found.
//-----------------------------------------------------------------------------
bool CXD3D::FindBestWindowedMode(bool bHAL, bool bREF)
{
	// get the display mode of the primary adapter, which is assumed to be
	// where the window will appear
	D3DDISPLAYMODE dm;
	
	m_pd3d->GetAdapterDisplayMode(0, &dm);
	
	// 'best' storage
	AdapterInfo* paiBest = NULL;
	DeviceInfo*  pdiBest = NULL;
	DeviceCombo* pdcBest = NULL;

	// iterators
	AdapterInfo* pai;
	DeviceInfo*  pdi;
	DeviceCombo* pdc;	

	// success flags
	bool bBetter, bBest;

	UINT i, j, k;
	
	// traverse the enumerated adapters information
	for (i = 0; i < Enumeration.AdapterInfos.Length(); i++)
	{
		pai = &Enumeration.AdapterInfos[i];
		
		// for each adapter, traverse the device infos
		for (j = 0; j < pai->DeviceInfos.Length(); j++)
		{
			pdi = &pai->DeviceInfos[j];
			
			// skip according to the requirements
			if (bHAL && pdi->DevType != D3DDEVTYPE_HAL)
				continue;
			
			if (bREF && pdi->DevType != D3DDEVTYPE_REF)
				continue;
			
			// traverse device combos for this device
			for (k = 0; k < pdi->DeviceCombos.Length(); k++)
			{
				pdc = &pdi->DeviceCombos[k];
				
				// skip the non-windowed or distinct format combos
				if (!pdc->Windowed)
					continue;
				
				if (pdc->DisplayFormat != dm.Format)
					continue;
				
				// this device combo is better than the current best if:
				//  (a) there's no best yet;
				//  (b) it's a HAL and the current best is not;
				//  (c) it's a HAL with matching backbuffer and display
				//      formats, in which case is also the best
				bBetter = pdcBest == NULL ||

						  pdc->DevType     == D3DDEVTYPE_HAL && 
						  pdcBest->DevType != D3DDEVTYPE_HAL;

				bBest = pdc->DevType == D3DDEVTYPE_HAL &&
						pdc->BackBufferFormat == pdc->DisplayFormat;

				bBetter |= bBest;

				if (bBetter)
				{
					// save it as the current best
					paiBest = pai;
					pdiBest = pdi;
					pdcBest = pdc;
					
					// this dc looks great -- take it
					if (bBest)
						goto DoneSearchingWDC;
				}
			}
		}
	}

DoneSearchingWDC:

    // none found!!
	if (pdcBest == NULL)
		return false;

	Settings.Windowed = 1;
	Settings.AdapterInfos[1] = paiBest;

	int l = paiBest->DisplayModes.Find(dm);

	// for some bizarre multi-monitor setups in which the primary adapter's
	// current dm is not available in a secondary one, there's nothing else
	// we can do...
	if (i > 0 && l == -1)
		return false;

	// index to the best dm within the ai
	Settings.ndm[1] = l;

	// indices to the best di and dc
	if (bBest)
	{
		Settings.ndi[1] = j;
		Settings.ndc[1] = k;
	}
	else
	{
		// retract to the 'better' di and dc
		Settings.ndi[1] = (UINT)paiBest->DeviceInfos.Find(*pdiBest);
		Settings.ndc[1] = (UINT)pdiBest->DeviceCombos.Find(*pdcBest);
	}

	return true;
}

//-----------------------------------------------------------------------------
// ChooseInitialSettings(): according to the best mode founds and app settings
//-----------------------------------------------------------------------------
HRESULT CXD3D::ChooseInitialSettings()
{    
	bool bFoundFullscreen = FindBestFullscreenMode(false, false);
	bool bFoundWindowed   = FindBestWindowedMode(false, false);
	
	if (m_bStartFullscreen && bFoundFullscreen)
		Settings.Windowed = 0;
	
	if (!bFoundWindowed && bFoundFullscreen)
		Settings.Windowed = 0;
	
	if (!bFoundFullscreen && !bFoundWindowed)
		return D3DAPPERR_NOCOMPATIBLEDEVICES;

	if (!m_bStartFullscreen && !bFoundWindowed)
		return D3DAPPERR_NOCOMPATIBLEDEVICES;
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// HandlePossibleSizeChange(): reset the device if the client area size has
// changed; it will update the window properties, but it will not issue a reset
// unless old and new dimensions differ; a new window size will require a new
// backbuffer size, so the 3D environment must change accordingly.
//-----------------------------------------------------------------------------
HRESULT CXD3D::HandlePossibleSizeChange()
{
    if (m_bIgnoreSizeChange)
		return S_OK;

    HRESULT hr = S_OK;
    
	RECT rcOld = m_rcClient;

    GetClientRect(m_hWndRender, &m_rcClient);

    // check for client rect changes
	if (rcOld.right  - rcOld.left != m_rcClient.right  - m_rcClient.left ||
        rcOld.bottom - rcOld.top  != m_rcClient.bottom - m_rcClient.top)
    {
        Pause(true);

        // store the new dims
		m_d3dpp.BackBufferWidth  = m_rcClient.right  - m_rcClient.left;
        m_d3dpp.BackBufferHeight = m_rcClient.bottom - m_rcClient.top;
    
        // reset
		if (m_pd3dDevice != NULL)
        {
            if (FAILED(hr = ResetEnvironment()))
            {
                if (hr != D3DERR_OUTOFVIDEOMEMORY)
					hr = D3DAPPERR_RESETFAILED;
                
				DisplayErrorMsg(hr, MSGERR_CANNOTCONTINUE);
            }
        }
        
		Pause(false);
    }
    
	return hr;
}

//-----------------------------------------------------------------------------
// BuildPresentParamsFromSettings(): 'builds' presentation parameters from
// the current settings
//-----------------------------------------------------------------------------
void CXD3D::BuildPresentParamsFromSettings()
{
	m_d3dpp.Windowed			   = Settings.Windowed;
	m_d3dpp.hDeviceWindow          = m_hWndRender;
	m_d3dpp.BackBufferCount        = 1;
	m_d3dpp.EnableAutoDepthStencil = Enumeration.AppUsesDepthBuffer;
	m_d3dpp.MultiSampleType        = Settings.GetMSType();
	m_d3dpp.MultiSampleQuality     = Settings.GetMSQuality();
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.Flags				   = 0;
	
	if (Enumeration.AppUsesDepthBuffer)
	{
		m_d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		m_d3dpp.AutoDepthStencilFormat = Settings.GetDSFormat();
	}
	
	if (m_bWindowed)
	{
		m_d3dpp.BackBufferWidth  = m_rcClient.right  - m_rcClient.left;
		m_d3dpp.BackBufferHeight = m_rcClient.bottom - m_rcClient.top;
		m_d3dpp.FullScreen_RefreshRateInHz = 0;		
	}
	else
	{
		m_d3dpp.BackBufferWidth  = Settings.GetDisplayMode().Width;
		m_d3dpp.BackBufferHeight = Settings.GetDisplayMode().Height;
		m_d3dpp.FullScreen_RefreshRateInHz = Settings.GetDisplayMode().RefreshRate;
	}

	m_d3dpp.BackBufferFormat = Settings.GetBackBufferFormat();
	m_d3dpp.PresentationInterval = Settings.GetPresentInterval();
}

//-----------------------------------------------------------------------------
// InitializeEnvironment(): checks for a null REF device, builds presentation
// parameters from settings, instances the d3d device, sets up device stats,
// saves the backbuffer description, sets up the fullscreen cursor and finally
// inits and restores device objects. If the last step fails, it retries, but
// in windowed mode (to display a warning) and the reference rasterizer.
//-----------------------------------------------------------------------------
HRESULT CXD3D::InitializeEnvironment()
{
	HRESULT hr;
	
	AdapterInfo* pai = Settings.GetAdapterInfo();
	DeviceInfo*  pdi = Settings.GetDeviceInfo();
	
	// Warn user about a null REF device that cannot render anything
	if (pdi->Caps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE)
		DisplayErrorMsg(D3DAPPERR_NULLREFDEVICE, 0);
	
	// translate the VP type to a device creation behavior
	switch (Settings.GetVPType())
	{
	case PURE_VP: m_dwCreateFlags  = D3DCREATE_PUREDEVICE;
	case HARD_VP: m_dwCreateFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING; break;
	case MIXD_VP: m_dwCreateFlags  = D3DCREATE_MIXED_VERTEXPROCESSING;    break;
	case SOFT_VP: m_dwCreateFlags  = D3DCREATE_SOFTWARE_VERTEXPROCESSING; break;
	default:	  return E_FAIL;
	}
	
	// setup the creation presentation parameters
	BuildPresentParamsFromSettings();
	
	// create the device
	hr = m_pd3d->CreateDevice(pdi->AdapterOrdinal,
							  pdi->DevType,
							  m_hWndRender,
							  m_dwCreateFlags,
							  &m_d3dpp,
							  &m_pd3dDevice);
	
	if (SUCCEEDED(hr))
	{
		// store the device's description, beginning with type;
		lstrcpy(m_strDeviceStats, DEVICETYPESTRING(pdi->DevType, false));
		
		// then VP type, including  non-HAL devices simulating hardware VP and
		// the pure hardware VP variant...
		if ((m_dwCreateFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING) == 0 &&
			pdi->DevType != D3DDEVTYPE_HAL)
			lstrcat(m_strDeviceStats, TEXT(" simulated"));

		if (m_dwCreateFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		{
			if (m_dwCreateFlags & D3DCREATE_PUREDEVICE)
				lstrcat(m_strDeviceStats, TEXT(" pure"));
			
			lstrcat(m_strDeviceStats, TEXT(" hardware"));
		}
		else if (m_dwCreateFlags & D3DCREATE_MIXED_VERTEXPROCESSING)
			lstrcat(m_strDeviceStats, TEXT(" mixed"));
		else
			lstrcat(m_strDeviceStats, TEXT(" software"));
		
		lstrcat(m_strDeviceStats, TEXT(" VP"));
		
		// ...and the adapter's description for HAL devices
		if (pdi->DevType == D3DDEVTYPE_HAL)
		{
			lstrcat(m_strDeviceStats, TEXT(" on "));
			
			// be sure not to overflow m_strDeviceStats when appending
			const int nDescription = sizeof(pai->AdapterIdentifier.Description);
			
			TCHAR szDescription[nDescription];
			
			// DX Utils handle unicode somewhat gracefully...
			DXUtil_ConvertAnsiStringToGenericCch(szDescription,
												 pai->AdapterIdentifier.Description,
												 nDescription);
			
			lstrcat(szDescription, TEXT(" @ "));
			
			// append as many characters as space is left on the stats
			_tcsncat(m_strDeviceStats,
					 szDescription,
					 UBOUND(m_strDeviceStats) - lstrlen(m_strDeviceStats) - 1);

			TCHAR  szDims[100]; 
			TCHAR  szFmt[100];
			TCHAR  szDepthFmt[100];
			TCHAR* szMS;
	
			_sntprintf(szDims, 100, TEXT("%dx%d, "), 
			   m_d3dpp.BackBufferWidth,
			   m_d3dpp.BackBufferHeight);

			szDims[99] = TEXT('\0');
	
			// append as many characters as space is left on the stats
			_tcsncat(m_strDeviceStats, szDims,
					 UBOUND(m_strDeviceStats) - lstrlen(m_strDeviceStats) - 1);

			D3DFORMAT fmt = Settings.GetDisplayMode().Format;
	
			// display format (including the back buffer format if they do not match)
			if (fmt == m_d3dpp.BackBufferFormat)
				lstrcpyn(szFmt, D3DUtil_D3DFormatToString(fmt, false), 100);
			else
				_sntprintf(szFmt, 100, TEXT("%s back, %s front"), 
					D3DUtil_D3DFormatToString(m_d3dpp.BackBufferFormat, false), 
					D3DUtil_D3DFormatToString(fmt, false));
			
			szFmt[99] = TEXT('\0');
	
			// append as many characters as space is left on the stats
			_tcsncat(m_strDeviceStats,
					 szFmt,
					 UBOUND(m_strDeviceStats) - lstrlen(m_strDeviceStats) - 1);

			// depth/stencil buffer format
			if (Enumeration.AppUsesDepthBuffer)
			{
				_sntprintf(szDepthFmt, 100, TEXT(" (%s)"), 
					D3DUtil_D3DFormatToString(Settings.GetDSFormat(), false));
				
				szDepthFmt[99] = TEXT('\0');
				
				// append as many characters as space is left on the stats
				_tcsncat(m_strDeviceStats,
						 szDepthFmt,
						 UBOUND(m_strDeviceStats) - lstrlen(m_strDeviceStats) - 1);
			}
			
			// multisampling type (no. of samples or nonmaskable)
			szMS = MULTISAMPLESTRING(Settings.GetMSType(), false);

			// append as many characters as space is left on the stats
			_tcsncat(m_strDeviceStats,
					 szMS,
					 UBOUND(m_strDeviceStats) - lstrlen(m_strDeviceStats) - 1);
		}
		
		// setup the fullscreen cursor
		if (m_bShowCursor && !m_bWindowed)
		{
			HCURSOR hCursor = (HCURSOR)GetClassLong(m_hWndRender, GCL_HCURSOR);
			
			D3DUtil_SetDeviceCursor(m_pd3dDevice, hCursor, true);
			m_pd3dDevice->ShowCursor(true);
		}
		
		// confine the cursor to the fullscreen window
		if (m_bClipCursor)
			ClipCursor(m_bWindowed ? NULL : &m_rcWindow);
		
		// initialize the app's device-dependant objects
		if (FAILED(hr = InitDeviceObjects()))
			DeleteDeviceObjects();
		else
		{
			m_bDeviceObjectsInited = true;
			
			// restore the app's device-dependant objects
			if (FAILED(hr = RestoreDeviceObjects()))
				InvalidateDeviceObjects();
			else
			{
				m_bDeviceObjectsRestored = true;
				return S_OK;
			}
		}	
		
		// if any of that failed, cleanup before we try again
		CleanupEnvironment();
	}
	
	// if failure comes strictly from IDirect3D9 we'll try falling back to
	// the reference rasterizer; in other words, we'll ignore the error if
	// it is a 'file not found' error, because it is not Direct3D's fault,
	// it's the programmer's (or user's) fault!
	if (hr != D3DAPPERR_MEDIANOTFOUND &&
		hr != HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) &&
		pdi->DevType == D3DDEVTYPE_HAL)
	{
		// request a reference device
		if (FindBestWindowedMode(false, true))
		{
			// main window must not be topmost to show an error message...
			SetWindowPos(m_hWndRender,
						 HWND_NOTOPMOST,
						 m_rcWindow.left,
						 m_rcWindow.top,
						 (m_rcWindow.right  - m_rcWindow.left),
						 (m_rcWindow.bottom - m_rcWindow.top),
						 SWP_SHOWWINDOW);
			
			// ...in which we let the user know we are switching
			DisplayErrorMsg(hr, MSGWARN_SWITCHEDTOREF);
			
			// try again
			hr = InitializeEnvironment();
		}
	}
	
	return hr;
}

//-----------------------------------------------------------------------------
// ResetEnvironment(): signals the app it must invalidate all video memory
// objects and render the current frame if paused, then resets the device. The
// backbuffer description is saved.
//-----------------------------------------------------------------------------
HRESULT CXD3D::ResetEnvironment()
{
	HRESULT hr;
	
	// invalidate any resources thatt cannot survive the reset
	if (m_bDeviceObjectsRestored)
	{
		m_bDeviceObjectsRestored = false;
		InvalidateDeviceObjects();
	}
	
	if (FAILED(hr = m_pd3dDevice->Reset(&m_d3dpp)))
		return hr;

	// restore (re-create) resources
	if (FAILED(hr = RestoreDeviceObjects()))
	{
		InvalidateDeviceObjects();
		return hr;
	}
	
	m_bDeviceObjectsRestored = true;
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// RenderEnvironment(): tests for coop level, possibly resets the environment
// and issues the app defined FrameMove and Render that will actually draw the
// scene.
//-----------------------------------------------------------------------------
HRESULT CXD3D::RenderEnvironment()
{
	HRESULT hr;
	
	if (m_bDeviceLost)
	{
		// test the cooperative level to see if it's okay to render
		if (FAILED(hr = m_pd3dDevice->TestCooperativeLevel()))
		{
			// if the device was truly lost, (i.e., a fullscreen device just
			// lost focus), wait until we get it back
			if (hr == D3DERR_DEVICELOST)
				return S_OK;
			
			// eventually, we will get this return value, indicating
			// that we can reset the device
			if (hr == D3DERR_DEVICENOTRESET)
			{
				// if we are windowed, read the desktop mode and use the same
				// format for the back buffer UNTESTED WITH THE NEW SETTINGS CLASS!!!
				if (m_bWindowed)
				{
					m_pd3d->GetAdapterDisplayMode(Settings.GetAdapterInfo()->AdapterOrdinal,
												  &Settings.GetDisplayMode());
					
					m_d3dpp.BackBufferFormat = Settings.GetDisplayMode().Format;
				}
				
				// ...before resetting the device
				if (FAILED(hr = ResetEnvironment()))
					return hr;
			}
			
			return hr;
		}
		
		// we have a device
		m_bDeviceLost = false;
	}
	
	// setup the app's timers
	FLOAT fTime        = DXUtil_Timer(TIMER_GETAPPTIME);
	FLOAT fElapsedTime = DXUtil_Timer(TIMER_GETELAPSEDTIME);
	
	// skip rendering if no time elapsed (the application is paused)
	if (fElapsedTime == 0.0f)
		return S_OK;
	
	// store the time
	m_fTime        = fTime;
	m_fElapsedTime = fElapsedTime;
	
	// move the scene
	if (FAILED(hr = FrameMove()))
		return hr;
	
	// render the scene as normal
	if( FAILED(hr = Render()))
		return hr;
	
	// update the FPS
	UpdateStats();
	
	// present the next buffer in the swap chain
	if(m_pd3dDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
		m_bDeviceLost = true;
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// CleanupEnvironment(): cleanup device objects
//-----------------------------------------------------------------------------
void CXD3D::CleanupEnvironment()
{
	if (m_pd3dDevice != NULL)
	{
		if (m_bDeviceObjectsRestored)
		{
			m_bDeviceObjectsRestored = false;
			InvalidateDeviceObjects();
		}
		
		if (m_bDeviceObjectsInited)
		{
			m_bDeviceObjectsInited = false;
			DeleteDeviceObjects();
		}
		
		if (m_pd3dDevice->Release() > 0)
			DisplayErrorMsg(D3DAPPERR_NONZEROREFCOUNT, MSGERR_CANNOTCONTINUE);
		
		m_pd3dDevice = NULL;
	}
}

//-----------------------------------------------------------------------------
// UpdateStats()
//-----------------------------------------------------------------------------
void CXD3D::UpdateStats()
{
	// keep track of the frame count
	static float fLastTime = 0.0f;
	static DWORD dwFrames  = 0;
	
	float fTime = DXUtil_Timer(TIMER_GETABSOLUTETIME);
	
	++dwFrames;
	
	// let a full second elapse before updating the scene stats
	if (fTime - fLastTime <= 1.0f)
		return;

	m_fFPS    = dwFrames / (fTime - fLastTime);
	fLastTime = fTime;
	dwFrames  = 0;
	
	const int nMax = UBOUND(m_strFrameStats);
	
	_sntprintf(m_strFrameStats, nMax, _T("%.02f fps"), m_fFPS);
	
	m_strFrameStats[nMax - 1] = TEXT('\0');
}

//-----------------------------------------------------------------------------
// Pause(): toggles the active state of the app and resets the timer
//-----------------------------------------------------------------------------
void CXD3D::Pause(bool bPause)
{
	static DWORD dwAppPausedCount = 0;
	
	dwAppPausedCount += BSCALE(bPause);
	m_bActive         = dwAppPausedCount == 0;
	
	// handle the first pause request (of many, nestable pause requests) and
	// stop the scene from animating
	if (bPause && dwAppPausedCount == 1)
		DXUtil_Timer(TIMER_STOP);
	
	// restart the timer
	if (dwAppPausedCount == 0)
		DXUtil_Timer(TIMER_START);
}

//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
//-----------------------------------------------------------------------------
HRESULT CXD3D::DisplayErrorMsg(HRESULT hr, DWORD dwType)
{
    static bool s_bFatalErrorReported = false;
    TCHAR strMsg[512];

    // If a fatal error message has already been reported, the app
    // is already shutting down, so don't show more error messages.
    if (s_bFatalErrorReported)
        return hr;

    switch(hr)
    {
        case D3DAPPERR_NODIRECT3D:
            _tcscpy(strMsg,
					_T("Could not initialize Direct3D; check that the\n")
                    _T("latest version of DirectX is correctly installed\n")
					_T("on your system."));
            break;

        case D3DAPPERR_NOCOMPATIBLEDEVICES:
            _tcscpy(strMsg,
					_T("Could not find any compatible Direct3D devices."));
            break;

        case D3DAPPERR_NOWINDOWABLEDEVICES:
            _tcscpy(strMsg,
					_T("Cannot run in a desktop window with the current\n")
					_T("display settings; change your desktop settings\n")
					_T("to a 16- or 32-bit display mode and retry."));
            break;

        case D3DAPPERR_NOHARDWAREDEVICE:
            _tcscpy(strMsg,
					_T("No hardware-accelerated Direct3D devices found."));
            break;

        case D3DAPPERR_HALNOTCOMPATIBLE:
            _tcscpy(strMsg,
					_T("This application requires functionality not\n")
					_T("available on your Direct3D hardware accelerator."));
            break;

        case D3DAPPERR_NOWINDOW:
            _tcscpy(strMsg,
					_T("No window to render to was supplied.\n"));
            break;

        case D3DAPPERR_NOWINDOWEDHAL:
            _tcscpy(strMsg,
					_T("Your Direct3D hardware accelerator cannot render\n")
					_T("into a window.\n"));
            break;

        case D3DAPPERR_NODESKTOPHAL:
            _tcscpy(strMsg,
					_T("Your Direct3D hardware accelerator cannot render\n")
					_T("into a window with the current desktop display\n")
					_T("settings."));
            break;

        case D3DAPPERR_NOHALTHISMODE:
            _tcscpy(strMsg,
					_T("This application requires functionality that is\n")
					_T("not available on your Direct3D hardware accelerator\n")
					_T("with the current desktop display settings.\n"));
            break;

        case D3DAPPERR_MEDIANOTFOUND:
        case HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND):
            _tcscpy(strMsg, _T("Could not load required media."));
            break;

        case D3DAPPERR_RESETFAILED:
            _tcscpy(strMsg, _T("Could not reset the Direct3D device."));
            break;

        case D3DAPPERR_INITFAILED:
            _tcscpy(strMsg, _T("Could not initialize the Direct3D device."));
            break;

        case D3DAPPERR_NONZEROREFCOUNT:
            _tcscpy(strMsg,
					_T("A D3D object has a non-zero reference count\n")
					_T("(meaning things were not properly cleaned up)."));
            break;

        case D3DAPPERR_NULLREFDEVICE:
            _tcscpy(strMsg,
					_T("Warning: Nothing will be rendered.\n")
					_T("The reference rendering device was selected, but\n")
					_T("your computer only has a reduced-functionality\n")
					_T("reference device installed. Install the DirectX\n")
					_T("SDK to get the full reference device.\n"));
            break;

        case E_OUTOFMEMORY:
            _tcscpy(strMsg, _T("Not enough memory."));
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
            _tcscpy(strMsg, _T("Not enough video memory."));
            break;

        default:
            _tcscpy(strMsg, _T("Unknown Direct3D error."));
    }

    if (dwType == MSGWARN_SWITCHEDTOREF)
	{
        _tcscat(strMsg,
				_T("\n\nSwitching to the reference rasterizer, a\n")
				_T("software device that implements the entire\n")
				_T("Direct3D feature set, but runs very slowly."));
	}   

    if (dwType == MSGERR_CANNOTCONTINUE)
    {
        s_bFatalErrorReported = true;
        
		_tcscat(strMsg, _T("\n\nDirect3D cannot continue."));
        
    }

	MessageBox(NULL, strMsg, L"Direct3D", MB_ICONWARNING | MB_OK);
    
    // close the window
    if (s_bFatalErrorReported && m_hWndRender) SendMessage(m_hWndRender, WM_CLOSE, 0, 0);
	
	return hr;
}
