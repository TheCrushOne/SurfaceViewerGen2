//-----------------------------------------------------------------------------
// XD3DEnum.h.: enumerates D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------

#ifndef XD3DENUM_H
#define XD3DENUM_H

// D3D9 util stuff
#include "d3dutil.h"
#include "dxutil.h"

#include "TArray.h"

//-----------------------------------------------------------------------------
// DSMSConflict: a depth/stencil buffer format that is incompatible with a
// multisample type. We'll use an array of them, hence the typedef.
//-----------------------------------------------------------------------------
struct DSMSConflict
{
    D3DFORMAT			fmt;
    D3DMULTISAMPLE_TYPE	mst;
};

typedef CTArray<DSMSConflict> DSMSConflictArray;

//-----------------------------------------------------------------------------
// DeviceCombo class: a combination of adapter format and back buffer format
// that is compatible with a particular D3D device and the application. We
// will also use arrays of DeviceCombos, hence the typedef.
//-----------------------------------------------------------------------------
struct DeviceCombo
{
    int			AdapterOrdinal;			// the host adapter
    D3DDEVTYPE	DevType;
    D3DFORMAT	DisplayFormat;
    D3DFORMAT	BackBufferFormat;
    bool		Windowed;
    
   DWORDARRAY DSFormats;				// List of depth/stencil formats
    DWORDARRAY MSTypes;					// List of D3DMULTISAMPLE_TYPEs
    DWORDARRAY MSQualityLevels;			// List of quality levels for ea. type
    DWORDARRAY VPTypes;					// List of VP Types
    DWORDARRAY PresentIntervals;		// List of D3DPRESENT_INTERVALs

    DSMSConflictArray DSMSConflicts;	// List of DSMSConflicts
};

typedef CTArray<DeviceCombo> DeviceComboArray;

//-----------------------------------------------------------------------------
// DeviceInfo: info about a D3D device, including a list of device combos
// that work with the device; we'll use arrays of these, hence the typedef
//-----------------------------------------------------------------------------
struct DeviceInfo
{
    int			AdapterOrdinal;		// the host adapter
    D3DDEVTYPE	DevType;
    D3DCAPS9	Caps;				// device capabilities
    
	DeviceComboArray DeviceCombos; // List of DeviceCombos
};

typedef CTArray<DeviceInfo> DeviceInfoArray;

// this is a d3d structure we want to use an array of
typedef CTArray<D3DDISPLAYMODE> D3DDISPLAYMODEARRAY;

//-----------------------------------------------------------------------------
// AdapterInfo: info about a display adapter, and a typedef for an array of
// them
//-----------------------------------------------------------------------------
struct AdapterInfo
{
	int						AdapterOrdinal;

	D3DADAPTER_IDENTIFIER9	AdapterIdentifier;

	D3DDISPLAYMODEARRAY DisplayModes;	// List of D3DDISPLAYMODE s

	DeviceInfoArray DeviceInfos;		// List of DeviceInfos
};

typedef CTArray<AdapterInfo> AdapterInfoArray;

//-----------------------------------------------------------------------------
// CXD3DEnum class: enumerates available D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------
class CXD3DEnum
{
private:
	// pointer to the IDirect3D9 interface
    LPDIRECT3D9 m_pd3d;

	// implementation, called by EnumerateDeviceCombos
    void	EnumerateDSFormats(DeviceCombo* pdc);
    void	EnumerateMSTypes(DeviceCombo* pdc);
    void	EnumerateDSMSConflicts(DeviceCombo* pdc);
    void	EnumerateVPTypes(DeviceInfo* pdi, DeviceCombo* pdc);
    void	EnumeratePIntervals(DeviceInfo* pdi, DeviceCombo* pdc);

	// device combo enumeration, called by EnumerateDevices
    HRESULT EnumerateDeviceCombos(DeviceInfo* pdi, DWORDARRAY* pDisplayFormats);    

	// device enumeration, called by Enumerate
	HRESULT EnumerateDevices(AdapterInfo* pai, DWORDARRAY* pDisplayFormats);

public:
	// adapter enumeration
	HRESULT Enumerate(LPDIRECT3D9 pD3D);

	// constructor, initialises the app constraints
    CXD3DEnum();	

    // application constraints
    
	UINT AppMinFullscreenWidth;			// app min fullscreen width
    UINT AppMinFullscreenHeight;		// app min fullscreen height

    UINT AppMinRGBBits;					// min RGB bits per channel
    UINT AppMinAlphaBits;				// min alpha bits per pixel
    
	bool AppUsesMixedVP;				// whether the app can take advantage
										// of the mixed VP type

	bool AppUsesDepthBuffer;			// wether the app uses a depth buffer  
    	
	UINT AppMinDepthBits;				// min depth bits
	UINT AppMinStencilBits;				// min stencil bits
    
	DWORDARRAY AppDisplayFormats;		// list of app allowed display formats
	DWORDARRAY AppBackBufferFormats;	// list of app allowed BB formats
	DWORDARRAY AppDepthStencilFormats;	// list of app allowed DS formats
	DWORDARRAY AppMultiSamplingTypes;	// list of app allowed MS types
    
	AdapterInfoArray AdapterInfos;		// list of enumerated AdapterInfos
};

#endif
