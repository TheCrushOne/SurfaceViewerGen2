#pragma once

/**
@file     carlight.h
@brief    Car light support class
@date     Created on 23/10/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class ShaderUnit;
class Engine;
class Light;

typedef bool CFLAG;



/*
\class NeonLight
\brief - Специальный класс для работы неоновым освещением машины
*/
class NeonLight
{
public:
   NeonLight(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev);
   ~NeonLight();

   void Update(D3DXVECTOR3, float);
   void ToggleState();
private:
   void setPosition();
   void initService();
   //void mountDefaultNeonCoordinates();
   void createLight();
private:
   float m_carPosX, m_carPosY, m_carPosZ, m_carAngleYaw;
   float* m_lCPPosDistance, * m_lCPPosAngleDeviation, * m_lCPHeight;

   DWORD m_lSCPCount, m_rSCPCount;
   DWORD m_fCPCount, m_rCPCount;

   DWORD m_lightDensity;     //некий коэффициент

   std::shared_ptr<ShaderUnit> m_shaderunit;
   std::shared_ptr<Engine> m_engine;
   LPDIRECT3DDEVICE9 m_d3dd;

   //DWORD m_startLightIndex;      //не требуется
   D3DLIGHT9** m_leftSideLight, ** m_rightSideLight;
   D3DLIGHT9** m_frontLight, ** m_rearLight;
   DWORD** m_shaderLightIndex;

   DWORD* m_lSLSCount, * m_rSLSCount;
   DWORD* m_fLSCount, * m_rLSCount;

   float** m_fHlLSPosX, ** m_fHlLSPosZ, ** m_fHlLSHeight;
   float** m_rMlLSPosX, ** m_rMlLSPosZ, ** m_rMlLSHeight;
   float** m_sFlLSPosX, ** m_sFlLSPosZ, ** m_sFlLSHeight;
   float** m_sRlLSPosX, ** m_sRlLSPosZ, ** m_sRlLSHeight;

   bool m_lightEnabled;

   D3DCOLORVALUE m_currentNeonColor;
public:
   static const DWORD DEFAULT_LIGHT_DENSITY;
   static const D3DCOLORVALUE DEFAULT_NEON_COLOR;
   static const float DEFAULT_NEON_ATTENUATION0;
   static const float DEFAULT_NEON_ATTENUATION1;
   static const float DEFAULT_NEON_ATTENUATION2;
   static const float DEFAULT_NEON_FALLOFF;
   static const float DEFAULT_NEON_RANGE;
   static const float DEFAULT_NEON_PHI;
   static const float DEFAULT_NEON_THETA;
   static const DWORD MAX_LINE_LIGHT;

   static const bool DEFAULT_LIGHT_ENABLE;
};

// Основной класс для работы с освещением машины
class CarLight
{
public:
   CarLight(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev, DWORD stIndex);
   ~CarLight();

   void LRelease() {}
   void LRecreate() {}

   void Update(D3DXVECTOR3, float);
   void ToggleState(CLTYPE type);
   void ToggleState(CLTYPE type, bool status);
   void NeonRedirect();
   //void setBrakeLight(ATYPE status);     //убрано
private:
   void initService();
   void setPosition();
private:
   float m_carPosX, m_carPosY, m_carPosZ, m_carAngleYaw;

   //расстояние, угол и высота относительно центра машины
   float m_fDHlPosDistance, m_fDHlPosAngleDeviation, m_fDHlHeight;
   float m_fBHlPosDistance, m_fBHlPosAngleDeviation, m_fBHlHeight;
   float m_fSlPosDistance, m_fSlPosAngleDeviation, m_fSlHeight;
   float m_rMlPosDistance, m_rMlPosAngleDeviation, m_rMlHeight;
   float m_rBlPosDistance, m_rBlPosAngleDeviation, m_rBlHeight;
   float m_rRlPosDistance, m_rRlPosAngleDeviation, m_rRlHeight;
   float m_rSlPosDistance, m_rSlPosAngleDeviation, m_rSlHeight;

   //углы направления
   float m_fDHlLightAngleDeviation, m_fBHlLightAngleDeviation, m_fSlLightAngleDeviation;//64
   float m_rMlLightAngleDeviation, m_rBlLightAngleDeviation, m_rRlLightAngleDeviation, m_rSlLightAngleDeviation;//мт 1

   std::shared_ptr<NeonLight> m_neon;
   std::shared_ptr<Light> m_lightDispatcher;
   std::shared_ptr<ShaderUnit> m_shaderunit;
   std::shared_ptr<Engine> m_engine;

   LPDIRECT3DDEVICE9 m_d3dd;

   DWORD m_startLightIndex;
   DWORD m_carLightNum;

