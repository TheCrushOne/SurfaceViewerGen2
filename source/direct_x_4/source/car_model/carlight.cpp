/**
@file     carlight.cpp
@brief    Car light processing class
@date     Created on 24/10/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "carlight.h"
#include "engine/engine.h"
#include "utils/light.h"
#include "engine\shader_processing\shaderunit.h"

#define EFFECT_LIGHT_CREATE_INSTANT_CAST(bl, x, diff, a0, a1, a2, f, ra, p, th) \
    ZeroMemory(&x, sizeof(D3DLIGHT9)); x.Type = D3DLIGHT_SPOT; x.Diffuse = diff;  x.Specular = diff; /*x.Ambient = diff;*/ \
    x.Attenuation0 = a0; x.Attenuation1 = a1; x.Attenuation2 = a2; \
    x.Falloff = f; x.Range = ra; x.Phi = p; x.Theta = th; \
    bl ? m_lightDispatcher->SetLight(&x, OBL_HEADLIGHT): 1; \
    m_lightIndexes[k++] = m_shaderunit->SetSLight(&x);

#define VISION_LIGHT_CREATE_INSTANT_CAST(bl, x, diff, a0, a1, a2, ra) \
    ZeroMemory(&x, sizeof(D3DLIGHT9)); x.Type = D3DLIGHT_POINT; x.Diffuse = diff;  \
    x.Attenuation0 = a0; x.Attenuation1 = a1; x.Attenuation2 = a2; \
    x.Range = ra; bl ? m_lightDispatcher->SetLight(&x, OBL_HEADLIGHT) : 1; \
    m_lightIndexes[k++] = m_shaderunit->SetCVLight(&x);

#define NEON_LIGHT_CREATE_INSTANT_CAST(x, dif, a0, a1, a2, ff, ra, p, th, csli, ind, k) \
    ZeroMemory(&x, sizeof(D3DLIGHT9)); x.Type = D3DLIGHT_POINT; x.Diffuse = dif;  x.Ambient = dif; \
    x.Attenuation0 = a0; x.Attenuation1 = a1; x.Attenuation2 = a2; \
    x.Falloff = ff; x.Range = ra; x.Phi = p; x.Theta = th; /*m_lightDispatcher->setLight(&x);*/ /*csli[ind][k++] = m_shaderunit->setLight(&x)*/;

#define NEON_LIGHT_KP_MOUNT_MULTI_CAST(s1, s2, s3, k) \
    k = m_shaderunit->SetKPLight(&s1, &s2, &s3);


const float CarLight::DEFAULT_FDHLPOSDISTANCE = 2.575f;  //2.57
const float CarLight::DEFAULT_FBHLPOSDISTANCE = 2.535f;  //2.53
const float CarLight::DEFAULT_FSLPOSDISTANCE = 2.3f;     //2.3
const float CarLight::DEFAULT_RMLPOSDISTANCE = 3.15f;    //3.15
const float CarLight::DEFAULT_RBLPOSDISTANCE = 3.22f;    //3.19
const float CarLight::DEFAULT_RRLPOSDISTANCE = 3.19f;
const float CarLight::DEFAULT_RSLPOSDISTANCE = 3.13f;

const float CarLight::DEFAULT_FDHLPOSANGLEDEVIATION = D3DX_PI / 7.835f;  //7.835
const float CarLight::DEFAULT_FBHLPOSANGLEDEVIATION = D3DX_PI / 9.0f;    //9.0
const float CarLight::DEFAULT_FSLPOSANGLEDEVIATION = D3DX_PI / 8.0f;     //8.0
const float CarLight::DEFAULT_RMLPOSANGLEDEVIATION = D3DX_PI / 11.3f;    //10.8
const float CarLight::DEFAULT_RBLPOSANGLEDEVIATION = D3DX_PI / 16.0f;    //16.0
const float CarLight::DEFAULT_RRLPOSANGLEDEVIATION = D3DX_PI / 13.2f;
const float CarLight::DEFAULT_RSLPOSANGLEDEVIATION = D3DX_PI / 10.2f;

const float CarLight::DEFAULT_FDHLHEIGHT = 0.73f;    //0.73
const float CarLight::DEFAULT_FBHLHEIGHT = 0.68f;    //0.68
const float CarLight::DEFAULT_FSLHEIGHT = 0.8f;      //0.8
const float CarLight::DEFAULT_RMLHEIGHT = 1.05f;     //0.98
const float CarLight::DEFAULT_RBLHEIGHT = 0.99f;     //0.99
const float CarLight::DEFAULT_RRLHEIGHT = 0.98f;
const float CarLight::DEFAULT_RSLHEIGHT = 0.98f;

const float CarLight::DEFAULT_FDHLLIGHTANGLEDEVIATION = D3DX_PI / 20;
const float CarLight::DEFAULT_FBHLLIGHTANGLEDEVIATION = D3DX_PI / 20;
const float CarLight::DEFAULT_FSLLIGHTANGLEDEVIATION = D3DX_PI / 20;
const float CarLight::DEFAULT_RMLLIGHTANGLEDEVIATION = D3DX_PI / 64;
const float CarLight::DEFAULT_RBLLIGHTANGLEDEVIATION = D3DX_PI / 64;
const float CarLight::DEFAULT_RRLLIGHTANGLEDEVIATION = D3DX_PI / 64;
const float CarLight::DEFAULT_RSLLIGHTANGLEDEVIATION = D3DX_PI / 64;

