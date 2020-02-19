#include "stdafx.h"
#include "utils/utils.h"
#include "car_model/carmove.h"
#include "engine/engine.h"
#include "utils/camera.h"
#include "car_model\carlight.h"

const float CarMove::DEFAULT_TURN_RAD = 3.0f;
const float CarMove::DEFAULT_STRAIGHT_MOVE_SPEED = 0.132f; // 0.265f; - слишком быстро
const float CarMove::DEFAULT_ANGLE_SPIKE_DELTA = D3DX_PI/12;
const float CarMove::DEFAULT_SPEED_SPIKE_DELTA = 0.005f;
const float CarMove::DEFAULT_SPEEDUP_PITCH = 0.005f; //д.б. больше спайка
const float CarMove::DEFAULT_Y_SPIKE = 0.75f;        //хз почему теперь не 0.15
const float CarMove::DEFAULT_TURN_ANGLE_SPIKE = 0.1f;
const float CarMove::DEFAULT_TURN_RAD_SPIKE = 0.1f;


const float CarMove::DEFAULT_STEERING_LIMIT = D3DX_PI / 4.0f;
const DWORD CarMove::DEFAULT_GEAR_LIMIT = 6;
const float CarMove::DEFAULT_INERTIAL_PARAMETER = 0.07f;
const float CarMove::DEFAULT_STEERING_WHEEL_RETURN_ANGLE_PITCH = 0.05f;
const DWORD CarMove::DEFAULT_GEAR_SHIFT_FRAME_COUNT = 10;
const float CarMove::DEFAULT_GEAR_SHIFT_KOEFF = 0.1f;
const float CarMove::DEFAULT_CAR_LENGTH = 4.15f;
const float CarMove::TURN_ANGLE_SPIKE = 0.01f;
const float CarMove::DEFAULT_SLOWDOWN_PARAMETER = 0.07f;
const float CarMove::DEFAULT_GEAR_SHIFT_DELTA = 0.01f;

CarMove::CarMove(std::shared_ptr<Engine>& eng, LPCWSTR waypointFile, float ySpike)
   : m_engine(eng)
{
    getWay(waypointFile);
    initService();
}

CarMove::~CarMove()
{
    free(m_way);
    delete m_date;
}

// Переключение режима управления (ручной/авто)
void CarMove::SwitchUserControl()
{
   m_wpType = (m_wpType == WBC_CYCLED) ? WBC_USERCONTROLLED : WBC_CYCLED;
   m_engine->GetCamera()->SetUCSCamera(m_wpType == WBC_USERCONTROLLED);
}

// Обработка движения при ручном управлении
void CarMove::ProcessMoveControl(float dx, float dy)
{
   static float pdy = 0.0f, prevSpeed = 0.0f;
   static float mRec = 0.0f;

   if (dx != 0.0f) //не факт...проверить
      m_turnAngle += dx/(1.0f + 10.0f*fabs(m_straightMoveSpeed));
   else
      steerBack();
   correctTurnAngle();

   SHAMANISTIC_INSTANT_CAST(dy);

   checkGearShiftUpStatus();

   mRec = movementRecount(dy);

   if (m_accelerator == WBC_BRAKE)
      m_straightMoveSpeed += (m_straightMoveSpeed > 0.0f ? -mRec : mRec);
   else if (m_accelerator == WBC_IDLE)
      m_straightMoveSpeed += (m_straightMoveSpeed > 0.0f ? mRec : -mRec);
   else //WBC_THROTTLE
   {
      if (m_driveType == WBC_DRIVE)
         m_straightMoveSpeed += mRec;
      else if (m_driveType == WBC_NEUTRAL)
         m_straightMoveSpeed += (m_straightMoveSpeed > 0.0f ? mRec : -mRec);
      else
         m_straightMoveSpeed -= mRec;
   }

    ((prevSpeed*m_straightMoveSpeed < 0.0f)
        || ((fabs(prevSpeed) - fabs(m_straightMoveSpeed) < 0.0f)
        && (m_accelerator == WBC_BRAKE))) ?
        m_straightMoveSpeed = 0.0f : 
        1;

    prevSpeed = m_straightMoveSpeed;

    //pdy = dy;

    processCoordinates();
}