   CFLAG m_fDHlEnabled;
   CFLAG m_fBHlEnabled;
   CFLAG m_rMlEnabled;
   CFLAG m_rBlEnabled;
   CFLAG m_rRlEnabled;
   CFLAG m_lSlEnabled;   //левые поворотники
   CFLAG m_rSlEnabled;   //правые поворотники

   //без шейдеров только ближний свет и габариты
   //ближний свет
   D3DLIGHT9 m_leftFrontDippedHeadlight, m_rightFrontDippedHeadlight;
   D3DLIGHT9 m_leftVisionFrontDippedHeadlight, m_rightVisionFrontDippedHeadlight;

   //дальний свет
   D3DLIGHT9 m_leftFrontBeamHeadlight, m_rightFrontBeamHeadlight;
   D3DLIGHT9 m_leftVisionFrontBeamHeadlight, m_rightVisionFrontBeamHeadlight;

   //передние поворотники
   D3DLIGHT9 m_leftFrontSignallight, m_rightFrontSignallight;
   D3DLIGHT9 m_leftVisionFrontSignallight, m_rightVisionFrontSignallight;

   //габариты
   D3DLIGHT9 m_leftRearMarkerlight, m_rightRearMarkerlight;
   D3DLIGHT9 m_leftVisionRearMarkerlight, m_rightVisionRearMarkerlight;

   //тормозные огни
   D3DLIGHT9 m_leftRearBrakelight, m_rightRearBrakelight;
   D3DLIGHT9 m_leftVisionRearBrakelight, m_rightVisionRearBrakelight;

   //фонари заднего хода
   D3DLIGHT9 m_leftRearReverselight, m_rightRearReverselight;
   D3DLIGHT9 m_leftVisionRearReverselight, m_rightVisionRearReverselight;

   //задние поворотники
   D3DLIGHT9 m_leftRearSignallight, m_rightRearSignallight;
   D3DLIGHT9 m_leftVisionRearSignallight, m_rightVisionRearSignallight;

   D3DCOLORVALUE m_frontDippedHeadlightEffect;
   D3DCOLORVALUE m_frontBeamHeadlightEffect;
   D3DCOLORVALUE m_frontSignallightEffect;
   D3DCOLORVALUE m_rearMarkerLightEffect;
   D3DCOLORVALUE m_rearBrakelightEffect;
   D3DCOLORVALUE m_rearReverselightEffect;
   D3DCOLORVALUE m_rearSignallightEffect;

   DWORD* m_lightIndexes;
public:
   static const float DEFAULT_FDHLPOSDISTANCE;
   static const float DEFAULT_FBHLPOSDISTANCE;
   static const float DEFAULT_FSLPOSDISTANCE;
   static const float DEFAULT_RMLPOSDISTANCE;
   static const float DEFAULT_RBLPOSDISTANCE;
   static const float DEFAULT_RRLPOSDISTANCE;
   static const float DEFAULT_RSLPOSDISTANCE;

   static const float DEFAULT_FDHLPOSANGLEDEVIATION;
   static const float DEFAULT_FBHLPOSANGLEDEVIATION;
   static const float DEFAULT_FSLPOSANGLEDEVIATION;
   static const float DEFAULT_RMLPOSANGLEDEVIATION;
   static const float DEFAULT_RBLPOSANGLEDEVIATION;
   static const float DEFAULT_RRLPOSANGLEDEVIATION;
   static const float DEFAULT_RSLPOSANGLEDEVIATION;

   static const float DEFAULT_FDHLHEIGHT;
   static const float DEFAULT_FBHLHEIGHT;
   static const float DEFAULT_FSLHEIGHT;
   static const float DEFAULT_RMLHEIGHT;
   static const float DEFAULT_RBLHEIGHT;
   static const float DEFAULT_RRLHEIGHT;
   static const float DEFAULT_RSLHEIGHT;

   static const float DEFAULT_FDHLLIGHTANGLEDEVIATION;
   static const float DEFAULT_FBHLLIGHTANGLEDEVIATION;
   static const float DEFAULT_FSLLIGHTANGLEDEVIATION;
   static const float DEFAULT_RMLLIGHTANGLEDEVIATION;
   static const float DEFAULT_RBLLIGHTANGLEDEVIATION;
   static const float DEFAULT_RRLLIGHTANGLEDEVIATION;
   static const float DEFAULT_RSLLIGHTANGLEDEVIATION;

   static const float DEFAULT_VISION_LIGHT_RAD;
   static const float DEFAULT_VISION_COLLIDED_LIGHT_RAD;
   static const DWORD DEFAULT_CAR_LIGHT_NUM;

