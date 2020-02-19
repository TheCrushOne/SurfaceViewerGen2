/**
  @file     myApp.cpp
  @brief    User's application class
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <zmouse.h>

#include "Library/cglD3D.h"
#include "myApp.h"

// *******************************************************************
// defines

namespace
{

const float s_rMouseMove2Rotate = 0.001f;
const float s_rMouseWheel2Zoom = 0.001f;
const float s_rKbd2Rotate = 1.16f;
const float s_rKbd2Zoom = 1.16f;

void shiftColor(unsigned &color, float delta, int shift)
{
  int nComp = (color >> shift) & 0xFF;
  nComp += int(delta * 255.0f);
  if (nComp > 0xFF) 
    nComp = 0xFF;
  else if (nComp < 0)
    nComp = 0;
  color &= ~(0xFF << shift);
  color |= nComp << shift;
}

}


// *******************************************************************
// Methods

/*
\fn bool processInput(unsigned int nMsg, int wParam, long lParam)
\brief This function performs input processing. Returns true if input is handled © h-file
\param nMsg - N/A
\param wParam - N/A
\param lParam - N/A
\retuкn - N/A
*/
bool myApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;
    D3DXVECTOR3 direction;

    

  // Handle input
  switch (nMsg)
  {
    // Process mouse move message
    case WM_MOUSEMOVE:
    {
      // Get mouse x & y (to the upper-left corner of the client area)
      int xPos = GET_X_LPARAM(lParam); 
      int yPos = GET_Y_LPARAM(lParam); 

      if ((wParam & MK_LBUTTON) != 0 && m_nPrevMouseX >= 0) // 
      {
        /*rotate(s_rMouseMove2Rotate*(xPos-m_nPrevMouseX), 
               s_rMouseMove2Rotate*(yPos-m_nPrevMouseY));*/
          dx = s_rMouseMove2Rotate*(xPos - m_nPrevMouseX);
          dy = s_rMouseMove2Rotate*(yPos - m_nPrevMouseY);
      }
        
      m_nPrevMouseX = xPos;
      m_nPrevMouseY = yPos;

	  g_camera->ctrlMouseMove(dx, dy);
      
      break;
    }
    
    // Process mouse wheel message
    case WM_MOUSEWHEEL:
    {
      int zDelta = (int)((signed short)(HIWORD(wParam)));
      //zoom(zDelta * s_rMouseWheel2Zoom);
      dz = zDelta * s_rMouseWheel2Zoom;
	  g_camera->ctrlZoom(dz);
      break;
    }
    
    case WM_KEYDOWN:
    {
      // Save flags for continuous controls
      if (wParam < MAX_KEYS)
        m_keysPressed[wParam] = true;
      //getMovementDirection(direction);
      //if (g_camera->getCameraMoveType() == CM_SPECTATOR)
          //g_camera->updatePosition(direction, getElapsedTimeInSeconds());
      break;
    }

    case WM_KEYUP:
    {
      // Save flags for continuous controls
      if (wParam < MAX_KEYS)
        m_keysPressed[wParam] = false;
      
      // Process switch controls
      switch (wParam)
      {
        case 0x31: 
            if (dLightEnabled)
                dLightEnabled = false;
            else
                dLightEnabled = true;
            break;
        case 0x32: 
            if (sLightEnabled)
                sLightEnabled = false;
            else
                sLightEnabled = true;
            break;
        case 0x33: 
            if (pLightEnabled)
                pLightEnabled = false;
            else
                pLightEnabled = true;
            break;
		case 0x4D:	//(vk M)			
			//MessageBoxA(0, "M", "OK", MB_OK);
			if (filtrationType == D3DTEXF_LINEAR)
				filtrationType = D3DTEXF_POINT;
			else if (filtrationType == D3DTEXF_POINT)
				filtrationType = D3DTEXF_NONE;
			else if (filtrationType == D3DTEXF_NONE)
				filtrationType = D3DTEXF_LINEAR;
			else 
				;
			
			break;
		case 0x6B:	//(vk +)
			if (m_keysPressed[VK_SHIFT])
				MessageBoxA(0, "6B", "OK", MB_OK);
			break;
		case 0x6D:	//(vk -)
			if (m_keysPressed[VK_SHIFT])
				MessageBoxA(0, "6D", "OK", MB_OK);
			break;
		case 0x43:  //(vk C)
            g_camera->toggleCameraMoveType();
            break;
        case VK_F1: //(vk F1)
            g_camera->toggleCameraCollisionMod();
			break;
        case 0x48:  //(vk H)
            g_info->toggleShowMainInfo();
            break;
        case 0x4B:  //(vk K)
            g_info->toggleShowDbgInfo();
            break;  
        case 0x49:  //(vk I)
            g_info->toggleShowServiceInfo();
            break;
      }
      break;
    }
  }
  
  return cglApp::processInput(nMsg, wParam, lParam);
}



