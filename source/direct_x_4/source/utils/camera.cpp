/**
@file     camera.cpp
@brief    Camera processing class
@date     Created on 22/10/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "camera.h"
#include "engine/engine.h"
#include "car_model\carmove.h"

const float Camera::DEFAULT_MAX_ZOOM = 1000.0f;
const float Camera::DEFAULT_MIN_ZOOM = 1.0f;
const float Camera::COLLISION_MIN_BORDER = 5.0f;
const float Camera::EARTH_LEVEL = 0.0f;
const float Camera::LIMIT_DEVIATION = 0.98f;  //около пи/2
const VCMTYPE Camera::DEFAULT_CAMERA_MOVE_TYPE = VBC_CM_ORBITAL;
const VFLAG Camera::DEFAULT_CAMERA_COLLISION_MOD = VBC_NOCOLLISION;
const float Camera::DEFAULT_SPECTATOR_MOVEMENT_SPEED = 36.0f;
const float Camera::SPEED_CHANGE_STEP = 1.0f;

//==============================//Конструктор(вынесен из .h после введения диспетчера)
Camera::Camera(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev)
   : m_d3dd(dev)
   , m_eyeX(17.6f)
   , m_eyeY(32.4f)
   , m_eyeZ(-34.6f)
   , m_lookatX(0.0f)
   , m_lookatY(0.0f)
   , m_lookatZ(0.0f)
   , m_carMove(eng->GetCarmove())
{
   initService();
}

Camera::Camera(std::shared_ptr<Engine> eng, LPDIRECT3DDEVICE9 dev, float eX, float eY, float eZ, float laX, float laY, float laZ)
   : m_d3dd(dev)
   , m_eyeX(eX)
   , m_eyeY(eY)
   , m_eyeZ(eZ)
   , m_lookatX(laX)
   , m_lookatY(laY)
   , m_lookatZ(laZ)
   , m_carMove(eng->GetCarmove())
{
   initService();
}

// Автовосстановления объектов директикса(после ресетом устройства)
void Camera::LRecreate(int nW, int nH)
{
   //матрица проекции
   //D3DXMATRIX proj;
   D3DXMatrixPerspectiveFovLH(
      &m_mProj,
      D3DX_PI * 0.25f, // 90 градусов
      (float)nW / (float)nH,
      0.1f,               //странно...при 0 некорректно работает Z-буфер....
      6000.0f
   );
}

// Обновление данных(на каждом кадре)
void Camera::Update()
{
   if (m_cameraMoveType == VBC_CM_FOLLOW)        //фикс бага с недоворотом камеры
   {
      fwRecount();
      updateMatrixes();
   }
}

// Отрисовка(которой нечего тут делать...)
void Camera::Render()
{
   //updateMatrixes();
}

// Внешняя функция управления камерой №1
void Camera::CtrlForwardSide(float dsd, float dfwd)
{
   if (m_cameraMoveType == VBC_CM_ORBITAL)
      directRotate(dsd, dfwd);
   else if (m_cameraMoveType == VBC_CM_SPECTATOR)
      strafeMove(dsd, dfwd);
   else if (m_cameraMoveType == VBC_CM_FOLLOW)
      fwRotate(dsd, dfwd);
   else
      ;       //возможны еще варианты
}

// Внешняя функция управления камерой №1
void Camera::CtrlMouseMove(float dx, float dy)
{
   if (m_cameraMoveType == VBC_CM_ORBITAL)
      CtrlForwardSide(dx, dy);
   else if (m_cameraMoveType == VBC_CM_SPECTATOR)
      reverseRotate(dx, dy);
   else
      ;       //возможны еще варианты(камера от 3-го лица)
}

// Внешняя функция управления камерой №2
void Camera::CtrlZoom(float dz)
{
   if (m_cameraMoveType == VBC_CM_ORBITAL)
      zoom(dz);
   else if (m_cameraMoveType == VBC_CM_SPECTATOR)
      ;       //пока что бинда нет, потом возможно изменение fovа
   else if (m_cameraMoveType == VBC_CM_FOLLOW)
      fwZoom(dz);
   else
      ;       //-||-
}

// Переключение автомата типа движения камеры
void Camera::ToggleCameraMoveType()
{
   if (m_masterBlock)
      return;
   if (m_cameraMoveType == VBC_CM_ORBITAL)
      m_cameraMoveType = VBC_CM_SPECTATOR;
   else if (m_cameraMoveType == VBC_CM_SPECTATOR)
      m_cameraMoveType = VBC_CM_FOLLOW;
   else if (m_cameraMoveType == VBC_CM_FOLLOW)
      m_cameraMoveType = VBC_CM_ORBITAL;
   else
      ;
}

// Переключение автомата типа взаимодействия с околонулевой координатой по Y
void Camera::ToggleCameraCollisionMod()
{
   m_blockCameraCollision = !m_blockCameraCollision;
   m_bCCToggled = VBC_TRUE;
   if (!m_blockCameraCollision)
      m_eCCollision = VBC_NOCOLLISION;
}

// Переключение автомата типа отображения текстур
void Camera::ToggleRenderStateFillMode()
{
   if (m_rSFMode == D3DFILL_WIREFRAME)
   {
      m_rSFMode = D3DFILL_FORCE_DWORD;
      m_rSCMode = D3DCULL_CCW;
   }
   else if (m_rSFMode == D3DFILL_FORCE_DWORD)
   {
      m_rSFMode = D3DFILL_WIREFRAME;
      m_rSCMode = D3DCULL_NONE;
   }
   m_d3dd->SetRenderState(D3DRS_FILLMODE, m_rSFMode);
   m_d3dd->SetRenderState(D3DRS_CULLMODE, m_rSCMode);
}

// Изменение скорости перемещения(пока что только spectator)
void Camera::ChangeMovementSpeed(float dspd)
{
   m_spectatorMovementSpeed += dspd;
}

// Инициализации переменных камеры(пока что только default)
void Camera::initService()
{
   //float yaw = vbc_carMove->getAngleYaw();
   m_fwEyeX = -6.0f;// *sin(yaw);
   m_fwEyeY = 2.0f;
   m_fwEyeZ = 0.0f;// 6.0f*cos(yaw);
   m_fwLookatX = 0.0f;
   m_fwLookatY = 2.0f;
   m_fwLookatZ = 0.0f;
   m_cameraMaxZoom = DEFAULT_MAX_ZOOM;
   m_cameraMinZoom = DEFAULT_MIN_ZOOM;
   m_cameraMoveType = DEFAULT_CAMERA_MOVE_TYPE;
   m_blockCameraCollision = DEFAULT_CAMERA_COLLISION_MOD;
   m_spectatorMovementSpeed = DEFAULT_SPECTATOR_MOVEMENT_SPEED;
   m_eCCollision = VBC_NOCOLLISION;

#ifdef _DEBUG
   m_d3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);    //дебаг
   m_d3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);         //сквозной просвет
   m_rSFMode = D3DFILL_WIREFRAME;
   m_rSCMode = D3DCULL_NONE;
#else
   m_d3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_FORCE_DWORD);
   m_d3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
   m_rSFMode = D3DFILL_FORCE_DWORD;
   m_rSCMode = D3DCULL_CCW;
   //sta_d3dd->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
#endif
   D3DDEVICE_CREATION_PARAMETERS cparams;
   RECT rect;

   m_d3dd->GetCreationParameters(&cparams);
   GetWindowRect(cparams.hFocusWindow, &rect);
   LRecreate((rect.left - rect.right), (rect.top - rect.bottom));

   updateMatrixes();
}

// Обработка вращения камеры вокруг точки(точка взгляда)(исп. сферическая с.к.)
void Camera::directRotate(float dx, float dy)
{
   if (dx != 0)            //вокруг Y(движение по орбите на постоянной высоте)
   {
      float tmp_x;
      tmp_x = m_lookatX + (m_eyeX - m_lookatX)*cos(dx) - (m_eyeZ - m_lookatZ)*sin(dx);
      m_eyeZ = m_lookatZ + (m_eyeX - m_lookatX)*sin(dx) + (m_eyeZ - m_lookatZ)*cos(dx);
      m_eyeX = tmp_x;
   }

   if (dy != 0)            //по Y, т.е. вокруг произвольной оси из плоскости XZ(незначительный поворот(скорее всего погрешность вычислений) при отведении до упора вверх и вниз)
   {
      SHAMANISTIC_INSTANT_CAST(dy);

      if ((dy <= 0)
      && (m_eCCollision))
         return;

      float dist = sqrt(sqrt((m_eyeX - m_lookatX)*(m_eyeX - m_lookatX) + (m_eyeY - m_lookatY)*(m_eyeY - m_lookatY))*sqrt((m_eyeX - m_lookatX)*(m_eyeX - m_lookatX) + (m_eyeY - m_lookatY)*(m_eyeY - m_lookatY)) + (m_eyeZ - m_lookatZ)*(m_eyeZ - m_lookatZ));
      float height = (m_eyeY - m_lookatY);
      if (((height / dist >= LIMIT_DEVIATION)         //защита от беспорядочных перемещений в зените
         && (dy > 0))
         || ((height / dist <= -LIMIT_DEVIATION)
         && (dy < 0)))
         return;

      SHAMANISTIC_INSTANT_CAST(dy);
      float theta = acos((m_eyeY - m_lookatY) / dist);
      float cosfi = (m_eyeX - m_lookatX) / (dist*sin(theta));
      float sinfi = (m_eyeZ - m_lookatZ) / (dist*sin(theta));

      theta += dy;

      m_eyeY = dist*cos(theta) + m_lookatY;
      avoidCameraCollision();
      if (m_eCCollision)
      {
         updateMatrixes();
         return;
      }
      m_eyeX = dist*sin(theta)*cosfi + m_lookatX;
      m_eyeZ = dist*sin(theta)*sinfi + m_lookatZ;
   }
   updateMatrixes();
}

// Обработка вращения камеры вокруг точки(точка местонахождения камеры)(исп. сферическая с.к.)
void Camera::reverseRotate(float dx, float dy)
{
   if (dx != 0)            //вокруг Y (вверх от направления камеры)
   {
      float tmp_x;
      tmp_x = m_eyeX + (m_lookatX - m_eyeX)*cos(dx) - (m_lookatZ - m_eyeZ)*sin(dx);
      m_lookatZ = m_eyeZ + (m_lookatX - m_eyeX)*sin(dx) + (m_lookatZ - m_eyeZ)*cos(dx);
      m_lookatX = tmp_x;
   }

   if (dy != 0)            //по Y, т.е. вокруг произвольной оси из плоскости XZ(незначительный поворот при отведении до упора вверх и вниз)
   {
      float dist = sqrt(sqrt((m_eyeX - m_lookatX)*(m_eyeX - m_lookatX) + (m_eyeY - m_lookatY)*(m_eyeY - m_lookatY))*sqrt((m_eyeX - m_lookatX)*(m_eyeX - m_lookatX) + (m_eyeY - m_lookatY)*(m_eyeY - m_lookatY)) + (m_eyeZ - m_lookatZ)*(m_eyeZ - m_lookatZ));
      float height = (m_eyeY - m_lookatY);
      if (((height / dist <= -LIMIT_DEVIATION)         //защита от беспорядочных перемещений в зените
         && (dy > 0))
         || ((height / dist >= LIMIT_DEVIATION)
         && (dy < 0)))
         return;
      //float tmp_x, tmp_y, tmp_z;
      float theta = acos((m_lookatY - m_eyeY) / dist);
      float cosfi = (m_lookatX - m_eyeX) / (dist*sin(theta));
      float sinfi = (m_lookatZ - m_eyeZ) / (dist*sin(theta));
      SHAMANISTIC_INSTANT_CAST(dy);
      theta += dy;
      m_lookatX = dist*sin(theta)*cosfi + m_eyeX;
      m_lookatZ = dist*sin(theta)*sinfi + m_eyeZ;
      m_lookatY = dist*cos(theta) + m_eyeY;
   }
   updateMatrixes();
}

// Обработка вращения камеры вокруг точки(центра машины)(исп. сферическая с.к.)
void Camera::fwRotate(float dx, float dy)
{
   if (dx != 0)            //вокруг Y(движение по орбите на постоянной высоте)
   {
      float tmp_x;
      tmp_x = m_fwLookatX + (m_fwEyeX - m_fwLookatX)*cos(dx) - (m_fwEyeZ - m_fwLookatZ)*sin(dx);
      m_fwEyeZ = m_fwLookatZ + (m_fwEyeX - m_fwLookatX)*sin(dx) + (m_fwEyeZ - m_fwLookatZ)*cos(dx);
      m_fwEyeX = tmp_x;
   }

   if (dy != 0)            //по Y, т.е. вокруг произвольной оси из плоскости XZ(незначительный поворот(скорее всего погрешность вычислений) при отведении до упора вверх и вниз)
   {
      SHAMANISTIC_INSTANT_CAST(dy);

      if ((dy <= 0)
         && (m_eCCollision))
         return;


      float dist = sqrt(sqrt((m_fwEyeX - m_fwLookatX)*(m_fwEyeX - m_fwLookatX) + (m_fwEyeY - m_fwLookatY)*(m_fwEyeY - m_fwLookatY))*sqrt((m_fwEyeX - m_fwLookatX)*(m_fwEyeX - m_fwLookatX) + (m_fwEyeY - m_fwLookatY)*(m_fwEyeY - m_fwLookatY)) + (m_fwEyeZ - m_fwLookatZ)*(m_fwEyeZ - m_fwLookatZ));
      float height = (m_fwEyeY - m_fwLookatY);
      if (((height / dist >= LIMIT_DEVIATION)         //защита от беспорядочных перемещений в зените
         && (dy > 0))
         || ((height / dist <= -LIMIT_DEVIATION)
         && (dy < 0)))
         return;

      SHAMANISTIC_INSTANT_CAST(dy);
      float theta = acos((m_fwEyeY - m_fwLookatY) / dist);
      float cosfi = (m_fwEyeX - m_fwLookatX) / (dist*sin(theta));
      float sinfi = (m_fwEyeZ - m_fwLookatZ) / (dist*sin(theta));

      theta += dy;

      m_fwEyeY = dist*cos(theta) + m_fwLookatY;
      avoidCameraCollision();
      if (m_eCCollision)
      {
         fwRecount();
         updateMatrixes();
         return;
      }
      m_fwEyeX = dist*sin(theta)*cosfi + m_fwLookatX;
      m_fwEyeZ = dist*sin(theta)*sinfi + m_fwLookatZ;
   }
   fwRecount();
   updateMatrixes();
}

// Перенаправление на обработку перемещения камеры №3
void Camera::strafeMove(float dw, float dm)
{
   if (dw != 0.0f)
   {
      float tmp_x, tmp_y = 0.0f, tmp_z;
      tmp_x = (m_eyeX - m_lookatX);
      tmp_z = (m_eyeZ - m_lookatZ);
      speedRecount(tmp_x, tmp_y, tmp_z);
      m_eyeX -= tmp_z*dw;
      m_eyeZ += tmp_x*dw;
      m_lookatX -= tmp_z*dw;
      m_lookatZ += tmp_x*dw;
   }

   if (dm != 0.0f)
   {
      float tmp_x, tmp_y, tmp_z;
      tmp_x = (m_eyeX - m_lookatX);
      tmp_y = (m_eyeY - m_lookatY);
      tmp_z = (m_eyeZ - m_lookatZ);
      speedRecount(tmp_x, tmp_y, tmp_z);
      m_eyeX += tmp_x*dm;        
      m_eyeZ += tmp_z*dm;
      m_lookatX += tmp_x*dm;        
      m_lookatZ += tmp_z*dm;
        
      if ((m_eCCollision)
         && (dm < 0))
      {
         updateMatrixes();
         return;
      }
      m_eyeY += tmp_y*dm;
      m_lookatY += tmp_y*dm;

      avoidCameraCollision();
   }
   updateMatrixes();
}

// Перенаправление на обработку перемещения камеры №2
void Camera::zoom(float dr)
{
   if (dr != 0)    //пошаманить со знаком
   {
      float distance = sqrt(sqrt((m_eyeX - m_lookatX)*(m_eyeX - m_lookatX) + (m_eyeY - m_lookatY)*(m_eyeY - m_lookatY))*sqrt((m_eyeX - m_lookatX)*(m_eyeX - m_lookatX) + (m_eyeY - m_lookatY)*(m_eyeY - m_lookatY)) + (m_eyeZ - m_lookatZ)*(m_eyeZ - m_lookatZ));

      if (((dr >= 0) && (distance <= m_cameraMinZoom))
         || ((dr <= 0) && (distance >= m_cameraMaxZoom)))
         return;
      m_eyeX -= (m_eyeX - m_lookatX)*dr;
      m_eyeY -= (m_eyeY - m_lookatY)*dr;
      m_eyeZ -= (m_eyeZ - m_lookatZ)*dr;
   }

   updateMatrixes();
}

// Перенаправление на обработку перемещения камеры №2
void Camera::fwZoom(float dr)
{
   if (dr != 0)    //пошаманить со знаком
   {
      float distance = sqrt(sqrt((m_fwEyeX - m_fwLookatX)*(m_fwEyeX - m_fwLookatX) + (m_fwEyeY - m_fwLookatY)*(m_fwEyeY - m_fwLookatY))*sqrt((m_fwEyeX - m_fwLookatX)*(m_fwEyeX - m_fwLookatX) + (m_fwEyeY - m_fwLookatY)*(m_fwEyeY - m_fwLookatY)) + (m_fwEyeZ - m_fwLookatZ)*(m_fwEyeZ - m_fwLookatZ));

      if (((dr >= 0) && (distance <= m_cameraMinZoom))
         || ((dr <= 0) && (distance >= m_cameraMaxZoom)))
         return;
      m_fwEyeX -= (m_fwEyeX - m_fwLookatX)*dr;
      m_fwEyeY -= (m_fwEyeY - m_fwLookatY)*dr;
      m_fwEyeZ -= (m_fwEyeZ - m_fwLookatZ)*dr;
   }
   fwRecount();
   updateMatrixes();
}

// Пересчета координат камеры из с.к. машины в глобальную
void Camera::fwRecount()
{
   static float yaw = 0.0f;
   static D3DXVECTOR3 pos = {0, 0, 0};
   yaw = m_carMove->GetAngleYaw();
   pos = m_carMove->GetCoordinatesWOMove();
   m_eyeX = pos.x + m_fwEyeX*sin(yaw) - m_fwEyeZ*cos(yaw);
   m_eyeZ = pos.z + m_fwEyeX*cos(yaw) + m_fwEyeZ*sin(yaw);
   m_eyeY = m_fwEyeY;
   m_lookatX = pos.x + m_fwLookatX*sin(yaw) - m_fwLookatZ*cos(yaw);
   m_lookatZ = pos.z + m_fwLookatX*cos(yaw) + m_fwLookatZ*sin(yaw);
   m_lookatY = m_fwLookatY;
}

// Пересчет изменения координат в соответствии со скоростью перемещения
void Camera::speedRecount(float &x, float &y, float &z)
{
   //x*x + y*y + z*z = speed*speed
   float delta = sqrt(m_spectatorMovementSpeed*m_spectatorMovementSpeed/(x*x + y*y + z*z));
   x *= delta;
   y *= delta;
   z *= delta;
}

// Исправление возможного столкновения с границей уровня(уровнем земли по умолчанию) по Y
void Camera::avoidCameraCollision()
{
   if (!m_blockCameraCollision)
      return;
   if (m_eyeY > COLLISION_MIN_BORDER)
      m_eCCollision = VBC_NOCOLLISION;
   if ((m_eyeY < COLLISION_MIN_BORDER)
      && (!m_eCCollision))
   {
      if (m_bCCToggled)
         m_lookatY -= (m_eyeY - COLLISION_MIN_BORDER);
      m_eyeY = COLLISION_MIN_BORDER;
      m_eCCollision = VBC_COLLISION;
   }
   if ((m_cameraMoveType == VBC_CM_ORBITAL)
      && (m_eCCollision))
   {
      if (m_lookatY < EARTH_LEVEL)
      {
         float tmp_x, tmp_y, tmp_ty, tmp_z, dw;
         tmp_x = (m_eyeX - m_lookatX);
         tmp_y = (m_eyeY - m_lookatY);
         tmp_ty = (m_eyeY - EARTH_LEVEL);
         tmp_z = (m_eyeZ - m_lookatZ);
         dw = (EARTH_LEVEL - m_lookatY) / tmp_y;
         m_lookatX += tmp_x*dw;
         m_lookatY = EARTH_LEVEL;
         m_lookatZ += tmp_z*dw;
      }
   }
   else
      ;
   if (m_bCCToggled)
      m_bCCToggled = VBC_FALSE;
}

// Обновление матрицы вида устройства d3dd()
void Camera::updateMatrixes()
{
   avoidCameraCollision();

   m_d3dd->SetTransform(D3DTS_PROJECTION, &m_mProj);

   //D3DXMATRIXA16 mView;
   D3DXMatrixLookAtLH(&m_mView, &D3DXVECTOR3(m_eyeX, m_eyeY, m_eyeZ),
      &D3DXVECTOR3(m_lookatX, m_lookatY, m_lookatZ),
      &D3DXVECTOR3(1.0f, 100.0f, 1.0f));
   m_d3dd->SetTransform(D3DTS_VIEW, &m_mView);
}