const float CarLight::DEFAULT_VISION_LIGHT_RAD = 0.1f;//0.07f;
const float CarLight::DEFAULT_VISION_COLLIDED_LIGHT_RAD = 0.25f;
const DWORD CarLight::DEFAULT_CAR_LIGHT_NUM = 40;

const D3DCOLORVALUE CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT = { 0.78f, 0.78f, 1.0f, 1.0f };
const D3DCOLORVALUE CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT = { 0.78f, 0.78f, 1.0f, 1.0f };
const D3DCOLORVALUE CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT = { 1.0f, 0.56f, 0.0f, 1.0f };
const D3DCOLORVALUE CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT = { 1.0f, 0.0f, 0.0f, 1.0f };
const D3DCOLORVALUE CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT = { 1.0f, 0.0f, 0.0f, 1.0f };
const D3DCOLORVALUE CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT = { 1.0f, 1.0f, 1.0f, 1.0f };
const D3DCOLORVALUE CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT = { 1.0f, 0.56f, 0.0f, 1.0f };

const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION1 = 1.0f;
const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION1 = 1.0f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION1 = 0.225f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION1 = 0.225f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION1 = 0.335f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION1 = 0.375f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION1 = 0.225f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION2 = 0.0f;

const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_FALLOFF = 3.4f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_FALLOFF = 20.4f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT_FALLOFF = 3.8f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT_FALLOFF = 0.4f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT_FALLOFF = 0.4f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT_FALLOFF = 3.4f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT_FALLOFF = 3.8f;

const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_RANGE = 20.0f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_RANGE = 20.0f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT_RANGE = 8.0f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT_RANGE = 20.0f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT_RANGE = 20.0f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT_RANGE = 20.0f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT_RANGE = 8.0f;

const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_PHI = D3DX_PI / 4;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_PHI = D3DX_PI / 6;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT_PHI = D3DX_PI / 3;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT_PHI = D3DX_PI / 3;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT_PHI = D3DX_PI / 3;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT_PHI = D3DX_PI / 4;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT_PHI = D3DX_PI / 3;

const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_THETA = D3DX_PI / 6;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_THETA = D3DX_PI / 8;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_EFFECT_THETA = D3DX_PI / 3;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_EFFECT_THETA = D3DX_PI / 3;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_EFFECT_THETA = D3DX_PI / 3;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_EFFECT_THETA = D3DX_PI / 6;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_EFFECT_THETA = D3DX_PI / 3;


const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION1 = 0.001f;
const float CarLight::DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION1 = 0.001f;
const float CarLight::DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION1 = 0.001f;
const float CarLight::DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION1 = 0.001f;
const float CarLight::DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION1 = 0.001f;
const float CarLight::DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION1 = 0.001f;
const float CarLight::DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION2 = 0.0f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION0 = 0.0f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION1 = 0.001f;
const float CarLight::DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION2 = 0.0f;

//=======================================================

const DWORD NeonLight::DEFAULT_LIGHT_DENSITY = 3;
const D3DCOLORVALUE NeonLight::DEFAULT_NEON_COLOR = { 1.0f, 1.0f, 0.0f, 0.5f };
const float NeonLight::DEFAULT_NEON_ATTENUATION0 = 0.0f;
const float NeonLight::DEFAULT_NEON_ATTENUATION1 = 0.025f;
const float NeonLight::DEFAULT_NEON_ATTENUATION2 = 0.525f;
const float NeonLight::DEFAULT_NEON_FALLOFF = 1.0f;
const float NeonLight::DEFAULT_NEON_RANGE = 0.4f;
const float NeonLight::DEFAULT_NEON_PHI = D3DX_PI / 1.19f;       //н/д
const float NeonLight::DEFAULT_NEON_THETA = D3DX_PI / 1.19f;
const DWORD NeonLight::MAX_LINE_LIGHT = 200;    

const bool NeonLight::DEFAULT_LIGHT_ENABLE = true;

//==============================//Конструктор(вынесен из .h после введения диспетчера)
CarLight::CarLight(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev, DWORD stIndex)
   : m_engine(eng)
   , m_lightDispatcher(eng->GetLight())
   , m_shaderunit(eng->GetShader())
   , m_d3dd(dev)
   , m_startLightIndex(stIndex)
   , m_lightIndexes(new DWORD[DEFAULT_CAR_LIGHT_NUM])
{
    initService();
}

CarLight::~CarLight()
{
    delete m_lightIndexes;
}

NeonLight::NeonLight(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev)
   : m_engine(eng)
   , m_shaderunit(eng->GetShader())
   , m_d3dd(dev)
{
    m_shaderLightIndex = new DWORD*[4];
    for (DWORD i = 0; i < 4; i++)
        m_shaderLightIndex[i] = new DWORD[MAX_LINE_LIGHT];
    initService();
}

