#pragma once

/**
@file     carmove.h
@brief    Car map move support class
@date     Created on 24/10/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class Engine;

//#define SHAMANISTIC_DYNAMIC_REINTERPRET_THROTTLE(x) x==WBC_THROTTLE?true:false
#define SHAMANISTIC_DYNAMIC_REINTERPRET_BRAKE(x) x==WBC_BRAKE?true:false
#define WAYPOINT_NUMBER_DYNAMIC_CAST m_lastWaypointNum==(m_wpCount-1)?m_lastWaypointNum=0:m_lastWaypointNum++;

/*
\class wb_carmove
\brief - Основной класс для работы с траекторией движения машины
*/
class CarMove
{
public:
   CarMove(std::shared_ptr<Engine>& eng, LPCWSTR waypointFile, float ySpike);
   ~CarMove();

   void ProcessMoveControl(float dx, float dy);
   void ToggleTransmissionDMode(DMSTYPE type);

   void LRelease() {}
   void LRecreate() {}

   void CarStop();
   void CarLaunch();

   inline D3DXVECTOR3 GetCoordinates()
   {
      speedCheck();
      if (m_wpType != WBC_USERCONTROLLED)
         moveNext();
      return m_pos;
   }

   inline D3DXVECTOR3 GetCoordinatesWOMove() { return m_pos; }
   inline float GetAngleYaw() { return m_angleYaw; }
   inline DWORD GetLastWaypointNum() { return m_lastWaypointNum; }
   inline enum MTYPE GetCarMoveType() { return m_currentMoveType; }
   inline D3DXVECTOR3 GetLastWaypoint() { return m_way[m_lastWaypointNum]; }
   inline D3DXVECTOR3 GetNextWaypoint() { return m_way[(m_lastWaypointNum + 1) % m_wpCount]; }

   inline float GetCurrentMoveSpeed() { return m_straightMoveSpeed; }

   inline float GetCurrentTurnRadius() { return m_turnRad; }
   void SwitchUserControl();

   inline WTYPE GetWPType() { return m_wpType; }
   inline float GetCurrentTurnAngle() { return m_turnAngle; }
   inline int GetCurrentGear() { return m_gear; }
   inline bool GetShiftStatus() { return m_shiftStatus; }
   inline ATYPE GetAcceleratorStatus() { return m_accelerator; }
   inline DMTYPE GetDriveType() { return m_driveType; }
private:
   void initService();
   void getWay(LPCWSTR);
   void moveNext();
   D3DXVECTOR3 getMoveDelta();
   void movePos(D3DXVECTOR3 mdt, float prevYaw);
   inline int checkTurnToggle();
   int _fastcall checkTurnToggle(D3DXVECTOR3);
   inline void correctDirection();
   inline float truncateAngle(float);
   inline void regrowthAngle(float&);

   void processCoordinates();
   void steerBack();
   void correctTurnAngle();
   void checkGearShiftUpStatus();
   float movementRecount(float dy);

	void speedCheck();
   //float wbc_posX, wbc_posY, wbc_posZ;
   D3DXVECTOR3 m_pos;
   float m_angleYaw;

   std::shared_ptr<Engine>& m_engine;

   char* m_date;

   float m_carLength;
   float m_turnRad;  //по заднему мосту
   float m_straightMoveSpeed;
   float m_angleSpikeDelta;
   float m_turnAngleSpike;
   float m_turnRadSpike;

   //====ucs #101 alfa
   float m_turnAngle;
   float m_steeringLimit;
   int m_gear;
   DMTYPE m_driveType;
   float m_inertialParameter;
   float m_stWhReturnAnglePitch;
   DWORD m_gearShiftFrameCount;
   float m_gearShiftKoeff;
   bool m_shiftStatus;
   ATYPE m_accelerator;
   float m_slowdownParameter;
   float m_gearShiftDelta;
   bool m_powerBound;

   static const float DEFAULT_STEERING_LIMIT;
   static const DWORD DEFAULT_GEAR_LIMIT;
   static const float DEFAULT_INERTIAL_PARAMETER;
   static const float DEFAULT_STEERING_WHEEL_RETURN_ANGLE_PITCH;
   static const DWORD DEFAULT_GEAR_SHIFT_FRAME_COUNT;
   static const float DEFAULT_GEAR_SHIFT_KOEFF;
   static const float DEFAULT_CAR_LENGTH;
   static const float DEFAULT_SLOWDOWN_PARAMETER;
   static const float DEFAULT_GEAR_SHIFT_DELTA;
   //====
   D3DXVECTOR3* m_way;
   DWORD m_wpCount;
   DWORD m_lastWaypointNum;
   D3DXVECTOR3 m_lastWaypointPos;
   D3DXVECTOR3 m_currentTurnCenterPos;

   WTYPE m_wpType;
   MTYPE m_currentMoveType;

   bool m_slowdown, m_speedup;
public:
   static const float DEFAULT_TURN_RAD;
   static const float DEFAULT_STRAIGHT_MOVE_SPEED;
   static const float DEFAULT_ANGLE_SPIKE_DELTA;
   static const float DEFAULT_SPEED_SPIKE_DELTA;
   static const float DEFAULT_SPEEDUP_PITCH;
   static const float DEFAULT_Y_SPIKE;
   static const float DEFAULT_TURN_ANGLE_SPIKE;
   static const float DEFAULT_TURN_RAD_SPIKE;
   static const float TURN_ANGLE_SPIKE;
};