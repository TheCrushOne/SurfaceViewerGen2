/*!
\file
\brief Заголовочный файл MathModelInterface.h
Определение интерфейсов для работы с математической моделью движения корабля
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
   * Абстрактный интерфейс к математике маневра
   * Маневр расчитывается в прямоугольной системе координат с центром в первой точке манревра
   */
   struct iManeuverMath : public colreg::iReleasable
   {
      /*!
      Получить позицию на маневре по времени относительно начала маневра
      \param[in] time время относительно начала маневра, секунды
      \return позицию мат. модели на маневре на время time
      */
      virtual colreg::model_point GetRelPositionByTime(double time)const = 0;

      /*!
      Получить время на маневре по позиции
      \param[in] p позиция в системе координат маневра, по которой ищем проекцию на маневр для опредеоения времени
      \return время на маневре, в секундах
      */
      virtual double GetTimeByRelPosition(const colreg::polar& p, double time_accuracy=0.1)const = 0;

      //! Получить скорость в конце маневра, узлы
      virtual double GetSpeed()const = 0;

      //! Получить дистанцию маневра, метры
      virtual double GetLenght()const = 0;

      //! Получить время на выполнение маневра, секунды
      virtual double GetTime()const = 0;

      //! Получить точку выхода из маневра
      virtual colreg::model_point GetEnd()const = 0;

      virtual iManeuverMath* Clone() const = 0;

      virtual ECheckManeuver CheckManeuver()const = 0;
   };
   
   /*
   Структура для задания окружающей среды в зоне маневра.
   */
   struct ManeuverEnvironment
   {
      double shallowWaterScale = colreg::NO_VALUE; // коэфициент = глубина/осадка для учета эффекта мелководья.
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
   * Абстрактный интерфейс математической модели корабля
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
      //! длина корабля
      virtual double GetLength()const = 0;
      
      //! ширина корабля 
      virtual double GetWidth()const = 0;
      
      //! осадка корабля 
      virtual double GetDraft()const = 0;

      //! Имя корабля 
      virtual const char* GetName()const = 0;

      virtual const char* GetNtproName()const = 0;

      //! Возвращает максимальную скорость
      virtual double GetFullSpeed(const ManeuverEnvironment* env = nullptr)const = 0;

      //! Максимальный угол перекладки руля
      virtual double GetMaxRudderAngle()const = 0;

      //! Минимальный угол перекладки руля
      virtual double GetMinRudderAngle()const = 0;

      //! Угол перекладки руля по радиусу поворота
      virtual double GetRudderAngle(double radius, const SVesselState *state=nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! Радиус поворота по углу переекладки руля, в милях
      virtual double GetRadius(double rudderAngle, const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;
      virtual double GetRadius(double turnAngle, double rudderAngle, const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! возвращает "мертвый" промежуток до поворота, в милях
      virtual double GetS0(double rudderAngle, const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! возвращает дистанцию до поворотной точки от начала циркуляции, в милях
      virtual double GetDistanceToTurn(double turnAngle, double turnAngleFull, double rudderAngle, 
        const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! возвращает точку выхода из циркуляции
      virtual colreg::polar GetExitTurnPoint(double turnAngle, double rudderAngle, 
      const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! возвращает дистанцию до поворотной точки от начала циркуляции, в милях
      virtual double GetDistanceAfterTurn(double turnAngle, double turnAngleFull, double rudderAngle, 
      const SVesselState *state = nullptr, const ManeuverEnvironment* env = nullptr)const = 0;

      //! возвращает дистанцию до полной остановки от скорости speed, в милях
      virtual double GetDistanceToStop(double speed, const ManeuverEnvironment* env = nullptr)const = 0;

      //! возвращает поддерживаемые скорости движения, в узлах
      virtual const speeds_ref* GetSupportedSpeeds()const = 0; 
      virtual const speeds_ref* GetTelegraphPoints()const = 0; // 0-100, [%]
      virtual const double GetSpeedByTelegraph(double tel, const colreg::ManeuverEnvironment* env = nullptr)const = 0; // speed[knots], telegraph: 0-100, [%]

      /*!
      Получить математику для расчета маневра циркуляцией.
      Поворот >180 моделируем двумя маневрами. Например поворот на 180 -  90 с turnAngleFull = 0 + 90 с turnAngleFull = 180. 
      \param[in] speed скорость движения на входе в дугу циркуляции, узлы
      \param[in] speedSetted заданная скорость движения на входе в дугу циркуляции, узлы
      \param[in] turnAngle Угол поворота (до 180 гр.), градусы
      \param[in] turnAngleFull Общий угол поворота с начала циркуляции, градусы
      \param[in] rudderAngle угол перекладки руля, градусы
      */
      virtual iManeuverMath* Circulation(double speed, double speedSetted, double speedTelegraph, double turnAngle, double turnAngleFull, double rudderAngle,
        const ManeuverEnvironment* env = nullptr)const = 0;

      /*!
      Получить математику для движения вперед
      \param[in] speed скорость движения, узлы
      \param[in] dist сколько двигаться, метры
      */
      virtual iManeuverMath* MoveForward(double speed, double dist, const ManeuverEnvironment* env = nullptr)const = 0;

      /*!
      Получить математику для ускорения/замедления
      \param[in] speedFrom начальная скорость движения, узлы
      \param[in] speedTo конечная скорость движения, узлы
      \param[in] maxDist максимальная дистанция на маневр изменения скорости, метры
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
   * Абстрактный интерфейс для доступа к мат. моделям движения кораблей
   */
   struct iMathModelManager : colreg::iReleasable
   {
      //! подобрать модель по информации корабля
      virtual const iVesselMathModel* SelectMathModelByShip(const ship_info& info) const = 0;
      virtual const colreg::iVesselMathModel* GetMathModelByName(const char* modelName) const = 0;
      virtual model_name_ref GetModelList() const = 0;
      virtual void SetCommunicator(ICommunicator* comm) = 0;

      //! получить поддерживаемые модели
      virtual const models_ref* GetModels()const = 0;

      //! вкл/выкл поддержку погоды
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
Создание манеджера моделей
\param[in] path Путь к папке ColregData, содержащий папку models/weatherModels с файлами моделей *.xml
*/
extern "C" MATH_MODEL_EXPORT_IMPORT colreg::iMathModelManager* __cdecl CreateModelManager(const char* path);