// Обработка переключения передач(как в акпп D/N/R без 1/2)
void CarMove::ToggleTransmissionDMode(DMSTYPE type)
{
   if (type == WBC_DOWN)
   {
      if (m_driveType == WBC_REVERSE)
         return;
      if (m_driveType == WBC_NEUTRAL)
      {
         m_driveType = WBC_REVERSE;
         m_engine->GetCarlight()->ToggleState(CBN_REAR_REVERSE_LIGHTS, true);
         m_gear = -1;
      }
      else if (m_driveType == WBC_DRIVE)
      {
         m_driveType = WBC_NEUTRAL;            
         m_gear = 0;
      }
      else
         ;
   }
   else if (type == WBC_UP)
   {
      if (m_driveType == WBC_DRIVE)
         return;
      if (m_driveType == WBC_NEUTRAL)
      {
         m_driveType = WBC_DRIVE;            
         m_gear = 1;
      }
      else if (m_driveType == WBC_REVERSE)
      {
         m_driveType = WBC_NEUTRAL;
         m_engine->GetCarlight()->ToggleState(CBN_REAR_REVERSE_LIGHTS, false);
         m_gear = 0;
      }
      else
         ;
   }
   else
      ;
}

// Старт постепенной остановки машины
void CarMove::CarStop()
{
   m_speedup = false;
   m_slowdown = true;
}

// Запуск машины
void CarMove::CarLaunch()
{
   m_speedup = true;
}

// Взятие данных о траектории из файла
void CarMove::getWay(LPCWSTR waypointFile)
{
   std::ifstream wpf;
   char ftmp[256], ttmp[256];
   char rtmp[256] = "";

   wpf.open(waypointFile);

   if (!wpf)
      throw std::runtime_error("crtitical : no file");

   wpf.getline(ftmp, 256, ' ');
   if (strcmp(ftmp, "fwpd"))
      throw std::runtime_error("no fwpd");
   wpf.getline(ftmp, 256, '_');

   m_date = new char[strlen(ftmp) + 1];
   m_date[0] = 0;
   strcat_s(m_date, strlen(ftmp) + 1, ftmp);

   wpf.getline(ftmp, 256);
   if (strcmp(ftmp, "10219239"))
      throw std::runtime_error("wrong checksum");

   wpf.getline(ttmp, 256, ' ');
   if (strcmp(ttmp, "type"))
      throw std::runtime_error("no type identifier");

   wpf.getline(ttmp, 256);
   if (!strcmp(ttmp, "cycled"))
      m_wpType = WBC_CYCLED;
   else if (!strcmp(ttmp, "nocycled"))
      m_wpType = WBC_NOCYCLED;
   else if (!strcmp(ttmp, "usercontrolled"))
      m_wpType = WBC_USERCONTROLLED;
   else
      ;

   while (strcmp(rtmp, "start"))
      wpf.getline(rtmp, 256);
   int i = 0, sz = 10;
   m_way = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3)*sz);
   D3DXVECTOR3 *tmp_ptr = 0;
   do
   {
      if (i == sz)
      {
         sz += 10;
         tmp_ptr = m_way;
         m_way = (D3DXVECTOR3*)realloc(tmp_ptr, sz*sizeof(D3DXVECTOR3));
      }
      wpf.getline(rtmp, 256, ' ');
      if (!strcmp(rtmp, "wp"))
      {
         wpf.getline(rtmp, 256, ' ');
         m_way[i].x = static_cast<float>(atof(rtmp));
         wpf.getline(rtmp, 256);
         m_way[i].z = static_cast<float>(atof(rtmp));
         m_way[i].y = DEFAULT_Y_SPIKE;
         i++;
      }
   }
   while (!strstr(rtmp, "finish"));

   m_wpCount = i;
   m_lastWaypointNum = 0;
   m_lastWaypointPos = m_way[0];
   m_currentMoveType = WBC_STRAIGHT;
   m_pos = m_way[0];
   m_angleYaw = D3DX_PI / 2;
   correctDirection();
}

