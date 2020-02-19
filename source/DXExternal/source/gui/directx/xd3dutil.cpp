#include "stdafx.h"
#include "XD3DUtil.h"

// color bits in a D3D format

UINT RGBBITS(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_A2B10G10R10:	return 10;
    case D3DFMT_A2R10G10B10:	return 10;    
	case D3DFMT_R8G8B8:			return 8;
    case D3DFMT_A8R8G8B8:		return 8;
    case D3DFMT_X8R8G8B8:		return 8;
	case D3DFMT_R5G6B5:			return 5;
    case D3DFMT_X1R5G5B5:		return 5;
    case D3DFMT_A1R5G5B5:		return 5;
	case D3DFMT_A4R4G4B4:		return 4;
    case D3DFMT_X4R4G4B4:		return 4;
	case D3DFMT_R3G3B2:			return 2;
    case D3DFMT_A8R3G3B2:		return 2;
	default:					return 0;
    }
}

// alpha bits in a D3D format

UINT ALPHABITS(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_A8R8G8B8:		return 8;
    case D3DFMT_A8R3G3B2:		return 8;
    case D3DFMT_A4R4G4B4:		return 4;    
	case D3DFMT_A2B10G10R10:	return 2;
    case D3DFMT_A2R10G10B10:	return 2;
	case D3DFMT_A1R5G5B5:		return 1;
	case D3DFMT_X8R8G8B8:		return 0;
    case D3DFMT_X4R4G4B4:		return 0;
    case D3DFMT_X1R5G5B5:		return 0;
    case D3DFMT_R8G8B8:			return 0;
    case D3DFMT_R5G6B5:			return 0;
    case D3DFMT_R3G3B2:			return 0;
    default:					return 0;
    }
}

// depth bits in a D3D format

UINT DEPTHBITS(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_D32:		return 32;
    case D3DFMT_D24X8:		return 24;
    case D3DFMT_D24S8:		return 24;
    case D3DFMT_D24X4S4:	return 24;
    case D3DFMT_D16:		return 16;
    case D3DFMT_D15S1:		return 15;
    default:				return 0;
    }
}

// stencil bits in a D3D format

UINT STENCILBITS(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_D24S8:		return 8;
    case D3DFMT_D24X4S4:	return 4;
    case D3DFMT_D15S1:		return 1;
    case D3DFMT_D16:		return 0;
    case D3DFMT_D24X8:		return 0;
    case D3DFMT_D32:		return 0;
    default:				return 0;
    }
}

// VP type to string UNUSED

TCHAR* VPTYPESTRING(VPTYPE vpt)
{
	switch (vpt)
	{
	case SOFT_VP: return TEXT("SOFTWARE_VP");
	case MIXD_VP: return TEXT("MIXED_VP");
	case HARD_VP: return TEXT("HARDWARE_VP");
	case PURE_VP: return TEXT("PURE_HARDWARE_VP");
	default:	  return TEXT("Unknown VP Type");
	}
}

// D3D device type to string, with optional prefix removal

TCHAR* DEVICETYPESTRING(D3DDEVTYPE dt, bool bPrefix)
{
    TCHAR* pRet = NULL;

    switch (dt)
    {
    case D3DDEVTYPE_HAL: pRet = TEXT("D3DDEVTYPE_HAL"); break;
    case D3DDEVTYPE_SW:  pRet = TEXT("D3DDEVTYPE_SOF"); break;
    case D3DDEVTYPE_REF: pRet = TEXT("D3DDEVTYPE_REF"); break;
    default:             pRet = TEXT("Unknown Device Type"); break;
    }

	if (bPrefix || _tcsstr(pRet, TEXT("D3DDEVTYPE_")) == NULL)
		return pRet;

	return pRet + lstrlen(TEXT("D3DDEVTYPE_"));
}

// Multisample type to string converter, with optional prefix removal

TCHAR* MULTISAMPLESTRING(D3DMULTISAMPLE_TYPE mst, bool bPrefix)
{
    TCHAR* pRet = NULL;

	// notice we'll skip the D3DMULTISAMPLE_NONE case
    switch (mst)
    {
    case D3DMULTISAMPLE_NONMASKABLE:	pRet = TEXT("D3DMULTISAMPLE_NONMASKABLE"); break;
    case D3DMULTISAMPLE_2_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_2_SAMPLES");   break;
    case D3DMULTISAMPLE_3_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_3_SAMPLES");   break;
    case D3DMULTISAMPLE_4_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_4_SAMPLES");   break;
    case D3DMULTISAMPLE_5_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_5_SAMPLES");   break;
    case D3DMULTISAMPLE_6_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_6_SAMPLES");   break;
    case D3DMULTISAMPLE_7_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_7_SAMPLES");   break;
    case D3DMULTISAMPLE_8_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_8_SAMPLES");   break;
    case D3DMULTISAMPLE_9_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_9_SAMPLES");   break;
    case D3DMULTISAMPLE_10_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_10_SAMPLES");  break;
    case D3DMULTISAMPLE_11_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_11_SAMPLES");  break;
    case D3DMULTISAMPLE_12_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_12_SAMPLES");  break;
    case D3DMULTISAMPLE_13_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_13_SAMPLES");  break;
    case D3DMULTISAMPLE_14_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_14_SAMPLES");  break;
    case D3DMULTISAMPLE_15_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_15_SAMPLES");  break;
    case D3DMULTISAMPLE_16_SAMPLES:		pRet = TEXT("D3DMULTISAMPLE_16_SAMPLES");  break;
    default:							pRet = TEXT("");
    }

	if (bPrefix || _tcsstr(pRet, TEXT("D3DMULTISAMPLE_")) == NULL)
		return pRet;

	return pRet + lstrlen(TEXT("D3DMULTISAMPLE_"));
}

// Presentation interval to string, with optional prefix removal

TCHAR* PRESENTINTERVALSTRING(UINT pi, bool bPrefix)
{
    TCHAR* pRet = NULL;

    switch(pi)
    {
    case D3DPRESENT_INTERVAL_IMMEDIATE: pRet = TEXT("D3DPRESENT_INTERVAL_IMMEDIATE"); break;
    case D3DPRESENT_INTERVAL_DEFAULT:   pRet = TEXT("D3DPRESENT_INTERVAL_DEFAULT"); break;
    case D3DPRESENT_INTERVAL_ONE:       pRet = TEXT("D3DPRESENT_INTERVAL_ONE"); break;
    case D3DPRESENT_INTERVAL_TWO:       pRet = TEXT("D3DPRESENT_INTERVAL_TWO"); break;
    case D3DPRESENT_INTERVAL_THREE:     pRet = TEXT("D3DPRESENT_INTERVAL_THREE"); break;
    case D3DPRESENT_INTERVAL_FOUR:      pRet = TEXT("D3DPRESENT_INTERVAL_FOUR"); break;
    default:                            pRet = TEXT("Unknown Present Interval"); break;
    }

	if (bPrefix || _tcsstr(pRet, TEXT("D3DPRESENT_INTERVAL_")) == NULL)
		return pRet;

	return pRet + lstrlen(TEXT("D3DPRESENT_INTERVAL_"));
}