/*
\fn void update()
\brief - Обработка поступающих сигналов с ув
*/
void myApp::update()
{

  

  // Call predecessor update
  cglApp::update();
  
  // Process keyboard
  float dx = 0.0f;
  float dy = 0.0f;
  float dr = 0.0f;
  if (m_keysPressed[VK_LEFT])   
    dx -= s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_RIGHT])   
    dx += s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_UP])   
    dy -= s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_DOWN])   
    dy += s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_SUBTRACT])   
    dr -= s_rKbd2Zoom * m_timer.getDelta();
  if (m_keysPressed[VK_ADD])   
    dr += s_rKbd2Zoom * m_timer.getDelta();
  //ктр.
    
  /*if (dx != 0.0f)
    g_camera.pivit(dx);
  if (dy != 0.0f)
    g_camera.roll(dy);
  if (dr != 0.0f)
    g_camera.tilt(dr);*/
  g_camera->ctrlForwardSide(dx, dy);
  g_camera->ctrlZoom(dr);
  

  //dFont->Release();
  /*
  float rI = 0.0f;
  for (int i = 0; i < 10000000;i++)
    rI += sin(i);*/
}



/*
\fn void renderInternal()
\brief - Функция рендера
*/
void myApp::renderInternal()
{
    // Просто очищаем задний буфер
    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал
    
    //MessageBoxW(0, (LPCWSTR)"d1", (LPCWSTR)"d2", MB_YESNOCANCEL);
    mountData();

    g_info->update();
#ifdef _DEBUG
	//dbgInfoBox(eyeX, eyeY, eyeZ, LookatX, LookatY, LookatZ);
#endif
}