// Инициализация переменных параметров класса перемещения машины(пока что по default'у)
void CarMove::initService()
{
   m_angleSpikeDelta = DEFAULT_ANGLE_SPIKE_DELTA;
   m_turnRad = DEFAULT_TURN_RAD;
   if (m_engine->GetStartTransformAngle() != 0.0f)
      m_straightMoveSpeed = 0.0f;
   else
      m_straightMoveSpeed = DEFAULT_STRAIGHT_MOVE_SPEED;
   m_turnAngleSpike = DEFAULT_TURN_ANGLE_SPIKE;
   m_turnRadSpike = DEFAULT_TURN_RAD_SPIKE;
   m_carLength = DEFAULT_CAR_LENGTH;

   m_turnAngle = 0.0f;
   m_steeringLimit = DEFAULT_STEERING_LIMIT;
   m_gear = 0;
   m_inertialParameter = DEFAULT_INERTIAL_PARAMETER;
   m_stWhReturnAnglePitch = DEFAULT_STEERING_WHEEL_RETURN_ANGLE_PITCH;
   m_gearShiftFrameCount = DEFAULT_GEAR_SHIFT_FRAME_COUNT;
   m_gearShiftKoeff = DEFAULT_GEAR_SHIFT_KOEFF;
   m_shiftStatus = false;
   m_slowdownParameter = DEFAULT_SLOWDOWN_PARAMETER;
   m_gearShiftDelta = DEFAULT_GEAR_SHIFT_DELTA;
   m_accelerator = WBC_IDLE;
   m_powerBound = false;
   m_driveType = WBC_NEUTRAL;

   m_slowdown = false;
   m_speedup = false;
}

// Обработка одношагового перемещения на след. позицию
void CarMove::moveNext()
{
   static bool started = false;
   static D3DXVECTOR3 mdt = { 0, 0, 0 };
   static int ctt = 0;
   static float prevYaw = 0.0f;
    
    
   if (m_currentMoveType == WBC_STRAIGHT)
   {
      mdt = getMoveDelta();
      if ((ctt = checkTurnToggle(mdt)) != 0)
      {
         if (ctt == 1)
            m_currentMoveType = WBC_TURN_LEFT;
         else
            m_currentMoveType = WBC_TURN_RIGHT;
         WAYPOINT_NUMBER_DYNAMIC_CAST;
      }
      else
         m_pos += mdt;
   }
   else if (m_currentMoveType == WBC_TURN_LEFT)
   {
      if ((ctt = checkTurnToggle()) != 0)
      {
         m_currentMoveType = WBC_STRAIGHT;
         correctDirection();
         //wbc_lastTraversedNum++;
      }
      else
      {
         mdt = getMoveDelta();
         movePos(mdt, prevYaw);
         //wbc_pos += mdt;
      }
   }
   else if (m_currentMoveType == WBC_TURN_RIGHT)
   {
      if ((ctt = checkTurnToggle()) != 0)
      {
         m_currentMoveType = WBC_STRAIGHT;
         correctDirection();
         //wbc_lastTraversedNum++;
      }
      else
      {
         mdt = getMoveDelta();
         movePos(mdt, prevYaw);
         //wbc_pos += mdt;
      }
   }
   else
      ;

   prevYaw = m_angleYaw;
}

// Вычисление покоординатного и углового перемещения за 1 шаг
D3DXVECTOR3 CarMove::getMoveDelta()
{
   float dx, dz, mk;
   static float rr = 0.0f, rf = 0.0f, phi = 0.0f;
   if (m_currentMoveType == WBC_STRAIGHT)
   {
      dx = m_way[(m_lastWaypointNum + 1) % m_wpCount].x - m_pos.x;
      dz = m_way[(m_lastWaypointNum + 1) % m_wpCount].z - m_pos.z;
      mk = sqrt((m_straightMoveSpeed*m_straightMoveSpeed)/(dx*dx + dz*dz));
      dx *= mk;
      dz *= mk;
   }
   else if (m_currentMoveType == WBC_TURN_LEFT)
   {
      m_angleYaw -= m_straightMoveSpeed / m_turnRad;
      dx = m_turnRad * (cos(m_angleYaw - m_straightMoveSpeed / m_turnRad) - cos(m_angleYaw));
      dz = -m_turnRad * (sin(m_angleYaw - m_straightMoveSpeed / m_turnRad) - sin(m_angleYaw));
   }
   else if (m_currentMoveType == WBC_TURN_RIGHT)
   {
      m_angleYaw += m_straightMoveSpeed / m_turnRad;
      dx = -m_turnRad*(cos(m_angleYaw + m_straightMoveSpeed / m_turnRad) - cos(m_angleYaw));
      dz = m_turnRad*(sin(m_angleYaw + m_straightMoveSpeed / m_turnRad) - sin(m_angleYaw));
   }
   else
      ;

   return D3DXVECTOR3(dx, 0.0f, dz);       //пока что никаких перемещений по Y
}

