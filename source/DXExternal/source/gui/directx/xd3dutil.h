//-----------------------------------------------------------------------------
// XD3DUtil.h: XD3D framework utility constants, macros and globals
//-----------------------------------------------------------------------------

#ifndef _XD3DUTIL_H_
#define _XD3DUTIL_H_

#include <tchar.h>
#include <d3d9.h>

// Vertex Proccessing types enum, including the (hardware) pure device variant

enum VPTYPE
{
	SOFT_VP,
	MIXD_VP,
	HARD_VP,
	PURE_VP,
    FORCE_DWORD_VP = 0x7fffffff
};

// bits in a D3D format inspectors

UINT RGBBITS(D3DFORMAT fmt);
UINT ALPHABITS(D3DFORMAT fmt);
UINT DEPTHBITS(D3DFORMAT fmt);
UINT STENCILBITS(D3DFORMAT fmt);


// D3D type to string converters, with optional prefix skipping

TCHAR* VPTYPESTRING(VPTYPE vpt);
TCHAR* DEVICETYPESTRING(D3DDEVTYPE dt, bool bPrefix = true);
TCHAR* MULTISAMPLESTRING(D3DMULTISAMPLE_TYPE mst, bool bPrefix = true);
TCHAR* PRESENTINTERVALSTRING(UINT pi, bool bPrefix = true);

#endif