/*
\fn void mountData()
\brief - Функция заполнения буфферов, установки флагов и параметров
*/
void myApp::mountData()
{
	//LPD3DXBUFFER bufMaterial;
    //vertexBuf = 0;
    //indexBuf = 0;   

    //===========
    //generateObject();
    //===========

    vFlags = D3DFVF_TEX1;   //координаты и рассеивающий цвет    
    
    d3dd->SetFVF(vFlags);
	
	loadObjectScene();

    
    static D3DXVECTOR3 direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    static D3DXVECTOR3 vectorA, vectorB, newPosition;
    static D3DXMATRIX c_mvMatrix, c_rtMatrix, c_mgMatrix;

    //перед всеми, чтобы можно было сразу фары прописать
    createLights();

    //уровень сцены задает дом
    //house
    for (DWORD i = 0; i < h_numMaterials; i++)
    {
        // Set the material and texture for this subset
        d3dd->SetMaterial(&h_meshMaterials[i]);

        d3dd->SetTexture(0, h_meshTextures[i]);
        // Draw the mesh subset
        h_mesh->DrawSubset(i);

    }


    D3DXMatrixTranslation(&c_mvMatrix, 0.0f, -0.15f, 6.0f);        //машина вперед
    D3DXMatrixRotationY(&c_rtMatrix, D3DX_PI / 2);      //изначально машина повернута носом в здание...
    D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
    
    d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
    //ground
    //drawGround();
    for (DWORD i = 0; i < g_numMaterials; i++)
    {
        // Set the material and texture for this subset
        d3dd->SetMaterial(&g_meshMaterials[i]);

        d3dd->SetTexture(0, g_meshTextures[i]);
        // Draw the mesh subset
        g_mesh->DrawSubset(i);

    }
    D3DXMatrixRotationY(&c_rtMatrix, 0);
    D3DXMatrixTranslation(&c_mvMatrix, 0, 0, 0);        //мир обратно:)
    D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
    d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
    
    static short directionType = DT_XU;
    static float c_posX = 0.0f, c_posZ = 0.0f, c_angleYaw = 0.0f;
    static float c_xPitch = 0.5f, c_zPitch = 0.5f, c_yawPitch = -D3DX_PI / 48;
    static float c_xdTurnBorder = -10.0f, c_xuTurnBorder = 10.0f, c_zdTurnBorder = 7.0f, c_zuTurnBorder = 13.0f;
    static float c_xdTrajBorder = -17.0f, c_xuTrajBorder = 17.0f, c_zdTrajBorder = 0.0f, c_zuTrajBorder = 20.0f;
    static float c_fHlPosDistance = 2.59f, c_fHlPosAngleDeviation = D3DX_PI / 8.6f, c_fHlHeight = 0.64f;
    static float c_rMlPosDistance = 3.16f, c_rMlPosAngleDeviation = D3DX_PI / 10.8f, c_rMlHeight = 0.98f;
    static float c_fHlLightAngleDeviation = D3DX_PI / 20, c_rMlLightAngleDeviation = D3DX_PI / 64;//64
    static D3DXVECTOR3 vecDir, vecPos;
    static float fct = 0.0f;

    vectorA = D3DXVECTOR3(c_posX, 0, c_posZ);

    switch (directionType)
    {
    case DT_XU:
        c_posX += c_xPitch;
        if (c_posX >= c_xuTurnBorder)
            directionType = DT_XUZU;
        break;
    case DT_XD:
        c_posX -= c_xPitch;
        if (c_posX <= c_xdTurnBorder)
            directionType = DT_XDZD;
        break;
    case DT_ZU:
        c_posZ += c_zPitch;
        if (c_posZ >= c_zuTurnBorder)
            directionType = DT_ZUXD;
        break;
    case DT_ZD:
        c_posZ -= c_zPitch;
        if (c_posZ <= c_zdTurnBorder)
            directionType = DT_ZDXU;
        break;
    case DT_XUZU:
        c_angleYaw += c_yawPitch;        
        c_posX += (c_xuTrajBorder - c_xuTurnBorder)*(sin(c_angleYaw - c_yawPitch) - sin(c_angleYaw));
        c_posZ += (c_zuTrajBorder - c_zuTurnBorder)*(cos(c_angleYaw - c_yawPitch) - cos(c_angleYaw));
        if (c_angleYaw <= -D3DX_PI / 2)
        {
            directionType = DT_ZU;
            c_angleYaw = -D3DX_PI/2;
        }
        break;
    case DT_ZUXD:
        c_angleYaw += c_yawPitch;
        c_posX -= (c_xdTrajBorder - c_xdTurnBorder)*(sin(c_angleYaw - c_yawPitch) - sin(c_angleYaw));
        c_posZ += (c_zuTrajBorder - c_zuTurnBorder)*(cos(c_angleYaw - c_yawPitch) - cos(c_angleYaw));
        if (c_angleYaw <= -D3DX_PI)
        {
            directionType = DT_XD;
            c_angleYaw = -D3DX_PI;
        }
        break;
    case DT_XDZD:
        c_angleYaw += c_yawPitch;
        c_posX -= (c_xdTrajBorder - c_xdTurnBorder)*(sin(c_angleYaw - c_yawPitch) - sin(c_angleYaw));
        c_posZ -= (c_zdTrajBorder - c_zdTurnBorder)*(cos(c_angleYaw - c_yawPitch) - cos(c_angleYaw));
        if (c_angleYaw <= -1.5f*D3DX_PI)
        {
            directionType = DT_ZD;
            c_angleYaw = -3*(D3DX_PI/2);
        }
        break;
    case DT_ZDXU:
        c_angleYaw += c_yawPitch;
        c_posX += (c_xuTrajBorder - c_xuTurnBorder)*(sin(c_angleYaw - c_yawPitch) - sin(c_angleYaw));
        c_posZ -= (c_zdTrajBorder - c_zdTurnBorder)*(cos(c_angleYaw - c_yawPitch) - cos(c_angleYaw));
        if (c_angleYaw <= -2 * D3DX_PI)
        {
            directionType = DT_XU;
            c_angleYaw = 0.0f;
        }
        
        break;
    }
    
      

    vectorB = D3DXVECTOR3(c_posX, 0, c_posZ);

    direction = vectorA - vectorB;
    D3DXVec3Normalize(&direction, &direction);
    newPosition = vectorA + (direction * 0.001f);

    D3DXMatrixTranslation(&c_mvMatrix, c_posX, 0.72f, c_posZ-2.2f);        //машина вперед
    D3DXMatrixRotationY(&c_rtMatrix, D3DX_PI/2 + c_angleYaw);      //изначально машина повернута носом в здание...
    D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
    
    d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
    
    if (c_angleYaw == -D3DX_PI / 2)		//debug
        fct = 0.0f;

    c_leftFrontHeadlight.Position.x = c_posX + c_fHlPosDistance*cos(c_fHlPosAngleDeviation - c_angleYaw);
    c_leftFrontHeadlight.Position.y = c_fHlHeight;
    c_leftFrontHeadlight.Position.z = c_posZ - 2.2f + c_fHlPosDistance*sin(c_fHlPosAngleDeviation - c_angleYaw);
    c_rightFrontHeadlight.Position.x = c_posX + c_fHlPosDistance*cos(-c_fHlPosAngleDeviation - c_angleYaw);
    c_rightFrontHeadlight.Position.y = c_fHlHeight;
    c_rightFrontHeadlight.Position.z = c_posZ - 2.2f  + c_fHlPosDistance*sin(-c_fHlPosAngleDeviation - c_angleYaw);

    c_leftRearMarkerlight.Position.x = c_posX - c_rMlPosDistance*cos(-c_rMlPosAngleDeviation - c_angleYaw);
    c_leftRearMarkerlight.Position.y = c_rMlHeight;
    c_leftRearMarkerlight.Position.z = c_posZ - 2.2f - c_rMlPosDistance*sin(-c_rMlPosAngleDeviation - c_angleYaw);
    c_rightRearMarkerlight.Position.x = c_posX - c_rMlPosDistance*cos(c_rMlPosAngleDeviation - c_angleYaw);
    c_rightRearMarkerlight.Position.y = c_rMlHeight;
    c_rightRearMarkerlight.Position.z = c_posZ - 2.2f - c_rMlPosDistance*sin(c_rMlPosAngleDeviation - c_angleYaw);

    c_leftVisionFrontHeadlight.Position.x = c_leftFrontHeadlight.Position.x;
    c_leftVisionFrontHeadlight.Position.y = c_leftFrontHeadlight.Position.y;
    c_leftVisionFrontHeadlight.Position.z = c_leftFrontHeadlight.Position.z;
    c_rightVisionFrontHeadlight.Position.x = c_rightFrontHeadlight.Position.x;
    c_rightVisionFrontHeadlight.Position.y = c_rightFrontHeadlight.Position.y;
    c_rightVisionFrontHeadlight.Position.z = c_rightFrontHeadlight.Position.z;

    c_leftVisionRearMarkerlight.Position.x = c_leftRearMarkerlight.Position.x;
    c_leftVisionRearMarkerlight.Position.y = c_leftRearMarkerlight.Position.y;
    c_leftVisionRearMarkerlight.Position.z = c_leftRearMarkerlight.Position.z;
    c_rightVisionRearMarkerlight.Position.x = c_rightRearMarkerlight.Position.x;
    c_rightVisionRearMarkerlight.Position.y = c_rightRearMarkerlight.Position.y;
    c_rightVisionRearMarkerlight.Position.z = c_rightRearMarkerlight.Position.z;

    vecDir = D3DXVECTOR3(cos(-c_angleYaw + c_fHlLightAngleDeviation), 0.0f, sin(-c_angleYaw + c_fHlLightAngleDeviation));
    D3DXVec3Normalize((D3DXVECTOR3*)&c_leftFrontHeadlight.Direction, &vecDir);
    vecDir = D3DXVECTOR3(cos(-c_angleYaw - c_fHlLightAngleDeviation), 0.0f, sin(-c_angleYaw - c_fHlLightAngleDeviation));
    D3DXVec3Normalize((D3DXVECTOR3*)&c_rightFrontHeadlight.Direction, &vecDir);
    vecDir = D3DXVECTOR3(-cos(-c_angleYaw - c_rMlLightAngleDeviation), -0.3f, -sin(-c_angleYaw - c_rMlLightAngleDeviation));
    D3DXVec3Normalize((D3DXVECTOR3*)&c_leftRearMarkerlight.Direction, &vecDir);
    vecDir = D3DXVECTOR3(-cos(-c_angleYaw + c_rMlLightAngleDeviation), -0.3f, -sin(-c_angleYaw + c_rMlLightAngleDeviation));
    D3DXVec3Normalize((D3DXVECTOR3*)&c_rightRearMarkerlight.Direction, &vecDir);

    d3dd->SetLight(2, &c_leftFrontHeadlight);
    d3dd->SetLight(3, &c_rightFrontHeadlight);
    d3dd->SetLight(4, &c_leftRearMarkerlight);
    d3dd->SetLight(5, &c_rightRearMarkerlight);

    d3dd->SetLight(6, &c_leftVisionFrontHeadlight);
    d3dd->SetLight(7, &c_rightVisionFrontHeadlight);
    d3dd->SetLight(8, &c_leftVisionRearMarkerlight);
    d3dd->SetLight(9, &c_rightVisionRearMarkerlight);

    //car
    for (DWORD i = 0; i < c_numMaterials; i++)
    {
        // Set the material and texture for this subset
        d3dd->SetMaterial(&c_meshMaterials[i]);

        d3dd->SetTexture(0, c_meshTextures[i]);
        // Draw the mesh subset
        c_mesh->DrawSubset(i);
    } 
    

    D3DXMatrixRotationY(&c_rtMatrix, 0);
    D3DXMatrixTranslation(&c_mvMatrix, 0, 0, 0);        //мир обратно:)
    D3DXMatrixMultiply(&c_mgMatrix, &c_rtMatrix, &c_mvMatrix);
    d3dd->SetTransform(D3DTS_WORLD, &c_mgMatrix);
	
	//D3DXMATRIX matTranslate;    // a matrix to store the translation information

	// build a matrix to move the model 12 units along the x-axis and 4 units along the y-axis
	// store it to matTranslate
	//D3DXMatrixTranslation(&matTranslate, 12.0f, 4.0f, 0.0f);

	// tell Direct3D about our matrix
	//d3ddev->SetTransform(D3DTS_WORLD, &matTranslate);
    //d3dd->SetStreamSource(0, vertexBuf, 0, sizeof(vertex));
    //d3dd->SetIndices(indexBuf);    
    //d3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primCount);
    //d3dd->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

    /*D3DMATERIAL9 material;

    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white

    d3dd->SetMaterial(&material);  */  // set the globably-used material to &material

	d3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, filtrationType);
	d3dd->SetSamplerState(0, D3DSAMP_MINFILTER, filtrationType);

    d3dd->SetLight(0, &dLight);    
    d3dd->LightEnable(0, dLightEnabled);   
	d3dd->SetLight(1, &pLight);
    d3dd->LightEnable(1, pLightEnabled);

    d3dd->LightEnable(2, sLightEnabled);
    d3dd->LightEnable(3, sLightEnabled);
    d3dd->LightEnable(4, sLightEnabled);
    d3dd->LightEnable(5, sLightEnabled);

    d3dd->LightEnable(6, sLightEnabled);
    d3dd->LightEnable(7, sLightEnabled);
    d3dd->LightEnable(8, sLightEnabled);
    d3dd->LightEnable(9, sLightEnabled);
    
}