// Вычисление покоординатного и углового перемещения за 1 шаг
void CarMove::movePos(D3DXVECTOR3 mdt, float prevYaw)
{
   static D3DXVECTOR3 dlt = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rVct = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

   dlt = D3DXVECTOR3((m_carLength / 2.0f)*sin(prevYaw), 0.0f, (m_carLength / 2.0f)*cos(prevYaw));
   rVct = m_pos - dlt;

   rVct += mdt;

   dlt = D3DXVECTOR3((m_carLength / 2.0f)*sin(m_angleYaw), 0.0f, (m_carLength / 2.0f)*cos(m_angleYaw));

   m_pos = rVct + dlt;
}

// Перенаправление на фукнцию проверки окончания движения вперед/поворота в сторону в случае поворота
inline int CarMove::checkTurnToggle()
{
   return checkTurnToggle(D3DXVECTOR3(0, 0, 0));
}

// Проверка окончания движения вперед/поворота в сторону
int _fastcall CarMove::checkTurnToggle(D3DXVECTOR3 mdt)
{
   static D3DXVECTOR3 tmp_next = { 0, 0, 0 };
   static float wpAngle = 0.0f, dAngle = 0.0f;
   if (m_currentMoveType == WBC_STRAIGHT)
   {
      tmp_next = m_pos + mdt;
      if ((((m_way[(m_lastWaypointNum + 1) % m_wpCount].x >= m_pos.x)
         && (m_way[(m_lastWaypointNum + 1) % m_wpCount].x <= tmp_next.x))
         || ((m_way[(m_lastWaypointNum + 1) % m_wpCount].x <= m_pos.x)
         && (m_way[(m_lastWaypointNum + 1) % m_wpCount].x >= tmp_next.x)))
         && (((m_way[(m_lastWaypointNum + 1) % m_wpCount].z >= m_pos.z)
         && (m_way[(m_lastWaypointNum + 1) % m_wpCount].z <= tmp_next.z))
         || ((m_way[(m_lastWaypointNum + 1) % m_wpCount].z <= m_pos.z)
         && (m_way[(m_lastWaypointNum + 1) % m_wpCount].z >= tmp_next.z))))
      {
         //код некрасивый какой-то...исправить при случае
         if (((m_way[(m_lastWaypointNum + 2)].x - m_pos.x)*(m_way[(m_lastWaypointNum + 2)].x - m_pos.x) + (m_way[(m_lastWaypointNum + 2)].z - m_pos.z)*(m_way[(m_lastWaypointNum + 2)].z - m_pos.z)) >= 4*(m_turnRad + m_turnRadSpike)*(m_turnRad + m_turnRadSpike))
            if ((mdt.z*(m_way[(m_lastWaypointNum + 2) % m_wpCount].x - m_pos.x) - mdt.x*(m_way[(m_lastWaypointNum + 2) % m_wpCount].z - m_pos.z)) >= 0)
               return -1;
            else
               return 1;
         else
            if ((mdt.z*(m_way[(m_lastWaypointNum + 2) % m_wpCount].x - m_pos.x) - mdt.x*(m_way[(m_lastWaypointNum + 2) % m_wpCount].z - m_pos.z)) >= 0)
               return 1;
            else
               return -1;
      }
   }
   else if ((m_currentMoveType == WBC_TURN_LEFT)
      || (m_currentMoveType == WBC_TURN_RIGHT))
   {
      if ((m_way[(m_lastWaypointNum + 1) % m_wpCount].z - m_pos.z) >= 0)
         wpAngle = atan((m_way[(m_lastWaypointNum + 1) % m_wpCount].x - m_pos.x) / (m_way[(m_lastWaypointNum + 1) % m_wpCount].z - m_pos.z));
      else
         wpAngle = D3DX_PI + atan((m_way[(m_lastWaypointNum + 1) % m_wpCount].x - m_pos.x) / (m_way[(m_lastWaypointNum + 1) % m_wpCount].z - m_pos.z));
      regrowthAngle(wpAngle);
      regrowthAngle(m_angleYaw);
      m_angleSpikeDelta < (m_straightMoveSpeed / m_turnRad) ? m_angleSpikeDelta = m_straightMoveSpeed / m_turnRad + 0.5f : 1;
      if (fabs(m_angleYaw - wpAngle) <= m_angleSpikeDelta)
      {       
         if (m_currentMoveType == WBC_TURN_LEFT)
            dAngle = -(m_straightMoveSpeed / m_turnRad + m_turnAngleSpike);
         else
            dAngle = m_straightMoveSpeed / m_turnRad + m_turnAngleSpike;
         if ((m_angleYaw - wpAngle)*(m_angleYaw + dAngle - wpAngle) <= 0)
            return 1;
      }
   }    
   else
   ;

   return 0;
}