NeonLight::~NeonLight()
{
   for (DWORD j = 0; j < 4; j++)
   {
      delete m_frontLight[j];
      delete m_fHlLSPosX[j];
      delete m_fHlLSPosZ[j];
      delete m_fHlLSHeight[j];
      delete m_shaderLightIndex[j];
   }
   delete m_frontLight;
   delete m_fHlLSPosX;
   delete m_fHlLSPosZ;
   delete m_fHlLSHeight;
   delete m_shaderLightIndex;
   delete m_fLSCount;

   delete m_lCPPosDistance;
   delete m_lCPPosAngleDeviation;
   delete m_lCPHeight;
}
//==============================//блок внешнего управления//

// Обновление света(на каждом кадре)
void CarLight::Update(D3DXVECTOR3 cpos, float cay)
{
   m_carPosX = cpos.x;
   m_carPosY = cpos.y;
   m_carPosZ = cpos.z;
   m_carAngleYaw = cay - D3DX_PI/2.0f;
   setPosition();
   m_neon->Update(cpos, cay);        //неон(только при шейдерной обработке)
}


// Перенаправление на переключение тумблера неонового освещения
void CarLight::NeonRedirect()
{
   m_neon->ToggleState();
}

// Переключение тумблера основного освещения
void CarLight::ToggleState(CLTYPE type)
{
    switch (type)
    {
    case CBN_FRONT_DIPPED_HEADLIGHTS:
        m_fDHlEnabled = !m_fDHlEnabled;
        m_lightDispatcher->SetEnabled(OBL_HEADLIGHT, m_fDHlEnabled);
        break;
    case CBN_FRONT_BEAM_HEADLIGHTS:
        m_fBHlEnabled = !m_fBHlEnabled;
        break;
    case CBN_REAR_MARKER_LIGHTS:        //rm будут работать на одном бинде без шейдеров, а тут с двумя
        m_rMlEnabled = !m_rMlEnabled;
        break;
    case CBN_REAR_BRAKE_LIGHTS:
        m_rBlEnabled = !m_rBlEnabled;
        break;
    case CBN_REAR_REVERSE_LIGHTS:
        m_rRlEnabled = !m_rRlEnabled;
        break;
    case CBN_LEFT_SIGNAL_LIGHT:
        m_lSlEnabled = !m_lSlEnabled;
        if (m_lSlEnabled)
            m_rSlEnabled = false;
        break;
    case CBN_RIGHT_SIGNAL_LIGHT:
        m_rSlEnabled = !m_rSlEnabled;
        if (m_rSlEnabled)
            m_lSlEnabled = false;
        break;
    };
    //m_lightEnabled = !m_lightEnabled;
}

// Переключение тумблера основного освещения
void CarLight::ToggleState(CLTYPE type, bool status)
{
    switch (type)
    {
    case CBN_FRONT_DIPPED_HEADLIGHTS:
        m_fDHlEnabled = status;
        m_lightDispatcher->SetEnabled(OBL_HEADLIGHT, m_fDHlEnabled);
        break;
    case CBN_FRONT_BEAM_HEADLIGHTS:
        m_fBHlEnabled = status;
        break;
    case CBN_REAR_MARKER_LIGHTS:        //rm будут работать на одном бинде без шейдеров, а тут с двумя
        m_rMlEnabled = status;
        break;
    case CBN_REAR_BRAKE_LIGHTS:
        m_rBlEnabled = status;
        break;
    case CBN_REAR_REVERSE_LIGHTS:
        m_rRlEnabled = status;
        break;
    case CBN_LEFT_SIGNAL_LIGHT:
        m_lSlEnabled = status;
        break;
    case CBN_RIGHT_SIGNAL_LIGHT:
        m_rSlEnabled = status;
        break;
    };
    //m_lightEnabled = !m_lightEnabled;
}


//==============================

// Обновление неон света(на каждом кадре)
// cay - Угол рыскания машины(angle yaw)
void NeonLight::Update(D3DXVECTOR3 cpos, float cay)
{
    m_carPosX = cpos.x;
    m_carPosY = cpos.y;
    m_carPosZ = cpos.z;
    m_carAngleYaw = cay - D3DX_PI/2;
    setPosition();    //отключено до введения шейдеров и разбиения по бленду
}

// Переключения тумблера неонового освещения
void NeonLight::ToggleState()
{
   m_lightEnabled = !m_lightEnabled;
}


//==============================