/*
\fn void loadObjectScene()
\brief - Функция загрузки модели объекта из .х файла
*/
void myApp::loadObjectScene()
{
	static bool loaded = false;
	if (loaded)
		return;

    //house
	LPD3DXBUFFER h_materialBuffer = NULL;
	h_numMaterials = 0;
	h_mesh = NULL;
    HRESULT hr = D3DXLoadMeshFromX(h_meshFileName, D3DXMESH_SYSTEMMEM,
		d3dd, NULL,
		&h_materialBuffer, NULL,
        &h_numMaterials, &h_mesh);
	if (FAILED(hr))
		MessageBoxA(0, "hr=D3DXLoadMeshFromX", "ERR", MB_OK);

	// Loading the material buffer
	D3DXMATERIAL* h_d3dxMaterials = (D3DXMATERIAL*)h_materialBuffer->GetBufferPointer();
	// Holding material and texture pointers
	h_meshMaterials = new D3DMATERIAL9[h_numMaterials];
	h_meshTextures = new LPDIRECT3DTEXTURE9[h_numMaterials];
	// Filling material and texture arrays
	for (DWORD i = 0; i < h_numMaterials; i++)
	{
		// Copy the material
		h_meshMaterials[i] = h_d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		h_meshMaterials[i].Ambient = h_meshMaterials[i].Diffuse;

		// Create the texture if it exists - it may not
		
        if (FAILED(D3DXCreateTextureFromFileA(d3dd, (LPCSTR)h_d3dxMaterials[i].pTextureFilename, &h_meshTextures[i])))
            h_meshTextures[i] = NULL;
	}

	h_materialBuffer->Release();


    //car
    LPD3DXBUFFER c_materialBuffer = NULL;
    c_numMaterials = 0;
    c_mesh = NULL;
    hr = D3DXLoadMeshFromX(c_meshFileName, D3DXMESH_SYSTEMMEM,
        d3dd, NULL,
        &c_materialBuffer, NULL,
        &c_numMaterials, &c_mesh);
    if (FAILED(hr))
        MessageBoxA(0, "hr=D3DXLoadMeshFromX", "ERR", MB_OK);

    // Loading the material buffer
    D3DXMATERIAL* c_d3dxMaterials = (D3DXMATERIAL*)c_materialBuffer->GetBufferPointer();
    // Holding material and texture pointers
    c_meshMaterials = new D3DMATERIAL9[c_numMaterials];
    c_meshTextures = new LPDIRECT3DTEXTURE9[c_numMaterials];
    // Filling material and texture arrays
    for (DWORD i = 0; i < c_numMaterials; i++)
    {
        // Copy the material
        c_meshMaterials[i] = c_d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        c_meshMaterials[i].Ambient = c_meshMaterials[i].Diffuse;

        // Create the texture if it exists - it may not

        if (FAILED(D3DXCreateTextureFromFileA(d3dd, (LPCSTR)c_d3dxMaterials[i].pTextureFilename, &c_meshTextures[i])))        
            c_meshTextures[i] = NULL;

        
    }

    c_materialBuffer->Release();

    //ground
    LPD3DXBUFFER g_materialBuffer = NULL;
    g_numMaterials = 0;
    g_mesh = NULL;
    hr = D3DXLoadMeshFromX(g_meshFileName, D3DXMESH_SYSTEMMEM,
        d3dd, NULL,
        &g_materialBuffer, NULL,
        &g_numMaterials, &g_mesh);
    if (FAILED(hr))
        MessageBoxA(0, "hr=D3DXLoadMeshFromX", "ERR", MB_OK);

    // Loading the material buffer
    D3DXMATERIAL* g_d3dxMaterials = (D3DXMATERIAL*)g_materialBuffer->GetBufferPointer();
    // Holding material and texture pointers
    g_meshMaterials = new D3DMATERIAL9[g_numMaterials];
    g_meshTextures = new LPDIRECT3DTEXTURE9[g_numMaterials];
    // Filling material and texture arrays
    for (DWORD i = 0; i < g_numMaterials; i++)
    {
        // Copy the material
        g_meshMaterials[i] = g_d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_meshMaterials[i].Ambient = g_meshMaterials[i].Diffuse;

        // Create the texture if it exists - it may not

        if (FAILED(D3DXCreateTextureFromFileA(d3dd, (LPCSTR)g_d3dxMaterials[i].pTextureFilename, &g_meshTextures[i])))
            g_meshTextures[i] = NULL;


    }

    g_materialBuffer->Release();


	loaded = true;
}


