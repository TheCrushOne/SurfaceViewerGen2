//-----------------------------------------------------------------------------
// XD3DSettings.h: Current D3D settings, including adapter, device, mode, etc.
//-----------------------------------------------------------------------------

#ifndef _XD3DSETTINGS_H_
#define _XD3DSETTINGS_H_

#include "XD3DUtil.h"
#include "XD3DEnum.h"

typedef CTArray<AdapterInfo*> AdapterInfoPtrArray;

//-----------------------------------------------------------------------------
// CXD3DSettings: Current D3D settings: adapter, device, mode, formats, etc.
//-----------------------------------------------------------------------------
class CXD3DSettings
{
public:
	AdapterInfoPtrArray	AdapterInfos;	// fullscreen [0] and windowed [1]

    UINT Windowed;						// index to the previous

	UINT ndm[2];	// indices to the best display mode within each AdapterInfo
	UINT ndi[2];	// indices to the best DeviceInfo within each AdapterInfo
    UINT ndc[2];	// indices to the best DeviceCombo within each DeviceInfo

	// indices into DeviceCombo properties that can be changed
	UINT nDSFormat;

	// constructor
	CXD3DSettings();

	// 'Get' wrappers
	AdapterInfo*		GetAdapterInfo();
    DeviceInfo*			GetDeviceInfo();
    D3DDISPLAYMODE		GetDisplayMode();
    
	VPTYPE				GetVPType();
    D3DFORMAT			GetBackBufferFormat();
    D3DFORMAT			GetDSFormat();
    D3DMULTISAMPLE_TYPE GetMSType();
    DWORD				GetMSQuality();
    DWORD				GetPresentInterval();

    // 'Set' wrappers
	void SetDSFormat(UINT nFmt);
};

#endif