// Исправления угла направления в случае перелета при повороте(при 60 фпс изменения не заметны, но позволяют снизить накапливающуюся ошибку)
inline void CarMove::correctDirection()
{
   //wbc_angleYaw = atan((wbc_way[(wbc_lastTraversedNum + 1) % wbc_wpCount].x - wbc_pos.x) / (wbc_way[(wbc_lastTraversedNum + 1) % wbc_wpCount].z - wbc_pos.z));
   if ((m_way[(m_lastWaypointNum + 1) % m_wpCount].z - m_pos.z) >= 0)
      m_angleYaw = atan((m_way[(m_lastWaypointNum + 1) % m_wpCount].x - m_pos.x) / (m_way[(m_lastWaypointNum + 1) % m_wpCount].z - m_pos.z));
   else
      m_angleYaw = D3DX_PI + atan((m_way[(m_lastWaypointNum + 1) % m_wpCount].x - m_pos.x) / (m_way[(m_lastWaypointNum + 1) % m_wpCount].z - m_pos.z));
   regrowthAngle(m_angleYaw);
}

// Подгон угла в случае вылета за диапазон (-PI;PI)
inline void CarMove::regrowthAngle(float &angle)
{
   while (angle >= D3DX_PI)
      angle = - 2*D3DX_PI + angle;
   while (angle <= -D3DX_PI)
      angle = 2*D3DX_PI + angle;
}

// Обработка движения машины
void CarMove::processCoordinates()
{
   static D3DXVECTOR3 dlt, rVct;
   static float prevYaw = 0.0f;
   static float tmpRad = 0.0f;
   static float dx = 0.0f, dz = 0.0f;

   static float dbg_x = 0.0f, dbg_z = 0.0f;

   prevYaw = m_angleYaw;

   if (m_turnAngle != 0.0f)
      tmpRad = (1.0f/tanh(m_turnAngle))*(m_carLength / 2.0f);

   if (m_turnAngle == 0.0f)
   {
      dx = m_straightMoveSpeed*cos(D3DX_PI / 2 - m_angleYaw);
      dz = m_straightMoveSpeed*sin(D3DX_PI / 2 - m_angleYaw);
   }       //из-за знака тангенса не требуется \/
   else
   {
      dx = -tmpRad*(cos(m_angleYaw + m_straightMoveSpeed / tmpRad) - cos(m_angleYaw));
      dz = tmpRad*(sin(m_angleYaw + m_straightMoveSpeed / tmpRad) - sin(m_angleYaw));
      m_angleYaw += m_straightMoveSpeed / tmpRad;

      dlt = D3DXVECTOR3((m_carLength / 2.0f)*sin(prevYaw), 0.0f, (m_carLength / 2.0f)*cos(prevYaw));
      rVct = m_pos - dlt;

      dbg_x = rVct.x + tmpRad*cos(m_angleYaw);
      dbg_z = rVct.z - tmpRad*sin(m_angleYaw);
   }

   m_engine->DrawDbgSphere(D3DXVECTOR3(dbg_x, 0.7f, dbg_z));

   movePos(D3DXVECTOR3(dx, 0.0f, dz), prevYaw);
}

// Возврат угла поворота(как возврат руля)
void CarMove::steerBack()
{
   m_turnAngle += -m_turnAngle*m_stWhReturnAnglePitch / (1.0f + 10.0f*fabs(m_straightMoveSpeed));
   if (fabs(m_turnAngle) <= TURN_ANGLE_SPIKE)
      m_turnAngle = 0.0f;
}