/*
\fn void drawGround()
\brief - Функция создания 3д поверхности из 2д текстуры(в данном случае поврехности земли)
*/
/*void myApp::drawGround()
{
    static bool created = false;
    if (!created)
    {
        createGround();
        created = true;
    }
    mountVertices();

    unsigned int g_vFlags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

    //Set vertex shader
    //d3dd->SetVertexShader(NULL);
    d3dd->SetFVF(g_vFlags);

    d3dd->SetStreamSource(0, vertexBuf, 0, sizeof(vertex));
        
    //Set texture
    d3dd->SetTexture(0, g_d3dTexture);

    d3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    d3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);    
    d3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    
    //Draw image
    d3dd->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
    
}*/


/*
\fn void createGround()
\brief - Фукнция одноразового задания параметров 2д текстуры
*/
/*void myApp::createGround()
{
    unsigned int g_vFlags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

    //Create vertex buffer
    d3dd->CreateVertexBuffer(sizeof(vertex)*4, NULL,
        g_vFlags, D3DPOOL_MANAGED, &vertexBuf, NULL);
    
    
    
    // Load image from file
    if (FAILED(D3DXCreateTextureFromFileExW(d3dd, g_texFileName, 1350, 883, 1, 0,
        D3DFMT_UNKNOWNA8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
        colorkey, &SrcInfo, NULL, &g_d3dTexture))) 
    if (FAILED(D3DXCreateTextureFromFileW(d3dd,   //Direct3D Device
        g_texFileName,       //File Name
        &g_d3dTexture)))
        MessageBoxA(0, "hr=D3DXCreateTextureFromFileExW", "ERR", MB_OK);

    


}*/


