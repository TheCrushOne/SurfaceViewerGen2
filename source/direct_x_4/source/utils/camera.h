#pragma once

/**
@file     camera.h
@brief    Camera support class
@date     Created on 22/10/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class CarMove;
class Engine;

typedef short VCMTYPE;
typedef bool VFLAG;

#define VBC_CM_ORBITAL 0
#define VBC_CM_SPECTATOR 1
#define VBC_CM_FOLLOW 2     //далее будет возможность управлять машиной

#define VBC_NOCOLLISION false
#define VBC_COLLISION true

#define VBC_TRUE true
#define VBC_FALSE false

// Основной класс для работы с камерой
class Camera
{
public:
   Camera(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev);
   Camera(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev, float eX, float eY, float eZ, float laX, float laY, float laZ);

   void LRelease() {}
   void LRecreate(int, int);

   void Update();
   void Render();

   void CtrlMouseMove(float dx, float dy);
   void CtrlForwardSide(float dfwd, float dsd);
   void CtrlZoom(float dz);

   void ToggleCameraMoveType();
   void ToggleCameraCollisionMod();
   void ToggleRenderStateFillMode();
   void ChangeMovementSpeed(float);

   inline void SetUCSCamera(bool enabled)
   {
      static VCMTYPE prevStat = VBC_CM_ORBITAL;
      (m_masterBlock != enabled) ? (enabled) ? prevStat = m_cameraMoveType, m_cameraMoveType = VBC_CM_FOLLOW : m_cameraMoveType = prevStat : 1;
      m_masterBlock = enabled;
   }
   inline float GetCurrentHeight() const { return m_eyeY; }
   inline D3DXVECTOR3 GetCurrentPosition() const { return D3DXVECTOR3(m_eyeX, m_eyeY, m_eyeZ); }
   inline D3DXVECTOR3 GetCurrentTarget() const { return D3DXVECTOR3(m_lookatX, m_lookatY, m_lookatZ); }
   inline VCMTYPE GetCameraMoveType() const { return m_cameraMoveType; }
   inline VFLAG GetCameraCollision() const { return m_blockCameraCollision; }
   inline VFLAG GetCurrentCollision() const { return m_eCCollision; }
   inline float GetMovementSpeed() const { return m_spectatorMovementSpeed; }
   inline DWORD GetRenderStateFillMode() const { return m_rSFMode; }
   inline DWORD GetRenderStateCullMode() const { return m_rSCMode; }
   inline D3DXMATRIXA16 GetMProjection() const { return m_mProj; }
   inline D3DXMATRIXA16 GetMView() const { return m_mView; }
private:
   void initService();

   void directRotate(float dx, float dy);
   void reverseRotate(float dx, float dy);
   void fwRotate(float dx, float dy);		//как директ, только вокруг подвижной точки
   void zoom(float dr);
   void fwZoom(float dr);
   void strafeMove(float dw, float dv);
   void up(float dr);

   void fwRecount();
   void speedRecount(float &, float &, float &);

   void avoidCameraCollision();

   void updateMatrixes();
   //geometry:
private:
   float m_eyeX, m_eyeY, m_eyeZ;
   float m_lookatX, m_lookatY, m_lookatZ;

   float m_cameraMaxZoom, m_cameraMinZoom;

   float m_spectatorMovementSpeed;

   bool m_masterBlock;
   //follow:
   float m_fwEyeX, m_fwEyeY, m_fwEyeZ;
   float m_fwLookatX, m_fwLookatY, m_fwLookatZ;
   float m_carAngleYaw;
   std::shared_ptr<CarMove> m_carMove;

   LPDIRECT3DDEVICE9 m_d3dd;

   VCMTYPE m_cameraMoveType;

   VFLAG m_blockCameraCollision;
   VFLAG m_eCCollision;
   VFLAG m_bCCToggled;

   DWORD m_rSFMode;      //вместе с ним переключается cull
   DWORD m_rSCMode;

   D3DXMATRIXA16 m_mView;
   D3DXMATRIXA16 m_mProj;
public:
   static const float DEFAULT_MAX_ZOOM;
   static const float DEFAULT_MIN_ZOOM;

   static const VCMTYPE DEFAULT_CAMERA_MOVE_TYPE;
   static const VFLAG DEFAULT_CAMERA_COLLISION_MOD;

   static const float DEFAULT_SPECTATOR_MOVEMENT_SPEED;
   static const float LIMIT_DEVIATION;

   static const float COLLISION_MIN_BORDER;
   static const float EARTH_LEVEL;

   static const float SPEED_CHANGE_STEP;
};