   static const D3DCOLORVALUE DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT;
   static const D3DCOLORVALUE DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT;
   static const D3DCOLORVALUE DEFAULT_FRONT_SIGNALLIGHT_EFFECT;
   static const D3DCOLORVALUE DEFAULT_REAR_MARKERLIGHT_EFFECT;
   static const D3DCOLORVALUE DEFAULT_REAR_BRAKELIGHT_EFFECT;
   static const D3DCOLORVALUE DEFAULT_REAR_REVERSELIGHT_EFFECT;
   static const D3DCOLORVALUE DEFAULT_REAR_SIGNALLIGHT_EFFECT;

   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION0;
   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION1;
   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_ATTENUATION2;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION0;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION1;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_ATTENUATION2;
   static const float DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION0;
   static const float DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION1;
   static const float DEFAULT_FRONT_SIGNALLIGHT_EFFECT_ATTENUATION2;
   static const float DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION0;
   static const float DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION1;
   static const float DEFAULT_REAR_MARKERLIGHT_EFFECT_ATTENUATION2;
   static const float DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION0;
   static const float DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION1;
   static const float DEFAULT_REAR_BRAKELIGHT_EFFECT_ATTENUATION2;
   static const float DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION0;
   static const float DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION1;
   static const float DEFAULT_REAR_REVERSELIGHT_EFFECT_ATTENUATION2;
   static const float DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION0;
   static const float DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION1;
   static const float DEFAULT_REAR_SIGNALLIGHT_EFFECT_ATTENUATION2;

   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_FALLOFF;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_FALLOFF;
   static const float DEFAULT_FRONT_SIGNALLIGHT_EFFECT_FALLOFF;
   static const float DEFAULT_REAR_MARKERLIGHT_EFFECT_FALLOFF;
   static const float DEFAULT_REAR_BRAKELIGHT_EFFECT_FALLOFF;
   static const float DEFAULT_REAR_REVERSELIGHT_EFFECT_FALLOFF;
   static const float DEFAULT_REAR_SIGNALLIGHT_EFFECT_FALLOFF;

   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_RANGE;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_RANGE;
   static const float DEFAULT_FRONT_SIGNALLIGHT_EFFECT_RANGE;
   static const float DEFAULT_REAR_MARKERLIGHT_EFFECT_RANGE;
   static const float DEFAULT_REAR_BRAKELIGHT_EFFECT_RANGE;
   static const float DEFAULT_REAR_REVERSELIGHT_EFFECT_RANGE;
   static const float DEFAULT_REAR_SIGNALLIGHT_EFFECT_RANGE;

   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_PHI;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_PHI;
   static const float DEFAULT_FRONT_SIGNALLIGHT_EFFECT_PHI;
   static const float DEFAULT_REAR_MARKERLIGHT_EFFECT_PHI;
   static const float DEFAULT_REAR_BRAKELIGHT_EFFECT_PHI;
   static const float DEFAULT_REAR_REVERSELIGHT_EFFECT_PHI;
   static const float DEFAULT_REAR_SIGNALLIGHT_EFFECT_PHI;

   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_EFFECT_THETA;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_EFFECT_THETA;
   static const float DEFAULT_FRONT_SIGNALLIGHT_EFFECT_THETA;
   static const float DEFAULT_REAR_MARKERLIGHT_EFFECT_THETA;
   static const float DEFAULT_REAR_BRAKELIGHT_EFFECT_THETA;
   static const float DEFAULT_REAR_REVERSELIGHT_EFFECT_THETA;
   static const float DEFAULT_REAR_SIGNALLIGHT_EFFECT_THETA;

   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION0;
   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION1;
   static const float DEFAULT_FRONT_DIPPED_HEADLIGHT_VISION_ATTENUATION2;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION0;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION1;
   static const float DEFAULT_FRONT_BEAM_HEADLIGHT_VISION_ATTENUATION2;
   static const float DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION0;
   static const float DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION1;
   static const float DEFAULT_FRONT_SIGNALLIGHT_VISION_ATTENUATION2;
   static const float DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION0;
   static const float DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION1;
   static const float DEFAULT_REAR_MARKERLIGHT_VISION_ATTENUATION2;
   static const float DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION0;
   static const float DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION1;
   static const float DEFAULT_REAR_BRAKELIGHT_VISION_ATTENUATION2;
   static const float DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION0;
   static const float DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION1;
   static const float DEFAULT_REAR_REVERSELIGHT_VISION_ATTENUATION2;
   static const float DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION0;
   static const float DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION1;
   static const float DEFAULT_REAR_SIGNALLIGHT_VISION_ATTENUATION2;
};