/*
\fn void mountVertices()
\brief - Задание параметров вершин(вынесено из createGround из-за громоздкости)
*/
/*void myApp::mountVertices()
{
    vertex* vertices;
    //IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR vertexColour, float rotate
    //Lock the vertex buffer
    //RECT rDest;
    
    int X = -25, Y = -15;
    g_rDest.left = X;
    g_rDest.right = X + 45;
    g_rDest.top = Y;
    g_rDest.bottom = Y + 35;

    vertexBuf->Lock(0, 0, (void**)(&vertices), NULL);
    float rtHeight = -0.12f;            //чтобы дом сильно не уходил под землю
    D3DCOLOR vertexColor = 0xAAFFFFAA;
    //Setup vertices
    //A -0.5f modifier is applied to vertex coordinates to match texture
    //and screen coords. Some drivers may compensate for this
    //automatically, but on others texture alignment errors are introduced
    //More information on this can be found in the Direct3D 9 documentation
    //vertices[0].color = vertexColor;
    vertices[0].x = (float)g_rDest.left - 0.5f;
    vertices[0].y = rtHeight;
    vertices[0].z = (float)g_rDest.top - 0.5f;
    vertices[0].nx = 0.0f;
    vertices[0].ny = 1.0f;
    vertices[0].nz = 0.0f;
    vertices[0].rhw = 1.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;
    
    //vertices[1].color = vertexColor;
    vertices[1].x = (float)g_rDest.right - 0.5f;
    vertices[1].y = rtHeight;
    vertices[1].z = (float)g_rDest.top - 0.5f;
    vertices[1].nx = 0.0f;
    vertices[1].ny = 1.0f;
    vertices[1].nz = 0.0f;
    vertices[1].rhw = 1.0f;
    vertices[1].u = 0.0f;
    vertices[1].v = 1.0f;

    //vertices[2].color = vertexColor;
    vertices[2].x = (float)g_rDest.right - 0.5f;
    vertices[2].y = rtHeight;
    vertices[2].z = (float)g_rDest.bottom - 0.5f;  
    vertices[2].nx = 0.0f;
    vertices[2].ny = 1.0f;
    vertices[2].nz = 0.0f;
    vertices[2].rhw = 1.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;

    //vertices[3].color = vertexColor;
    vertices[3].x = (float)g_rDest.left - 0.5f;
    vertices[3].y = rtHeight;
    vertices[3].z = (float)g_rDest.bottom - 0.5f;    
    vertices[3].nx = 0.0f;
    vertices[3].ny = 1.0f;
    vertices[3].nz = 0.0f;
    vertices[3].rhw = 1.0f;
    vertices[3].u = 1.0f;
    vertices[3].v = 0.0f;

    
    //Unlock the vertex buffer
    vertexBuf->Unlock();

    
}*/


/*
\fn void createLights()
\brief - Функция создания освещения
*/
void myApp::createLights()
{
	static bool created = false;
	if (created)
		return;
	ZeroMemory(&dLight, sizeof(D3DLIGHT9));
	dLight.Type = D3DLIGHT_DIRECTIONAL;//POINT SPOT
	dLight.Diffuse.r = 0.4f;
	dLight.Diffuse.g = 0.5f;
	dLight.Diffuse.b = 1.0f;
	dLight.Diffuse.a = 1.0f;    
	dLight.Range = 1000.0f;

	D3DXVECTOR3 vecDir, vecPos;
	vecDir = D3DXVECTOR3(-2.0f, -5.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&dLight.Direction, &vecDir);

    createCarLights();

	ZeroMemory(&pLight, sizeof(D3DLIGHT9));
	pLight.Type = D3DLIGHT_POINT;//POINT SPOT
	pLight.Diffuse.r = 0.0f;
	pLight.Diffuse.g = 1.0f;
	pLight.Diffuse.b = 0.0f;
	pLight.Diffuse.a = 1.0f;
    pLight.Attenuation0 = 0.0f;
    pLight.Attenuation1 = 0.125f;
    pLight.Attenuation2 = 0.1f;
	pLight.Range = 20.0f;

	//D3DXVECTOR3 vecDir;	
    pLight.Position.x = -3.5f;
    pLight.Position.y = 2.4f;
    pLight.Position.z = -0.0f;
	
	
	created = true;
}

