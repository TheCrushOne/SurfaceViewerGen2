/*!
\file
\brief ������������ ���� MathModelInterface.h
����������� ����������� ��� ������ � �������������� ������� �������� �������
*/

#pragma once
#include "BaseRef.h"
#include "CommonStructs.h"
#include "WeatherInterface.h"

struct ICommunicator;

namespace colreg
{
   constexpr double SPEED_ZERO = 0.00001;
   constexpr double OVERFLOW_MIN_RUDDER_ANGLE = std::numeric_limits<double>::min();
   constexpr double OVERFLOW_MAX_RUDDER_ANGLE = std::numeric_limits<double>::max();

   enum ECheckManeuver
   {
     CHM_Pass = 0,
     CHM_Fail
   };

#pragma pack (push, 1)
   struct model_point
   {
      model_point()
         : course{ 0. }
         , speed{ 0. }
         , time{ 0. }
      {}
      colreg::polar pos;
      double course;
      double speed;
      double time;
   };


   /**
   * \interface iManeuverMath
   * ����������� ��������� � ���������� �������
   * ������ ������������� � ������������� ������� ��������� � ������� � ������ ����� ��������
   */
   struct iManeuverMath : public colreg::iReleasable
   {
      /*!
      �������� ������� �� ������� �� ������� ������������ ������ �������
      \param[in] time ����� ������������ ������ �������, �������
      \return ������� ���. ������ �� ������� �� ����� time
      */
      virtual colreg::model_point GetRelPositionByTime(double time)const = 0;

      /*!
      �������� ����� �� ������� �� �������
      \param[in] p ������� � ������� ��������� �������, �� ������� ���� �������� �� ������ ��� ����������� �������
      \return ����� �� �������, � ��������
      */
      virtual double GetTimeByRelPosition(const colreg::polar& p, double time_accuracy=0.1)const = 0;

      //! �������� �������� � ����� �������, ����
      virtual double GetSpeed()const = 0;

      //! �������� ��������� �������, �����
      virtual double GetLenght()const = 0;

      //! �������� ����� �� ���������� �������, �������
      virtual double GetTime()const = 0;

      //! �������� ����� ������ �� �������
      virtual colreg::model_point GetEnd()const = 0;

      virtual iManeuverMath* Clone() const = 0;

      virtual ECheckManeuver CheckManeuver()const = 0;
   };
   
   /*
   ��������� ��� ������� ���������� ����� � ���� �������.
   */
   struct ManeuverEnvironment
   {
      double shallowWaterScale = colreg::NO_VALUE; // ���������� = �������/������ ��� ����� ������� ����������.
      weather_info weather;
      bool InactiveEnvironment()const
      {
        if (InactiveWind() && InactiveWave() && InactiveCurrent() && InactiveShallow())
          return true;
        else
          return false;
      }
      static constexpr double InactiveWindSpeed = 0.001;
      static constexpr double InactiveWaveHeight = 0.001;
      static constexpr double InactiveCurrentSpeed = 0.001;
      static constexpr double InactiveShallowDepth = 100.0;
      bool InactiveWind()const { return weather.windSpeed < InactiveWindSpeed ? true : false; }
      bool InactiveWave()const { return weather.waveHeight < InactiveWaveHeight ? true : false; }
      bool InactiveCurrent()const { return weather.currentSpeed < InactiveCurrentSpeed ? true : false; }
      bool InactiveShallow()const { return shallowWaterScale > InactiveShallowDepth ? true : false; }
   };

   using speeds_ref = base_ref<double>;

   /**
   * \interface iMathModel
   * ����������� ��������� �������������� ������ �������
   */
   struct SVesselState
   {
     SVesselState(): speed(0.0), speedSetted(0.0), speedTelegraph(0.0) {}
     SVesselState(double speed_, double speedSetted_) : speed(speed_), speedSetted(speedSetted_), speedTelegraph(speedSetted_) {}
     SVesselState(double speed_, double speedSetted_, double speedTelegraph_) : speed(speed_), speedSetted(speedSetted_), speedTelegraph(speedTelegraph_){}
     double speed; // actual speed [knots]
     double speedSetted; // speed before circulation [knots]
     double speedTelegraph; // speed according to telegraph [knots]
   };

   struct iVesselMathModel
   {
      //! ����� �������
      virtual double GetLength()const = 0;
      
      //! ������ ������� 
      virtual double GetWidth()const = 0;
      
      //! ������ ������� 
      virtual double GetDraft()const = 0;

      //! ��� ������� 
      virtual const char* GetName()const = 0;

      virtual const char* GetNtproName()const = 0;

      //! ���������� ������������ ��������
      virtual double GetFullSpeed(const ManeuverEnvironment* env = nullptr)const = 0;

      //! ������������ ���� ���������� ����
      virtual double GetMaxRudderAngle()const = 0;

      //! ����������� ���� ���������� ����
      virtual double GetMinRudderAngle()const = 0;

