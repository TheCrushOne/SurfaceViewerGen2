#ifndef __MYLAPP_H__632619789336566350
#define __MYLAPP_H__632619789336566350

/**
  @file     myApp.h
  @brief    Class myApp definition
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include "library/cglApp.h"
#include "library/cglD3D.h"
#include "utils/gmtr.h"   //utils уже там есть
#include <d3d9.h>

#include <d3dx9.h>	//is его не видит...


// *******************************************************************
// defines & constants

// *******************************************************************
// classes 


#include "utils/camera.h"
#include "utils/infoboard.h"



// Application class
/*
\class myApp
\brief - ќсновной класс дл€ работы
*/
class myApp : public cglApp
{
	
public:
  // Constructor
  myApp(int nW, int nH, void* hInst, int nCmdShow) 
    : cglApp(nW, nH, hInst, nCmdShow)
    , m_nPrevMouseX(-100)
    , m_nPrevMouseY(-100)
  {
	  //g_camera = new vb_camera(d3dd);
        d3dd = getD3Dd(*this);
        
      g_camera = new vb_camera(d3dd);
        g_info = new cb_infoboard(oD3D9, d3dd, g_camera);
#ifdef _DEBUG  
        d3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);      //дебаг
        d3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);         //сквозной просвет
#else
        d3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_FORCE_DWORD); 
		d3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        //d3dd->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
#endif
        d3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        d3dd->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
        d3dd->SetRenderState(D3DRS_ALPHAREF, (DWORD)8);
        //d3dd->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        
        d3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		d3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
        d3dd->SetRenderState(D3DRS_LIGHTING, true);                  //fill без света
        //d3dd->SetRenderState(D3DRS_FOGENABLE, true);
		//d3dd->SetRenderState(D3DRS_STENCILENABLE, true);
        //d3dd->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
        //d3dd->SetRenderState(D3DRS_POINTSCALEENABLE, true);
        //d3dd->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        d3dd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); 
        d3dd->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);        //по умолчанию вроде та же фукнци€
        //d3dd->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(80, 80, 80)); //аварийный свет

        /*L"17755_Audi_R8_ASCII.x"*/// - резерв(слишком большой масштаб)
        h_meshFileName = L"fw43_lowpoly_n1.x";
        c_meshFileName = /*L"17755_Audi_R8_ASCII.x";*/L"Lamborghini.x";
        g_meshFileName = /*L"39pr.png"*/L"tera.x"/*L"door35.jpg"*/;

		filtrationType = D3DTEXF_LINEAR;
		//cameraMod = CM_ORBITAL;

        vertexBuf = 0;
        m_nClearColor = 0x00000000;     //пока что общий фон черный

        dLightEnabled = true;
        sLightEnabled = false;
        pLightEnabled = false;

        
		//eyeX = 1000.6f, eyeY = 1900.4f, eyeZ = 1000.6f, LookatX = 0.0f, LookatY = 0.0f, LookatZ = 0.0f;
        //eyeX = 17.6f, eyeY = 32.4f, eyeZ = -34.6f, LookatX = 0.0f, LookatY = 0.0f, LookatZ = 0.0f;


        for (int i = 0; i < MAX_KEYS; i++)
            m_keysPressed[i] = false;

        //mountData();
  }
  // Destructor
  virtual ~myApp() {}
  // This function performs input processing. Returns true if input is handled
  virtual bool processInput(unsigned int nMsg, int wParam, long lParam);

  
protected:
  virtual void update();
  virtual void renderInternal();
  
private:
  enum
  {
    MAX_KEYS = 128
  };
 
  int   m_nPrevMouseX;  
  int   m_nPrevMouseY;
  bool  m_keysPressed[MAX_KEYS];
  
  

  
  //geometry:
  //float eyeX, eyeY, eyeZ, LookatX, LookatY, LookatZ;





  //void rotate(float dx, float dy);
  //void zoom(float dr);
  //void strafe(float dw, float dh);

  //========================
  unsigned int indexCount, vertexCount, primCount, vFlags;
  LPDIRECT3DDEVICE9 d3dd;
  //IDirect3DIndexBuffer9  *indexBuf;
  IDirect3DVertexBuffer9 *vertexBuf;

  //void printData(std::string text, int x, int y, int width, int height);
  //void dbgInfoBox(float, float, float, float, float, float);
  void mountData();  
  
  DWORD filtrationType;
  DWORD cameraMod;

  vb_camera *g_camera;
  cb_infoboard *g_info;

  DWORD h_numMaterials;
  LPD3DXMESH h_mesh;
  D3DMATERIAL9 *h_meshMaterials;
  LPDIRECT3DTEXTURE9 *h_meshTextures;
  DWORD c_numMaterials;
  LPD3DXMESH c_mesh;
  D3DMATERIAL9 *c_meshMaterials;
  LPDIRECT3DTEXTURE9 *c_meshTextures;
  DWORD g_numMaterials;
  LPD3DXMESH g_mesh;
  D3DMATERIAL9 *g_meshMaterials;
  LPDIRECT3DTEXTURE9 *g_meshTextures;
  //IDirect3DTexture9 *g_d3dTexture;
  //RECT g_rDest;
  LPCWSTR h_meshFileName, c_meshFileName, g_meshFileName;

  D3DLIGHT9 dLight;
  D3DLIGHT9 c_leftFrontHeadlight, c_rightFrontHeadlight;
  D3DLIGHT9 c_leftVisionFrontHeadlight, c_rightVisionFrontHeadlight;
  D3DLIGHT9 c_leftRearMarkerlight, c_rightRearMarkerlight;
  D3DLIGHT9 c_leftVisionRearMarkerlight, c_rightVisionRearMarkerlight;
  D3DLIGHT9 pLight;
  bool dLightEnabled, sLightEnabled, pLightEnabled;
  void loadObjectScene();  
  void drawGround();
  void createGround();
  void mountVertices();
  void createLights();
  void createCarLights();

  //IDirect3DIndexBuffer9 *createIndexBuffer();
  //IDirect3DVertexBuffer9 *createVertexBuffer();

  //void cameraMove(float, float, float, float, float);	//смещени€ : первые 3 - вращение и зум в режиме CO; последние 2 - стрейф вправо/влево и подъем.


  
  /*
  \fn getD3Dd(const myApp &m_a)
  \brief функци€ вытаскивает указатель на d3d device(из protected зоны класса предка)
  \param m_a - —сылка на экземпл€р класса myApp
  \return - указатель на direct3d устройство
  */
  
  static LPDIRECT3DDEVICE9 getD3Dd(const myApp &m_a) { return (m_a.m_pD3D)->getDevice(); }  
};





// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350