/*
\fn void createCarLights()
\brief - Вспомогательная функция создания освещения(автомобиль)(вынесено в отдельную функцию из-за 4-ех источников света)
*/
void myApp::createCarLights()
{
    float c_lightsVisionRad = 0.4f;

    ZeroMemory(&c_leftFrontHeadlight, sizeof(D3DLIGHT9));
    c_leftFrontHeadlight.Type = D3DLIGHT_SPOT;//POINT SPOT
    c_leftFrontHeadlight.Diffuse.r = 0.78f;
    c_leftFrontHeadlight.Diffuse.g = 0.78f;
    c_leftFrontHeadlight.Diffuse.b = 1.0f;
    c_leftFrontHeadlight.Diffuse.a = 1.0f;
    c_leftFrontHeadlight.Attenuation0 = 0.0f;
    c_leftFrontHeadlight.Attenuation1 = 0.0525f;
    c_leftFrontHeadlight.Attenuation2 = 0.0f;
    c_leftFrontHeadlight.Falloff = 1.0f;
    c_leftFrontHeadlight.Range = 1000.0f;
    c_leftFrontHeadlight.Phi = D3DX_PI / 4;
    c_leftFrontHeadlight.Theta = D3DX_PI / 6;
    

    ZeroMemory(&c_rightFrontHeadlight, sizeof(D3DLIGHT9));
    c_rightFrontHeadlight.Type = D3DLIGHT_SPOT;//POINT SPOT
    c_rightFrontHeadlight.Diffuse.r = 0.78f;
    c_rightFrontHeadlight.Diffuse.g = 0.78f;
    c_rightFrontHeadlight.Diffuse.b = 1.0f;
    c_rightFrontHeadlight.Diffuse.a = 1.0f;
    c_rightFrontHeadlight.Attenuation0 = 0.0f;
    c_rightFrontHeadlight.Attenuation1 = 0.0525f;
    c_rightFrontHeadlight.Attenuation2 = 0.0f;
    c_rightFrontHeadlight.Falloff = 1.0f;
    c_rightFrontHeadlight.Range = 1000.0f;
    c_rightFrontHeadlight.Phi = D3DX_PI / 4;
    c_rightFrontHeadlight.Theta = D3DX_PI / 6;


    ZeroMemory(&c_leftRearMarkerlight, sizeof(D3DLIGHT9));
    c_leftRearMarkerlight.Type = D3DLIGHT_SPOT;//POINT SPOT
    c_leftRearMarkerlight.Diffuse.r = 1.0f;
    c_leftRearMarkerlight.Diffuse.g = 0.0f;
    c_leftRearMarkerlight.Diffuse.b = 0.0f;
    c_leftRearMarkerlight.Diffuse.a = 0.5f;
    c_leftRearMarkerlight.Attenuation0 = 0.0f;
    c_leftRearMarkerlight.Attenuation1 = 0.325f;
    c_leftRearMarkerlight.Attenuation2 = 0.0f;
    c_leftRearMarkerlight.Falloff = 0.4f;
    c_leftRearMarkerlight.Range = 1000.0f;
    c_leftRearMarkerlight.Phi = D3DX_PI / 3;//3
    c_leftRearMarkerlight.Theta = D3DX_PI / 3;//9

    ZeroMemory(&c_rightRearMarkerlight, sizeof(D3DLIGHT9));
    c_rightRearMarkerlight.Type = D3DLIGHT_SPOT;//POINT SPOT
    c_rightRearMarkerlight.Diffuse.r = 1.0f;
    c_rightRearMarkerlight.Diffuse.g = 0.0f;
    c_rightRearMarkerlight.Diffuse.b = 0.0f;
    c_rightRearMarkerlight.Diffuse.a = 0.5f;
    c_rightRearMarkerlight.Attenuation0 = 0.0f;
    c_rightRearMarkerlight.Attenuation1 = 0.325f;
    c_rightRearMarkerlight.Attenuation2 = 0.0f;
    c_rightRearMarkerlight.Falloff = 0.4f;
    c_rightRearMarkerlight.Range = 1000.0f;
    c_rightRearMarkerlight.Phi = D3DX_PI / 3;
    c_rightRearMarkerlight.Theta = D3DX_PI / 9;


    //==========

    ZeroMemory(&c_leftVisionFrontHeadlight, sizeof(D3DLIGHT9));
    c_leftVisionFrontHeadlight.Type = D3DLIGHT_POINT;//POINT SPOT
    c_leftVisionFrontHeadlight.Diffuse.r = c_leftFrontHeadlight.Diffuse.r;
    c_leftVisionFrontHeadlight.Diffuse.g = c_leftFrontHeadlight.Diffuse.g;
    c_leftVisionFrontHeadlight.Diffuse.b = c_leftFrontHeadlight.Diffuse.b;
    c_leftVisionFrontHeadlight.Diffuse.a = c_leftFrontHeadlight.Diffuse.a;
    c_leftVisionFrontHeadlight.Attenuation0 = 0.0f;
    c_leftVisionFrontHeadlight.Attenuation1 = 0.0f;
    c_leftVisionFrontHeadlight.Attenuation2 = 0.0f;
    c_leftVisionFrontHeadlight.Range = c_lightsVisionRad;

    ZeroMemory(&c_rightVisionFrontHeadlight, sizeof(D3DLIGHT9));
    c_rightVisionFrontHeadlight.Type = D3DLIGHT_POINT;//POINT SPOT
    c_rightVisionFrontHeadlight.Diffuse.r = c_rightFrontHeadlight.Diffuse.r;
    c_rightVisionFrontHeadlight.Diffuse.g = c_rightFrontHeadlight.Diffuse.g;
    c_rightVisionFrontHeadlight.Diffuse.b = c_rightFrontHeadlight.Diffuse.b;
    c_rightVisionFrontHeadlight.Diffuse.a = c_rightFrontHeadlight.Diffuse.a;
    c_rightVisionFrontHeadlight.Attenuation0 = 0.0f;
    c_rightVisionFrontHeadlight.Attenuation1 = 0.0f;
    c_rightVisionFrontHeadlight.Attenuation2 = 0.0f;
    c_rightVisionFrontHeadlight.Range = c_lightsVisionRad;

    ZeroMemory(&c_leftVisionRearMarkerlight, sizeof(D3DLIGHT9));
    c_leftVisionRearMarkerlight.Type = D3DLIGHT_POINT;//POINT SPOT
    c_leftVisionRearMarkerlight.Diffuse.r = c_leftRearMarkerlight.Diffuse.r;
    c_leftVisionRearMarkerlight.Diffuse.g = c_leftRearMarkerlight.Diffuse.g;
    c_leftVisionRearMarkerlight.Diffuse.b = c_leftRearMarkerlight.Diffuse.b;
    c_leftVisionRearMarkerlight.Diffuse.a = c_leftRearMarkerlight.Diffuse.a;
    c_leftVisionRearMarkerlight.Attenuation0 = 0.0f;
    c_leftVisionRearMarkerlight.Attenuation1 = 0.0f;
    c_leftVisionRearMarkerlight.Attenuation2 = 0.0f;
    c_leftVisionRearMarkerlight.Range = c_lightsVisionRad;

    ZeroMemory(&c_rightVisionRearMarkerlight, sizeof(D3DLIGHT9));
    c_rightVisionRearMarkerlight.Type = D3DLIGHT_POINT;//POINT SPOT
    c_rightVisionRearMarkerlight.Diffuse.r = c_rightRearMarkerlight.Diffuse.r;
    c_rightVisionRearMarkerlight.Diffuse.g = c_rightRearMarkerlight.Diffuse.g;
    c_rightVisionRearMarkerlight.Diffuse.b = c_rightRearMarkerlight.Diffuse.b;
    c_rightVisionRearMarkerlight.Diffuse.a = c_rightRearMarkerlight.Diffuse.a;
    c_rightVisionRearMarkerlight.Attenuation0 = 0.0f;
    c_rightVisionRearMarkerlight.Attenuation1 = 0.0f;
    c_rightVisionRearMarkerlight.Attenuation2 = 0.0f;
    c_rightVisionRearMarkerlight.Range = c_lightsVisionRad;
}
/*
\fn void update()
\brief - Генерация данных(только при 1ом вызове) и заполнение буфферов вершин и индексов
*/