// Инициализация переменных параметров класса освещения машины(пока что по default'у)
void CarLight::initService()
{
   m_fDHlPosDistance = DEFAULT_FDHLPOSDISTANCE;
   m_fBHlPosDistance = DEFAULT_FBHLPOSDISTANCE;
   m_fSlPosDistance = DEFAULT_FSLPOSDISTANCE;
   m_rMlPosDistance = DEFAULT_RMLPOSDISTANCE;
   m_rBlPosDistance = DEFAULT_RBLPOSDISTANCE;
   m_rRlPosDistance = DEFAULT_RRLPOSDISTANCE;
   m_rSlPosDistance = DEFAULT_RSLPOSDISTANCE;

   m_fDHlPosAngleDeviation = DEFAULT_FDHLPOSANGLEDEVIATION;
   m_fBHlPosAngleDeviation = DEFAULT_FBHLPOSANGLEDEVIATION;
   m_fSlPosAngleDeviation = DEFAULT_FSLPOSANGLEDEVIATION;
   m_rMlPosAngleDeviation = DEFAULT_RMLPOSANGLEDEVIATION;
   m_rBlPosAngleDeviation = DEFAULT_RBLPOSANGLEDEVIATION;
   m_rRlPosAngleDeviation = DEFAULT_RRLPOSANGLEDEVIATION;
   m_rSlPosAngleDeviation = DEFAULT_RSLPOSANGLEDEVIATION;

   m_fDHlHeight = DEFAULT_FDHLHEIGHT;
   m_fBHlHeight = DEFAULT_FBHLHEIGHT;
   m_fSlHeight = DEFAULT_FSLHEIGHT;
   m_rMlHeight = DEFAULT_RMLHEIGHT;
   m_rBlHeight = DEFAULT_RBLHEIGHT;
   m_rRlHeight = DEFAULT_RRLHEIGHT;
   m_rSlHeight = DEFAULT_RSLHEIGHT;

   m_fDHlLightAngleDeviation = DEFAULT_FDHLLIGHTANGLEDEVIATION;
   m_fBHlLightAngleDeviation = DEFAULT_FBHLLIGHTANGLEDEVIATION;
   m_fSlLightAngleDeviation = DEFAULT_FSLLIGHTANGLEDEVIATION;
   m_rMlLightAngleDeviation = DEFAULT_RMLLIGHTANGLEDEVIATION;
   m_rBlLightAngleDeviation = DEFAULT_RBLLIGHTANGLEDEVIATION;
   m_rRlLightAngleDeviation = DEFAULT_RRLLIGHTANGLEDEVIATION;
   m_rSlLightAngleDeviation = DEFAULT_RSLLIGHTANGLEDEVIATION;

   m_frontDippedHeadlightEffect = DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT;
   m_frontBeamHeadlightEffect = DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT;
   m_frontSignallightEffect = DEFAULT_FRONT_SIGNALLIGHT_EFFECT;
   m_rearMarkerLightEffect = DEFAULT_REAR_MARKERLIGHT_EFFECT;
   m_rearBrakelightEffect = DEFAULT_REAR_BRAKELIGHT_EFFECT;
   m_rearReverselightEffect = DEFAULT_REAR_REVERSELIGHT_EFFECT;
   m_rearSignallightEffect = DEFAULT_REAR_SIGNALLIGHT_EFFECT;

   m_carLightNum = DEFAULT_CAR_LIGHT_NUM;

   m_fDHlEnabled = false, m_fBHlEnabled = false;
   m_rMlEnabled = false, m_rBlEnabled = false;
   m_rRlEnabled = false, m_lSlEnabled = false;
   m_rSlEnabled = false;

   DWORD k = 0;

   EFFECT_LIGHT_CREATE_INSTANT_CAST(true, m_leftFrontDippedHeadlight, m_frontDippedHeadlightEffect, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION0, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION1, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION2, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_FALLOFF, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_RANGE, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_PHI, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_THETA);
    
   EFFECT_LIGHT_CREATE_INSTANT_CAST(true, m_rightFrontDippedHeadlight, m_frontDippedHeadlightEffect, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION0, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION1, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION2, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_FALLOFF, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_RANGE, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_PHI, DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_THETA);
      
   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_leftFrontBeamHeadlight, m_frontBeamHeadlightEffect, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION0, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION1, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION2, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_FALLOFF, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_RANGE, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_PHI, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_rightFrontBeamHeadlight, m_frontBeamHeadlightEffect, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION0, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION1, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION2, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_FALLOFF, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_RANGE, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_PHI, DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_leftFrontSignallight, m_frontSignallightEffect, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION0, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION1, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION2, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_FALLOFF, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_RANGE, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_PHI, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_rightFrontSignallight, m_frontSignallightEffect, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION0, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION1, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION2, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_FALLOFF, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_RANGE, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_PHI, DEFAULT_FRONT_SIGNALLIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(true, m_leftRearMarkerlight, m_rearMarkerLightEffect, DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_MARKERLIGHT_EFFECT_FALLOFF, DEFAULT_REAR_MARKERLIGHT_EFFECT_RANGE, DEFAULT_REAR_MARKERLIGHT_EFFECT_PHI, DEFAULT_REAR_MARKERLIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(true, m_rightRearMarkerlight, m_rearMarkerLightEffect, DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_MARKERLIGHT_EFFECT_FALLOFF, DEFAULT_REAR_MARKERLIGHT_EFFECT_RANGE, DEFAULT_REAR_MARKERLIGHT_EFFECT_PHI, DEFAULT_REAR_MARKERLIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_leftRearBrakelight, m_rearBrakelightEffect, DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_BRAKELIGHT_EFFECT_FALLOFF, DEFAULT_REAR_BRAKELIGHT_EFFECT_RANGE, DEFAULT_REAR_BRAKELIGHT_EFFECT_PHI, DEFAULT_REAR_BRAKELIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_rightRearBrakelight, m_rearBrakelightEffect, DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_BRAKELIGHT_EFFECT_FALLOFF, DEFAULT_REAR_BRAKELIGHT_EFFECT_RANGE, DEFAULT_REAR_BRAKELIGHT_EFFECT_PHI, DEFAULT_REAR_BRAKELIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_leftRearReverselight, m_rearReverselightEffect, DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_REVERSELIGHT_EFFECT_FALLOFF, DEFAULT_REAR_REVERSELIGHT_EFFECT_RANGE, DEFAULT_REAR_REVERSELIGHT_EFFECT_PHI, DEFAULT_REAR_REVERSELIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_rightRearReverselight, m_rearReverselightEffect, DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_REVERSELIGHT_EFFECT_FALLOFF, DEFAULT_REAR_REVERSELIGHT_EFFECT_RANGE, DEFAULT_REAR_REVERSELIGHT_EFFECT_PHI, DEFAULT_REAR_REVERSELIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_leftRearSignallight, m_rearSignallightEffect, DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_SIGNALLIGHT_EFFECT_FALLOFF, DEFAULT_REAR_SIGNALLIGHT_EFFECT_RANGE, DEFAULT_REAR_SIGNALLIGHT_EFFECT_PHI, DEFAULT_REAR_SIGNALLIGHT_EFFECT_THETA);

   EFFECT_LIGHT_CREATE_INSTANT_CAST(false, m_rightRearSignallight, m_rearSignallightEffect, DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION0, DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION1, DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION2, DEFAULT_REAR_SIGNALLIGHT_EFFECT_FALLOFF, DEFAULT_REAR_SIGNALLIGHT_EFFECT_RANGE, DEFAULT_REAR_SIGNALLIGHT_EFFECT_PHI, DEFAULT_REAR_SIGNALLIGHT_EFFECT_THETA);

   //==========

   VISION_LIGHT_CREATE_INSTANT_CAST(true, m_leftVisionFrontDippedHeadlight, m_leftFrontDippedHeadlight.Diffuse, DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION0, DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION1, DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_LIGHT_RAD);
    
   VISION_LIGHT_CREATE_INSTANT_CAST(true, m_rightVisionFrontDippedHeadlight, m_rightFrontDippedHeadlight.Diffuse, DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION0, DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION1, DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_LIGHT_RAD);
    
   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_leftVisionFrontBeamHeadlight, m_leftFrontBeamHeadlight.Diffuse, DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION0, DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION1, DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_rightVisionFrontBeamHeadlight, m_rightFrontBeamHeadlight.Diffuse, DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION0, DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION1, DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_leftVisionFrontSignallight, m_leftFrontSignallight.Diffuse, DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION0, DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION1, DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_rightVisionFrontSignallight, m_rightFrontSignallight.Diffuse, DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION0, DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION1, DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(true, m_leftVisionRearMarkerlight, m_leftRearMarkerlight.Diffuse, DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION0, DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION1, DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(true, m_rightVisionRearMarkerlight, m_rightRearMarkerlight.Diffuse, DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION0, DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION1, DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);
    
   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_leftVisionRearBrakelight, m_leftRearBrakelight.Diffuse, DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION0, DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION1, DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_rightVisionRearBrakelight, m_rightRearBrakelight.Diffuse, DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION0, DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION1, DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_leftVisionRearReverselight, m_leftRearReverselight.Diffuse, DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION0, DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION1, DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_rightVisionRearReverselight, m_rightRearReverselight.Diffuse, DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION0, DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION1, DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_leftVisionRearSignallight, m_leftRearSignallight.Diffuse, DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION0, DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION1, DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   VISION_LIGHT_CREATE_INSTANT_CAST(false, m_rightVisionRearSignallight, m_rightRearSignallight.Diffuse, DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION0, DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION1, DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION2, DEFAULT_VISION_COLLIDED_LIGHT_RAD);

   m_neon = std::make_shared<NeonLight>(m_engine, m_d3dd);
}


// Установка параметров света фар машины(+обновление неона)
void CarLight::setPosition()
{
   static D3DXVECTOR3 vecDir = D3DXVECTOR3(0, 0, 0);
   static DWORD signalCount = 0, signalDTime = 25, signalMTime = 50;
    
   m_leftFrontDippedHeadlight.Position = { m_carPosX + m_fDHlPosDistance*cos(m_fDHlPosAngleDeviation - m_carAngleYaw), m_fDHlHeight, m_carPosZ + m_fDHlPosDistance*sin(m_fDHlPosAngleDeviation - m_carAngleYaw) };
   m_rightFrontDippedHeadlight.Position = { m_carPosX + m_fDHlPosDistance*cos(-m_fDHlPosAngleDeviation - m_carAngleYaw), m_fDHlHeight, m_carPosZ + m_fDHlPosDistance*sin(-m_fDHlPosAngleDeviation - m_carAngleYaw) };
   m_leftFrontBeamHeadlight.Position = { m_carPosX + m_fBHlPosDistance*cos(m_fBHlPosAngleDeviation - m_carAngleYaw), m_fBHlHeight, m_carPosZ + m_fBHlPosDistance*sin(m_fBHlPosAngleDeviation - m_carAngleYaw) };
   m_rightFrontBeamHeadlight.Position = { m_carPosX + m_fBHlPosDistance*cos(-m_fBHlPosAngleDeviation - m_carAngleYaw), m_fBHlHeight, m_carPosZ + m_fBHlPosDistance*sin(-m_fBHlPosAngleDeviation - m_carAngleYaw) };
   m_leftFrontSignallight.Position = { m_carPosX + m_fSlPosDistance*cos(m_fSlPosAngleDeviation - m_carAngleYaw), m_fSlHeight, m_carPosZ + m_fSlPosDistance*sin(m_fSlPosAngleDeviation - m_carAngleYaw) };
   m_rightFrontSignallight.Position = { m_carPosX + m_fSlPosDistance*cos(-m_fSlPosAngleDeviation - m_carAngleYaw), m_fSlHeight, m_carPosZ + m_fSlPosDistance*sin(-m_fSlPosAngleDeviation - m_carAngleYaw) };

   m_leftRearMarkerlight.Position = { m_carPosX - m_rMlPosDistance*cos(-m_rMlPosAngleDeviation - m_carAngleYaw), m_rMlHeight, m_carPosZ - m_rMlPosDistance*sin(-m_rMlPosAngleDeviation - m_carAngleYaw) };
   m_rightRearMarkerlight.Position = { m_carPosX - m_rMlPosDistance*cos(m_rMlPosAngleDeviation - m_carAngleYaw), m_rMlHeight, m_carPosZ - m_rMlPosDistance*sin(m_rMlPosAngleDeviation - m_carAngleYaw) };
   m_leftRearBrakelight.Position = { m_carPosX - m_rBlPosDistance*cos(-m_rBlPosAngleDeviation - m_carAngleYaw), m_rBlHeight, m_carPosZ - m_rBlPosDistance*sin(-m_rBlPosAngleDeviation - m_carAngleYaw) };
   m_rightRearBrakelight.Position = { m_carPosX - m_rBlPosDistance*cos(m_rBlPosAngleDeviation - m_carAngleYaw), m_rBlHeight, m_carPosZ - m_rBlPosDistance*sin(m_rBlPosAngleDeviation - m_carAngleYaw) };
   m_leftRearReverselight.Position = { m_carPosX - m_rRlPosDistance*cos(-m_rRlPosAngleDeviation - m_carAngleYaw), m_rRlHeight, m_carPosZ - m_rRlPosDistance*sin(-m_rRlPosAngleDeviation - m_carAngleYaw) };
   m_rightRearReverselight.Position = { m_carPosX - m_rRlPosDistance*cos(m_rRlPosAngleDeviation - m_carAngleYaw), m_rRlHeight, m_carPosZ - m_rRlPosDistance*sin(m_rRlPosAngleDeviation - m_carAngleYaw) };
   m_leftRearSignallight.Position = { m_carPosX - m_rSlPosDistance*cos(-m_rSlPosAngleDeviation - m_carAngleYaw), m_rSlHeight, m_carPosZ - m_rSlPosDistance*sin(-m_rSlPosAngleDeviation - m_carAngleYaw) };
   m_rightRearSignallight.Position = { m_carPosX - m_rSlPosDistance*cos(m_rSlPosAngleDeviation - m_carAngleYaw), m_rSlHeight, m_carPosZ - m_rSlPosDistance*sin(m_rSlPosAngleDeviation - m_carAngleYaw) };

   m_leftVisionFrontDippedHeadlight.Position = m_leftFrontDippedHeadlight.Position;
   m_rightVisionFrontDippedHeadlight.Position = m_rightFrontDippedHeadlight.Position;
   m_leftVisionFrontBeamHeadlight.Position = m_leftFrontBeamHeadlight.Position;
   m_rightVisionFrontBeamHeadlight.Position = m_rightFrontBeamHeadlight.Position;
   m_leftVisionFrontSignallight.Position = m_leftFrontSignallight.Position;
   m_rightVisionFrontSignallight.Position = m_rightFrontSignallight.Position;

   m_leftVisionRearMarkerlight.Position = m_leftRearMarkerlight.Position;    
   m_rightVisionRearMarkerlight.Position = m_rightRearMarkerlight.Position;    
   m_leftVisionRearBrakelight.Position = m_leftRearBrakelight.Position;
   m_rightVisionRearBrakelight.Position = m_rightRearBrakelight.Position;
   m_leftVisionRearReverselight.Position = m_leftRearReverselight.Position;
   m_rightVisionRearReverselight.Position = m_rightRearReverselight.Position;
   m_leftVisionRearSignallight.Position = m_leftRearSignallight.Position;
   m_rightVisionRearSignallight.Position = m_rightRearSignallight.Position;


   vecDir = D3DXVECTOR3(cos(-m_carAngleYaw + m_fDHlLightAngleDeviation), 0.0f, sin(-m_carAngleYaw + m_fDHlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_leftFrontDippedHeadlight.Direction, &vecDir);
   vecDir = D3DXVECTOR3(cos(-m_carAngleYaw - m_fDHlLightAngleDeviation), 0.0f, sin(-m_carAngleYaw - m_fDHlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_rightFrontDippedHeadlight.Direction, &vecDir);

   vecDir = D3DXVECTOR3(cos(-m_carAngleYaw + m_fBHlLightAngleDeviation), 0.0f, sin(-m_carAngleYaw + m_fBHlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_leftFrontBeamHeadlight.Direction, &vecDir);
   vecDir = D3DXVECTOR3(cos(-m_carAngleYaw - m_fBHlLightAngleDeviation), 0.0f, sin(-m_carAngleYaw - m_fBHlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_rightFrontBeamHeadlight.Direction, &vecDir);

   vecDir = D3DXVECTOR3(cos(-m_carAngleYaw + m_fSlLightAngleDeviation), 0.0f, sin(-m_carAngleYaw + m_fSlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_leftFrontSignallight.Direction, &vecDir);
   vecDir = D3DXVECTOR3(cos(-m_carAngleYaw - m_fSlLightAngleDeviation), 0.0f, sin(-m_carAngleYaw - m_fSlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_rightFrontSignallight.Direction, &vecDir);

   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw - m_rMlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw - m_rMlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_leftRearMarkerlight.Direction, &vecDir);
   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw + m_rMlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw + m_rMlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_rightRearMarkerlight.Direction, &vecDir);

   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw - m_rBlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw - m_rBlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_leftRearBrakelight.Direction, &vecDir);
   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw + m_rBlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw + m_rBlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_rightRearBrakelight.Direction, &vecDir);

   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw - m_rRlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw - m_rRlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_leftRearReverselight.Direction, &vecDir);
   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw + m_rRlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw + m_rRlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_rightRearReverselight.Direction, &vecDir);

   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw - m_rSlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw - m_rSlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_leftRearSignallight.Direction, &vecDir);
   vecDir = D3DXVECTOR3(-cos(-m_carAngleYaw + m_rSlLightAngleDeviation), -0.3f, -sin(-m_carAngleYaw + m_rSlLightAngleDeviation));
   D3DXVec3Normalize((D3DXVECTOR3*)&m_rightRearSignallight.Direction, &vecDir);

   if (signalCount == signalMTime)
      signalCount = 0;
    
   m_shaderunit->UpdateSLight(m_lightIndexes[0], m_fDHlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[1], m_fDHlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[2], m_fBHlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[3], m_fBHlEnabled);

   m_shaderunit->UpdateSLight(m_lightIndexes[4], m_lSlEnabled && (signalCount <= signalDTime));
   m_shaderunit->UpdateSLight(m_lightIndexes[5], m_rSlEnabled && (signalCount <= signalDTime));

   m_shaderunit->UpdateSLight(m_lightIndexes[6], m_rMlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[7], m_rMlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[8], m_rBlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[9], m_rBlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[10], m_rRlEnabled);
   m_shaderunit->UpdateSLight(m_lightIndexes[11], m_rRlEnabled);

   m_shaderunit->UpdateSLight(m_lightIndexes[12], m_lSlEnabled && (signalCount <= signalDTime));
   m_shaderunit->UpdateSLight(m_lightIndexes[13], m_rSlEnabled && (signalCount <= signalDTime));
 
   m_shaderunit->UpdateCVLight(m_lightIndexes[14], m_fDHlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[15], m_fDHlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[16], m_fBHlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[17], m_fBHlEnabled);

   m_shaderunit->UpdateCVLight(m_lightIndexes[18], m_lSlEnabled && (signalCount <= signalDTime));
   m_shaderunit->UpdateCVLight(m_lightIndexes[19], m_rSlEnabled && (signalCount <= signalDTime));

   m_shaderunit->UpdateCVLight(m_lightIndexes[20], m_rMlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[21], m_rMlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[22], m_rBlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[23], m_rBlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[24], m_rRlEnabled);
   m_shaderunit->UpdateCVLight(m_lightIndexes[25], m_rRlEnabled);

   m_shaderunit->UpdateCVLight(m_lightIndexes[26], m_lSlEnabled && (signalCount <= signalDTime));
   m_shaderunit->UpdateCVLight(m_lightIndexes[27], m_rSlEnabled && (signalCount <= signalDTime));
    
   if ((!m_lSlEnabled)
      && (!m_rSlEnabled))
      signalCount = 0;
   signalCount++;
}


//---------------------------------------------------------

/*
\fn void initService()
\brief Фукнция инициализации переменных параметров класса машины(пока что по default'у)
*/
void NeonLight::initService()
{
   m_lightDensity = DEFAULT_LIGHT_DENSITY;
   m_fCPCount = 12;
   m_lCPPosDistance = new float[m_fCPCount];
   m_lCPPosAngleDeviation = new float[m_fCPCount];
   m_lCPHeight = new float[m_fCPCount];
    
   m_lCPPosDistance[0] = 2.59f;
   m_lCPPosDistance[1] = 2.59f;
   m_lCPPosDistance[2] = 2.59f;    

   m_lCPPosDistance[3] = 1.5f;
   m_lCPPosDistance[4] = 1.1f;
   m_lCPPosDistance[5] = 1.65f;

   m_lCPPosDistance[6] = 1.65f;
   m_lCPPosDistance[7] = 1.1f;
   m_lCPPosDistance[8] = 1.5f;

   m_lCPPosDistance[9] = 2.99f;
   m_lCPPosDistance[10] = 2.99f;
   m_lCPPosDistance[11] = 2.99f;


   m_lCPPosAngleDeviation[0] = D3DX_PI / 6.9f;
   m_lCPPosAngleDeviation[1] = 0.0f;    
   m_lCPPosAngleDeviation[2] = -D3DX_PI / 6.9f;

   m_lCPPosAngleDeviation[3] = D3DX_PI / 3.5f;
   m_lCPPosAngleDeviation[4] = D3DX_PI / 2.0f;
   m_lCPPosAngleDeviation[5] = D3DX_PI / 1.25f;

   m_lCPPosAngleDeviation[6] = -D3DX_PI / 1.25f;
   m_lCPPosAngleDeviation[7] = -D3DX_PI / 2.0f;
   m_lCPPosAngleDeviation[8] = -D3DX_PI / 3.5f;

   m_lCPPosAngleDeviation[9] = 2.9f*D3DX_PI / 3.3f;
   m_lCPPosAngleDeviation[10] = D3DX_PI;
   m_lCPPosAngleDeviation[11] = -2.9f*D3DX_PI / 3.3f;


   m_lCPHeight[0] = 0.0f;
   m_lCPHeight[1] = 0.0f;
   m_lCPHeight[2] = 0.0f;

   m_lCPHeight[3] = 0.0f;
   m_lCPHeight[4] = 0.0f;
   m_lCPHeight[5] = 0.0f;

   m_lCPHeight[6] = 0.0f;
   m_lCPHeight[7] = 0.0f;
   m_lCPHeight[8] = 0.0f;

   m_lCPHeight[9] = 0.0f;
   m_lCPHeight[10] = 0.0f;
   m_lCPHeight[11] = 0.0f;

   m_lightEnabled = DEFAULT_LIGHT_ENABLE;
   m_currentNeonColor = DEFAULT_NEON_COLOR;

   createLight();
}

// Создание источников неонового света на основе данных координат
void NeonLight::createLight()
{
   DWORD k = 0;
   float /*distx, distz, dist, dtx, dtz,*/ xc, zc;
   //DWORD ltsc;
   m_fHlLSPosX = new float*[m_fCPCount - 1];
   m_fHlLSPosZ = new float*[m_fCPCount - 1];
   m_fHlLSHeight = new float*[m_fCPCount - 1];
   m_fLSCount = new DWORD[m_fCPCount - 1];
   m_frontLight = new D3DLIGHT9*[4];
    
   for (DWORD j = 0; j < 4; j++)
   {
      m_frontLight[j] = new D3DLIGHT9[4];
      m_fHlLSPosX[j] = new float[4];
      m_fHlLSPosZ[j] = new float[4];
      m_fHlLSHeight[j] = new float[4];
      for (DWORD i = 0; i < 3; i++)
      {
         xc = (m_lCPPosDistance[3 * j + i] * cos(m_lCPPosAngleDeviation[3 * j + i]));
         zc = (m_lCPPosDistance[3 * j + i] * sin(m_lCPPosAngleDeviation[3 * j + i]));
         m_fHlLSPosX[j][i] = xc;
         m_fHlLSPosZ[j][i] = zc;
         m_fHlLSHeight[j][i] = m_lCPHeight[3 * j + i];

         NEON_LIGHT_CREATE_INSTANT_CAST((m_frontLight[j][i]), m_currentNeonColor, DEFAULT_NEON_ATTENUATION0, DEFAULT_NEON_ATTENUATION1, DEFAULT_NEON_ATTENUATION2, DEFAULT_NEON_FALLOFF, DEFAULT_NEON_RANGE, DEFAULT_NEON_PHI, DEFAULT_NEON_THETA, m_shaderLightIndex, m_FRONT, k);
      }
      NEON_LIGHT_KP_MOUNT_MULTI_CAST(m_frontLight[j][0], m_frontLight[j][1], m_frontLight[j][2], k);
   }
}

// Установка параметров неоновой подсветки машины
void NeonLight::setPosition()
{
   static D3DXVECTOR3 vecDir;
   DWORD k = 0;
   for (DWORD i = 0; i < 4; i++)
   {
      for (DWORD j = 0; j < 3; j++)
      {
         vecDir = D3DXVECTOR3(0.0f, -0.3f, 0.0f);
         D3DXVec3Normalize((D3DXVECTOR3*)&m_frontLight[i][j].Direction, &vecDir);
         m_frontLight[i][j].Position = { m_carPosX + m_lCPPosDistance[3 * i + j] * cos(m_lCPPosAngleDeviation[3 * i + j] - m_carAngleYaw),
               m_fHlLSHeight[i][j],
               m_carPosZ + m_lCPPosDistance[3 * i + j] * sin(m_lCPPosAngleDeviation[3 * i + j] - m_carAngleYaw) };
      }
   }

   m_shaderunit->UpdateKPLight(0, m_lightEnabled);
   m_shaderunit->UpdateKPLight(1, m_lightEnabled);
   m_shaderunit->UpdateKPLight(2, m_lightEnabled);
   m_shaderunit->UpdateKPLight(3, m_lightEnabled);
}