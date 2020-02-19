//-----------------------------------------------------------------------------
// XD3DSettings.cpp: settings class for the XD3D framework
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "XD3DSettings.h"

//-----------------------------------------------------------------------------
// Settings constructor: sizes the arrays to 2 growing in 2s, to save some
// memory (the default grow factor is 8, but we only need 2). It also
// initialises the DeviceInfo and DeviceCombo position indices, and defaults
// to Windowed, but notice that we will not get any meaningful values until
// the settings get initialised by the CXD3D class; the values will just
// save an access violation.
//-----------------------------------------------------------------------------

CXD3DSettings::CXD3DSettings()
{
	AdapterInfos.SetGrowFactor(2);
	AdapterInfos.SetLength(2);

	ndm[0] = ndm[1] = ndi[0] = ndi[1] = ndc[0] = ndc[1] = 0;

	Windowed = 1;

	nDSFormat = 0;
};

// adapter info wrapper, based on the windowed index

AdapterInfo* CXD3DSettings::GetAdapterInfo()
{
	return AdapterInfos[Windowed];
}

// device info wrapper, based on the windowed index and the di index

DeviceInfo*  CXD3DSettings::GetDeviceInfo()
{
	return &AdapterInfos[Windowed]->DeviceInfos[ndi[Windowed]];
}

// display mode wrapper, based on the windowed index

D3DDISPLAYMODE CXD3DSettings::GetDisplayMode()
{
	return AdapterInfos[Windowed]->DisplayModes[ndm[Windowed]];
}

// VP type wrapper

VPTYPE CXD3DSettings::GetVPType()
{
	return (VPTYPE)AdapterInfos[Windowed]->
				   DeviceInfos[ndi[Windowed]].
				   DeviceCombos[ndc[Windowed]].
				   VPTypes[0];
}

// back buffer format wrapper

D3DFORMAT CXD3DSettings::GetBackBufferFormat()
{
	return (D3DFORMAT)AdapterInfos[Windowed]->
					  DeviceInfos[ndi[Windowed]].
					  DeviceCombos[ndc[Windowed]].
					  BackBufferFormat;
}

// depth/stencil format wrapper

D3DFORMAT CXD3DSettings::GetDSFormat()
{
	return (D3DFORMAT)AdapterInfos[Windowed]->
					  DeviceInfos[ndi[Windowed]].
					  DeviceCombos[ndc[Windowed]].
					  DSFormats[nDSFormat];
}

// multisampling wrapper

D3DMULTISAMPLE_TYPE CXD3DSettings::GetMSType()
{
	return (D3DMULTISAMPLE_TYPE)AdapterInfos[Windowed]->
								DeviceInfos[ndi[Windowed]].
								DeviceCombos[ndc[Windowed]].
								MSTypes[0];
}

// multisampling quality levels wrapper

DWORD CXD3DSettings::GetMSQuality()
{
	return (DWORD)AdapterInfos[Windowed]->
				  DeviceInfos[ndi[Windowed]].
				  DeviceCombos[ndc[Windowed]].
				  MSQualityLevels[0];
}


// presentation interval wrapper

DWORD CXD3DSettings::GetPresentInterval()
{
	return (DWORD)AdapterInfos[Windowed]->
				  DeviceInfos[ndi[Windowed]].
				  DeviceCombos[ndc[Windowed]].
				  PresentIntervals[0];
}

// Set DS format wrapper

void CXD3DSettings::SetDSFormat(UINT nFmt)
{

	if (nFmt < AdapterInfos[Windowed]->
			   DeviceInfos[ndi[Windowed]].
			   DeviceCombos[ndc[Windowed]].
			   DSFormats.Length())
		nDSFormat = nFmt;
}