/*void myApp::generateObject()
{
    static bool mounted = false;     
    static gmtr_func gf(0.8f, 0.3f, 0.4f, 0.0f, 0.0f, 0.0f, 200);     //конструктор только записывает некоторые исходные данные(заполнение буфферов по смпец вызову)
    if (!mounted)           //сам объект хранится в памяти, на каждом кадре заполняются только буффера
    {
        gf.setD3DDevice(d3dd);
        gf.bufferizeObject();
        mounted = true;
    }
    vertexBuf = gf.getVertexBuffer();
    indexBuf = gf.getIndexBuffer();
    vertexCount = gf.getVertexCount();
    indexCount = gf.getIndexCount();
    primCount = gf.getPrimCount();
    
}*/


/*
\fn void printData(std::string text, int x, int y, int width, int height)
\brief - Вывод строки с данными на экран(дебаг окно)(временно не используется)
\param text -
\param x -
\param y -
\param width -
\param height -
*/
/*void myApp::printData(std::string text, int x, int y, int width, int height)
{
	

}*/


/*
\fn void dbgInfoBox(float X, float Y, float Z, float LX, float LY, float LZ)
\brief - 
\param X -
\param Y -
\param Z -
\param LX -
\param LY -
\param LZ - 
*/
/*void myApp::dbgInfoBox(float X, float Y, float Z, float LX, float LY, float LZ)
{
	std::stringstream sstr;
	sstr << "avg fps : " << oFPS << std::endl << "Pos : [" << X << ", " << Y << ", " << Z << " ]" << std::endl << "LA : [" << LX << ", " << LY << ", " << LZ << " ]";
	std::string dstr = sstr.str();
	printData(dstr, 20, 15, 100, 100);
}*/



