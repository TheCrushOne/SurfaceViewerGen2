#if !defined(AFX_D3DWND_H__C3D69D36_A0AD_47A5_8382_97AFF80430E3__INCLUDED_)
#define AFX_D3DWND_H__C3D69D36_A0AD_47A5_8382_97AFF80430E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// D3DWnd.h : header file
//

#include "XD3D.h"

// set up a set of axis vertices
static const D3DXVECTOR3 SAXES[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				 SAXES[0] + D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	SAXES[0], SAXES[0] + D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	SAXES[0], SAXES[0] + D3DXVECTOR3(0.0f, 0.0f, 1.0f),

	// x widget
	SAXES[1], SAXES[1] + D3DXVECTOR3(-0.1f, -0.1f, 0.0f),
	SAXES[1], SAXES[1] + D3DXVECTOR3(-0.1f, +0.1f, 0.0f),

	// y widget
	SAXES[3], SAXES[3] + D3DXVECTOR3(+0.1f, +0.1f, 0.0f),
	SAXES[3], SAXES[3] + D3DXVECTOR3(-0.1f, +0.1f, 0.0f),

	// z widget
	SAXES[5], SAXES[5] + D3DXVECTOR3(-0.1f, 0.0f, 0.0f),
	SAXES[5], SAXES[5] + D3DXVECTOR3(+0.1f, 0.0f, 0.0f),
};

static const int SAXES_VERTS = sizeof(SAXES) / sizeof(SAXES[0]);
static const int SAXES_LINES = SAXES_VERTS / 2;


/////////////////////////////////////////////////////////////////////////////
// CD3DWnd window

class CD3DWnd : public CWnd, public CXD3D
{
	// Construction
public:
	CD3DWnd();

	// Attributes
protected:
	// matrices
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	// view specification
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vDir;
	D3DXVECTOR3 m_vUp;

	// lights
	D3DLIGHT9 m_Light;

	// axes material and vertex buffer
	D3DMATERIAL9			m_AxesMaterial;
	LPDIRECT3DVERTEXBUFFER9 m_pVBAxes;

	// ambient color
	D3DCOLOR m_dwAmbientColor;

	// mouse input utility
	CD3DArcBall m_ArcBall;

public:

	// Operations
public:
	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CD3DWnd)
		//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CD3DWnd();

	// D3D Initializer
	HRESULT Initialize();

	// D3D overrides
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	inline void ResetEnvironment() { CXD3D::ResetEnvironment(); }
	// mouse input handler
	LRESULT HandleMouseMessages(UINT message, WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	//{{AFX_MSG(CD3DWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D3DWND_H__C3D69D36_A0AD_47A5_8382_97AFF80430E3__INCLUDED_)
