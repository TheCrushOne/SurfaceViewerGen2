#pragma once

#define MAX_LIGHT_COUNT 8   //���� ����� ����������� � ��� ���� ������(�.�. ��� ������ ��� ������)(����� ������ ��� �������� �� �������)

enum LTTYPE
{
   DBC_NONE,
   DBC_MARKER,
   DBC_ALL
};

enum SGNTYPE
{
   DBC_LEFT,
   DBC_RIGHT,
   DBC_WARNING
};

enum LTYPE
{
   OBL_MOONLIGHT,
   OBL_HEADLIGHT,	//������� ����
   OBL_NEONLIGHT,
   OBL_LANTERNLIGHT
};

enum RTYPE
{
   SBM_ORDERED,
   SBM_DISORDERED,
   SBM_CHAOTIC     //�� ������ ��������������
};

enum PRIORTYPE
{
   SBM_PRIORDECLSPEC,  //��������� �������� � ���
   SBM_PRIORLOW,
   SBM_PRIORMEDIUM,
   SBM_PRIORHIGH      //�������� ������
};

enum ELTYPE
{
   CBE_DLIGHT,
   CBE_PLIGHT
};

enum CLTYPE
{
   CBN_FRONT_DIPPED_HEADLIGHTS,
   CBN_FRONT_BEAM_HEADLIGHTS,
   CBN_REAR_MARKER_LIGHTS,
   CBN_REAR_BRAKE_LIGHTS,
   CBN_REAR_REVERSE_LIGHTS,
   CBN_LEFT_SIGNAL_LIGHT,
   CBN_RIGHT_SIGNAL_LIGHT,
};

enum WTYPE
{
   WBC_NOCYCLED,
   WBC_CYCLED,
   WBC_USERCONTROLLED      //�� ������ ���
};


enum MTYPE
{
   WBC_STRAIGHT,
   WBC_TURN_LEFT,
   WBC_TURN_RIGHT
};

enum ATYPE
{
   WBC_IDLE,
   WBC_THROTTLE,
   WBC_BRAKE
};

enum DMTYPE
{
   WBC_REVERSE,
   WBC_NEUTRAL,
   WBC_DRIVE
};

enum DMSTYPE
{
   WBC_UP,
   WBC_DOWN
};

enum STYPE
{
   MBS_AMBIENT,
   //MBS_DIRECTIONAL,
   MBS_BLINNPHONG      //��������
};

enum SLTYPE
{
   MBS_NEONLIGHT,
   MBS_LANTERNLIGHT
};

enum SLSTYPE
{
   MBS_DIRECTIONAL,
   MBS_POINT,
   MBS_CAR_VISION,
   MBS_SPOT,
   MBS_KINEMATIC_PAIR_LS1,
   MBS_KINEMATIC_PAIR_LS2,
   MBS_KINEMATIC_PAIR_LS3
};

enum WRLDTYPE
{
   MBE_STANDARD,
   MBE_ALPHADEMO
};

enum FCTYPE
{
   MBE_ENVIRONMENT,
   MBE_CARSYSTEM
};

enum TBE_OBJTYPE
{
   TBE_GROUND,
   TBE_HOUSE,
   TBE_COLUMN
};

enum DLST
{
   CBE_SPBDAY,
   CBE_MOON,
   CBE_OFF
};