// Подгон угла в случае вылета за диапазон (-stl;stl)
void CarMove::correctTurnAngle()
{
   static float rcntLimit = 0.0f;
   rcntLimit = m_steeringLimit*(1.0f - 2.0f*atan(fabs(20.0f*m_straightMoveSpeed))/D3DX_PI);
   m_turnAngle > (rcntLimit) ?
      m_turnAngle = (rcntLimit) :
      (m_turnAngle < -rcntLimit) ?
      m_turnAngle = -(rcntLimit) :
      1;
}

// Обработка переключения АКПП
void CarMove::checkGearShiftUpStatus()
{
   static float prevSpeed = 0.0f;
   static DWORD idleFrameCount = 0;
   static bool shiftUp = false;
   m_powerBound = false;

   //нужна дельта...а то трасмиссия уходит в бесконечный цикл
   if ((m_driveType != WBC_REVERSE)
      && (m_straightMoveSpeed >= m_gearShiftKoeff*m_gear + m_gearShiftDelta))
   {
      if (m_gear > 0)
      {
         shiftUp = true;
         m_shiftStatus = true;
      }
      else
         m_powerBound = true;
   }
   if (m_straightMoveSpeed <= m_gearShiftKoeff*(m_gear - 1) - m_gearShiftDelta)
   {
      if (m_gear > 1)
      {
         shiftUp = false;
         m_shiftStatus = true;
      }
      else
         m_powerBound = true;
   }
   prevSpeed = m_straightMoveSpeed;

   if (idleFrameCount >= m_gearShiftFrameCount)
   {
      m_shiftStatus = false;
      m_gear = (shiftUp) ? m_gear + 1 : m_gear - 1;
      idleFrameCount = 0;
   }
   else if (m_shiftStatus)
      idleFrameCount++;
   else
      ;
}

// Обработка переключения АКПП
float CarMove::movementRecount(float dy)
{
   //if (dy == 0.0f)       //криво...
   //    return -wbc_straightMoveSpeed;
   static ATYPE prevAcc = m_accelerator;
   if (dy < 0.0f)
      m_accelerator = WBC_BRAKE;
   else if (dy/*wbc_straightMoveSpeed*/ > 0.0f)
      m_accelerator = WBC_THROTTLE;
   else
      m_accelerator = WBC_IDLE;

   (prevAcc != m_accelerator) ? (m_engine->GetCarlight()->ToggleState(CBN_REAR_BRAKE_LIGHTS, SHAMANISTIC_DYNAMIC_REINTERPRET_BRAKE(m_accelerator))) : 1;
   prevAcc = m_accelerator;
   //пока что так
   if (m_accelerator == WBC_BRAKE)
        dy *= 3.0f;     //тормозное усилие х3
    else if ((m_shiftStatus)
        || (m_powerBound)
        || (m_driveType == WBC_NEUTRAL))
        dy = 0.0f;
    else
        ;
    //fabs(wbc_straightMoveSpeed(1.0f/wbc_inertialParameter + wbc_slowdownParameter) - dy) >= 0.0f;
    return (fabs(dy) - fabs(m_straightMoveSpeed)*m_slowdownParameter)*m_inertialParameter;
}

// Проверка скорости и соотв. тумблерного переключения необх. параметров
void CarMove::speedCheck()
{
   //если скорость ничтожно мала, можно...даже нужно приравнять к нулю
   if ((m_slowdown)
      &&(uts::fAbs(m_straightMoveSpeed) <= DEFAULT_SPEED_SPIKE_DELTA))
      m_straightMoveSpeed = 0.0f;

   if ((m_slowdown)
      && (m_straightMoveSpeed != 0.0f))
      m_straightMoveSpeed -= m_straightMoveSpeed*m_slowdownParameter*m_inertialParameter;
   if ((m_speedup)
      && (m_straightMoveSpeed < DEFAULT_STRAIGHT_MOVE_SPEED))
   {
      m_straightMoveSpeed += DEFAULT_SPEEDUP_PITCH;
      if (m_straightMoveSpeed > DEFAULT_STRAIGHT_MOVE_SPEED)
      {
         m_speedup = false;
         m_straightMoveSpeed = DEFAULT_STRAIGHT_MOVE_SPEED;
      }
   }
   if (m_straightMoveSpeed == 0.0f)
   {
      if (m_slowdown)
      {
         m_slowdown = false;
         m_engine->StartWRotate();
      }
   }
}