      //! ���� ���������� ���� �� ������� ��������
      virtual double GetRudderAngle(double radius, const SVesselState *state=nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! ������ �������� �� ���� ����������� ����, � �����
      virtual double GetRadius(double rudderAngle, const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;
      virtual double GetRadius(double turnAngle, double rudderAngle, const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! ���������� "�������" ���������� �� ��������, � �����
      virtual double GetS0(double rudderAngle, const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! ���������� ��������� �� ���������� ����� �� ������ ����������, � �����
      virtual double GetDistanceToTurn(double turnAngle, double turnAngleFull, double rudderAngle, 
        const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! ���������� ����� ������ �� ����������
      virtual colreg::polar GetExitTurnPoint(double turnAngle, double rudderAngle, 
      const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! ���������� ��������� �� ���������� ����� �� ������ ����������, � �����
      virtual double GetDistanceAfterTurn(double turnAngle, double turnAngleFull, double rudderAngle, 
      const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! ���������� ��������� �� ������ ��������� �� �������� speed, � �����
      virtual double GetDistanceToStop(double speed, const ManeuverEnvironment* env = nullptr)const = 0;

      //! ���������� �������������� �������� ��������, � �����
      virtual const speeds_ref* GetSupportedSpeeds()const = 0; 
      virtual const speeds_ref* GetTelegraphPoints()const = 0; // 0-100, [%]
      virtual const double GetSpeedByTelegraph(double tel, const colreg::ManeuverEnvironment* env = nullptr)const = 0; // speed[knots], telegraph: 0-100, [%]

      /*!
      �������� ���������� ��� ������� ������� �����������.
      ������� >180 ���������� ����� ���������. �������� ������� �� 180 -  90 � turnAngleFull = 0 + 90 � turnAngleFull = 180. 
      \param[in] speed �������� �������� �� ����� � ���� ����������, ����
      \param[in] speedSetted �������� �������� �������� �� ����� � ���� ����������, ����
      \param[in] turnAngle ���� �������� (�� 180 ��.), �������
      \param[in] turnAngleFull ����� ���� �������� � ������ ����������, �������
      \param[in] rudderAngle ���� ���������� ����, �������
      */
      virtual iManeuverMath* Circulation(double speed, double speedSetted, double speedTelegraph, double turnAngle, double turnAngleFull, double rudderAngle,
        const ManeuverEnvironment* env = nullptr)const = 0;

      /*!
      �������� ���������� ��� �������� ������
      \param[in] speed �������� ��������, ����
      \param[in] dist ������� ���������, �����
      */
      virtual iManeuverMath* MoveForward(double speed, double dist, const ManeuverEnvironment* env = nullptr)const = 0;

      /*!
      �������� ���������� ��� ���������/����������
      \param[in] speedFrom ��������� �������� ��������, ����
      \param[in] speedTo �������� �������� ��������, ����
      \param[in] maxDist ������������ ��������� �� ������ ��������� ��������, �����
      */
      virtual iManeuverMath* Accelerate(double speedFrom, double speedTo, double maxDist, const ManeuverEnvironment* env = nullptr)const = 0;
      virtual ~iVesselMathModel() = default;

      virtual ECheckManeuver CheckCirculation(double turnAngle, double rudderAngle, const SVesselState& state, const ManeuverEnvironment& env)const = 0;
      virtual ECheckManeuver CheckMoveForward(double speed, const ManeuverEnvironment& env)const = 0;
      virtual ECheckManeuver CheckAcceleration(double speedFrom, double speedTo, const ManeuverEnvironment& env)const = 0;
      virtual size_t GetForbiddenCourseSectors(double speed, const colreg::ManeuverEnvironment& env,
        size_t maxsize, colreg::forbidden_turn_sector *sectors)const = 0;
      virtual bool GetForbiddenSpeed(const colreg::ManeuverEnvironment& env,
        double &MinSpeed, double &MaxSpeed)const = 0;

      virtual void SetCommunicator(ICommunicator* comm) = 0;
   };

   using models_ref = base_ref<iVesselMathModel*>;
   using model_name_ref = base_ref<const char*>;
   /**
   * \interface iMathModelManager
   * ����������� ��������� ��� ������� � ���. ������� �������� ��������
   */
   struct iMathModelManager : colreg::iReleasable
   {
      //! ��������� ������ �� ���������� �������
      virtual const iVesselMathModel* SelectMathModelByShip(const ship_info& info) const = 0;
      virtual const colreg::iVesselMathModel* GetMathModelByName(const char* modelName) const = 0;
      virtual model_name_ref GetModelList() const = 0;
      virtual void SetCommunicator(ICommunicator* comm) = 0;

      //! �������� �������������� ������
      virtual const models_ref* GetModels()const = 0;

      //! ���/���� ��������� ������
      virtual void UseWeather(bool use)const = 0;
      virtual bool IsUseWeather()const = 0;
   };

#pragma pack(pop)
}

#ifdef COLREGDLL_EXPORTS
#define MATH_MODEL_EXPORT_IMPORT __declspec(dllexport) // export DLL information
#else
#define MATH_MODEL_EXPORT_IMPORT __declspec(dllimport) // import DLL information
#endif

/*!
�������� ��������� �������
\param[in] path ���� � ����� ColregData, ���������� ����� models/weatherModels � ������� ������� *.xml
*/
extern "C" MATH_MODEL_EXPORT_IMPORT colreg::iMathModelManager* __cdecl CreateModelManager(const char* path);