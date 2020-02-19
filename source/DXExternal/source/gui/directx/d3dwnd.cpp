// D3DWnd.cpp : implementation file
//

#include "stdafx.h"

#include "D3DWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CD3DWnd

CD3DWnd::CD3DWnd()
{
	m_dwAmbientColor = D3DCOLOR_ARGB(0, 255, 255, 255);

	m_pVBAxes = NULL;
}

CD3DWnd::~CD3DWnd()
{
	DeleteDeviceObjects();
}

/////////////////////////////////////////////////////////////////////////////
// CD3DWnd initialization

HRESULT CD3DWnd::Initialize()
{
	CXD3D::m_hWndRender = GetSafeHwnd();
	CXD3D::Enumeration.AppUsesDepthBuffer = true;
	return CXD3D::CreateD3D();
}

/////////////////////////////////////////////////////////////////////////////
// InitDeviceObjects override; init materials, textures, lights, matrices,
// the arcball mouse input utility and other resources

HRESULT CD3DWnd::InitDeviceObjects()
{
	// init materials and textures
	D3DUtil_InitMaterial(m_AxesMaterial, 1.0f, 1.0f, 0.0f, 1.0f);

	m_AxesMaterial.Emissive.a = 1.0f;
	m_AxesMaterial.Emissive.r = 1.0f;
	m_AxesMaterial.Emissive.g = 1.0f;
	m_AxesMaterial.Emissive.b = 0.0f;

	m_AxesMaterial.Specular.a = 1.0f;
	m_AxesMaterial.Specular.r = 1.0f;
	m_AxesMaterial.Specular.g = 1.0f;
	m_AxesMaterial.Specular.b = 0.0f;

	// init the arcball utility
	m_ArcBall.Init();
	m_ArcBall.SetWindow(m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.333333f);

	// init lights
	D3DUtil_InitLight(m_Light, D3DLIGHT_DIRECTIONAL, -0.5f, -0.2f, 0.5f);

	m_Light.Diffuse.a = 1.0f;
	m_Light.Diffuse.r = 1.0f;
	m_Light.Diffuse.g = 1.0f;
	m_Light.Diffuse.b = 1.0f;

	m_Light.Specular.r = 1.0f;
	m_Light.Specular.g = 1.0f;
	m_Light.Specular.b = 1.0f;
	m_Light.Specular.a = 1.0f;

	m_Light.Range = 1000;
	m_Light.Falloff = 0;
	m_Light.Attenuation0 = 1;
	m_Light.Attenuation1 = 0;
	m_Light.Attenuation2 = 0;

	// init matrices
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);

	m_vEye = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	m_vDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vDir, &m_vUp);

	// set the view matrix
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// RestoreDeviceObjects: 

HRESULT CD3DWnd::RestoreDeviceObjects()
{
	HRESULT hr;

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// * AXES
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	SAFE_RELEASE(m_pVBAxes);

	// Build the axes vertex buffer
	if (FAILED(hr = m_pd3dDevice->CreateVertexBuffer(sizeof(SAXES), 0,
		D3DFVF_XYZ, D3DPOOL_MANAGED, &m_pVBAxes, NULL)))
		return hr;

	void* pVerts;

	//copy them into the buffer (lock, alter, unlock)
	if (FAILED(hr = m_pVBAxes->Lock(0, 0, &pVerts, 0)))
		return hr;

	memcpy(pVerts, SAXES, sizeof(SAXES));

	m_pVBAxes->Unlock();

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// * PROJECTION MATRIX
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	float fAspect = (float)m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, fAspect, 0.1f, 100.0f);

	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// * MISC RENDER STATES
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	// fill mode
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// culling mode
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// texture filtering
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// z-buffering
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// * LIGHTING RENDER STATES
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	// enable lights, or not event ambient will have an effect
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// enable specular lighting
	m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	// ambient light color
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, m_dwAmbientColor);

	// setup a light
	m_pd3dDevice->SetLight(0, &m_Light);
	m_pd3dDevice->LightEnable(0, TRUE);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// XD3D FrameMove override

HRESULT CD3DWnd::FrameMove()
{
	D3DXMatrixTranslation(&m_matWorld, m_vDir.x, m_vDir.y, m_vDir.z);
	D3DXMatrixMultiply(&m_matWorld, &m_matWorld, m_ArcBall.GetRotationMatrix());
	D3DXMatrixMultiply(&m_matWorld, &m_matWorld, m_ArcBall.GetTranslationMatrix());

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// XD3D Render override; TODO: we are just rendering one color stage so far...

HRESULT CD3DWnd::Render()
{
	HRESULT hr;

	// clear the viewport; TODO: to other than black...
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L);

	if (FAILED(hr = m_pd3dDevice->BeginScene()))
		return hr;

	// setup materials

	// setup textures

	// setup texture stage states

	// render the primitives

	// render Axes
	if (m_pVBAxes)
	{
		m_pd3dDevice->SetTexture(0, NULL);
		m_pd3dDevice->SetMaterial(&m_AxesMaterial);
		m_pd3dDevice->SetFVF(D3DFVF_XYZ);

		m_pd3dDevice->SetStreamSource(0, m_pVBAxes, 0, sizeof(D3DXVECTOR3));
		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, SAXES_LINES);
	}

	m_pd3dDevice->EndScene();

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// XD3D InvalidateDeviceObjects override

HRESULT CD3DWnd::InvalidateDeviceObjects()
{
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// XD3D DeleteDeviceObjects override

HRESULT CD3DWnd::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pVBAxes);

	InvalidateDeviceObjects();

	return S_OK;
}

LRESULT CD3DWnd::HandleMouseMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_ArcBall.HandleMouseMessages(m_hWndRender, message, wParam, lParam);
}

BEGIN_MESSAGE_MAP(CD3DWnd, CWnd)
	//{{AFX_MSG_MAP(CD3DWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CD3DWnd message handlers

