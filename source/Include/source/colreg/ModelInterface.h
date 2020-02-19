/*!
\file
\brief Заголовочный файл ModelInterface.h
Определяет интерфейс для работы с моделью
*/
#pragma once
#include "CommonStructs.h"
#include "ModelStructs.h"
#include "CommonRefs.h"
#include <functional>
#include <memory>
#include "WeatherInterface.h"
#include "DebugStructs.h"

namespace colreg
{

   struct iVesselMathModel;
   struct iMathModelManager;
   
#pragma pack (push, 1)

   struct track_model_info_ref
   {
      const track_point_info* track;
      const model_point_info* model_infos;
      size_t size;
   };

   struct iModelTrack : iReleasable
   {
      virtual const track_model_info_ref* Get() const = 0;
   };

   //!тип сегмента
   enum SEGMENT_TYPE : char
   {
      ST_CONST_SPEED    = 1,     //< Движение с постоянной скоростью и курсом
      ST_ACCELERATION   = 1<<1,  //< Движение с постоянной курсом и ускорением/замедлением
      ST_CIRCULATION    = 1<<2,  //< Поворот
   };

   struct segment_info
   {
      size_t route_segment_index = INVALID_INDEX; //< Номер сегмента исходного маршрута
      SEGMENT_TYPE type;                          //< тип сегмента
      MODEL_MOVE_MODE model_mode;                 //< Режим движения модели
      double time;                                //< время на прохрождение сегмента
      double lenght;                              //< длина сегмента
      track_point_info start;                     //< точка входа 
      track_point_info end;                       //< точка выхода
   };

   using model_segments_ref = base_ref<segment_info>;

   //! Тип ошибки создания модели по маршруту
   enum class MODEL_ERROR : char
   {
      ME_NONE = 0,                     //< Ок
      ME_SIZE_ERROR,                   //< меньше двух точек 
      ME_TIME_ERROR,                   //< в точках прогноза кривое время
      ME_CIRCULATION_ERROR,            //< не вписаться в поворот
      ME_REVERSE_ERROR,                //< резко пошли в противоположное направление
   };

   enum class MODEL_WARNING : char
   {
      MW_NONE = 0,
      MW_MIN_RUDDER_ANGLE_OVERFLOW,     //< Радиус поворота соответствует слишком маленькому углу перекладки руля. Заменяем на максимально возможный радиус из модели
      MW_MAX_RUDDER_ANGLE_OVERFLOW,     //< Радиус поворота соответствует слишком большому углу перекладки руля. Заменяем на минимально возможный радиус из модели
      MW_WEATHER_WARNING                //< модель не может пройти из за погоды
   };

   struct model_warning
   {
      MODEL_WARNING warning = MODEL_WARNING::MW_NONE;
      size_t index = INVALID_INDEX;
      model_warning(MODEL_WARNING t, size_t i )
         : warning{ t }
         , index{ i }
      {}
   };

   using model_warnings_ref = base_ref<model_warning>;

   //! информациия о модели по маршруту/прогнозу
   struct model_create_result
   {
      MODEL_ERROR error = MODEL_ERROR::ME_NONE;  //< ошибка
      size_t index = INVALID_INDEX;             //< номер сегмента маршрута/точки траектории прогноза где произошла ошибка
      model_warnings_ref warnings;              //< предупреждения
   };

   enum ROUTE_CHECK_FLAG: int
   {
      RCF_NONE = 0, //фактически аналогично RCF_IN_ROUTE
      RCF_CHECK_DIRECTION = 1,
      RCF_IN_ROUTE = 2, // интересует зона IN_ROUTE, иначе RCR_CHECK_XTE_FAILED
      RCF_CLOSE_TO_ROUTE = 4, //интересует зоны IN_ROUTE+RZT_CLOSE_TO_ROUTE, иначе RCR_CHECK_XTE_FAILED
      RCF_NEAR_TO_ROUTE = 8 , //интересует зоны IN_ROUTE+RZT_CLOSE_TO_ROUTE+NEAR_TO_ROUTE, иначе RCR_CHECK_XTE_FAILED 
      RCF_SKIP_XTE = 16 // нужна только проекция на маршрут, неважно в какой зоне
   };

   
   enum ROUTE_CHECK_ERROR : int
   {
      RCR_SUCCESS = 0,
      RCR_CHECK_DIRECTION_FAILED = 1,
      RCR_CHECK_XTE_FAILED = 2,
      RCR_FAILED = 4
   };

   //! Влияние глубини воды на физику движенияв зависимости от осадки
   enum class SHALLOW_WATER_EFFECT_TYPE : char
   {
      SWT_UNKNOWN = 0,
      WST_DEEP_WATER,     //< нет влияние глубины на физику движения
      WST_HIGHT_WATER,    //< глубокая вода
      WST_MEDIUM_WATER,   //< средняя глубина воды
      WST_SHALLOW_WATER   //< мелководье
   };


   //! получить влияние глубины на движение корабля   
   inline SHALLOW_WATER_EFFECT_TYPE GetShallowWaterEffect(double depth, double draft)
   {
      const auto scale = depth / draft;
      if (scale > 5.)
         return SHALLOW_WATER_EFFECT_TYPE::WST_DEEP_WATER;
      if (scale > 4.)
         return SHALLOW_WATER_EFFECT_TYPE::WST_HIGHT_WATER;
      if (scale > 2)
         return SHALLOW_WATER_EFFECT_TYPE::WST_MEDIUM_WATER;
      
      return SHALLOW_WATER_EFFECT_TYPE::WST_SHALLOW_WATER;
   }
   
   enum class ROUTE_ZONE_TYPE : char //"маршрутные" зоны, по степени удаленности от маршрута. 
   {
      RZT_IN_ROUTE = 0,
      RZT_CLOSE_TO_ROUTE = 1,
      RZT_NEAR_TO_ROUTE = 2,
      RZT_OUT_OF_ROUTE = 3
   };

     
  
   /*!
   \interface iDepthProvider
   Абстрактный интерфейс для получения информации об окружающей среде
   */
   struct iEnvironmentProvider
   {
      virtual double GetDepth(const geo_point& pos)const = 0;
      virtual double GetShallowWaterScale(const geo_point& pos, double draft) const = 0;
      virtual weather_info GetWeatherInfo(const track_point_info& move_info) const = 0;
      virtual forbidden_turn_sector Estimate(const ship_info& info, const track_point_info& move_info) const = 0;
      virtual bool isFog(const geo_point& pos)const = 0;
   };

   /*!
   \interface iModel
   Абстрактный интерфейс для работы с моделью по маршруту/траектории прогнноза
   */
   struct iModel : public colreg::iReleasable
   {
      //! Для работы с моделью model_create_result::type должен быть MW_NONE  
      virtual const model_create_result* GetResult()const = 0;
      
      /*!
      Получить траекторию по маршруту
      \param[in] pos Tекущая позиция корабля для учета смещения
      \param[in] timeStart Cдвиг от начала маршрута, секунды
      \param[in] period Период времени, секунды
      \param[in] timeStep Шаг времени для точек траектории, секунды
      \return Точки траектории с временем относительно начала маршрута
      */
      virtual iModelTrack* GenerateTrajectory(const colreg::track_point_info& pos, double timeStart, int period, int timeStep) const = 0;

      /*!
      *получить точку на маршруте, сдвинутую на time секунд относительно начала маршрута
      \param[in] pos текущая позиция корабля для учета смешения
      \param[in] time сдвиг от начала маршрута, секунды
      */
      virtual bool GetPositionByTime(const colreg::track_point_info& pos, double time, track_point_info& position, model_point_info& modelInfo) const = 0;


      //! Получить проекцию точки position на маршрут
      //! ROUTE_CHECK_ERROR НЕ может быть nullptr
      virtual ROUTE_ZONE_TYPE GetProjectionOnRoute(const track_point_info& position, const model_point_info& modelInfo, ROUTE_CHECK_FLAG checkFlag,
                                             track_point_info& projection, model_point_info& projectionModelInfo,
                                             ROUTE_CHECK_ERROR* pCheckResult ) const = 0;
    
      //! Получить время необходимое на прохождение всего маршрута, в секундах
      virtual double GetRouteTime() const = 0;

      //! Получить дистанцию маршрута, в метрах
      virtual double GetRouteLenght() const = 0;

      //! Получить маршрут, разбитый на сегменты движения
      virtual const model_segments_ref* GetRouteSegments() const = 0;

      //! Получить маршрут по которому была построена модель движения
      virtual const route_ref* GetSourceRoute() const = 0;

      //! получить мат. модель
      virtual const colreg::iVesselMathModel* GetMathModel() const = 0;

      //! откорректировать скорости в мат модели, сам route при этом не поменяется
      virtual bool UpdateTrajectorySpeed(const track_point_info& curPos, double updateSpeedEps, ROUTE_SPEED_UPDATE_STRATEGY strategy ) = 0;

      //! учечть погоду в мат модели, сам route при этом не поменяется
      virtual bool UpdateWeather(const colreg::track_point_info& curPos, const weather_info& weather) = 0;

      //! получить точку, в которой делали update trajectory
      virtual colreg::track_point_info GetRouteUpdateTrajectoryPoint() = 0;

      virtual iModel* Clone() const = 0;

      //! Вернуть ширину указанной зоны, NO_VALUE в случае неудачи
      virtual double GetZoneWidth(size_t routeSegmIndex, bool left, ROUTE_ZONE_TYPE zone) const = 0;

   };

  
   /**
   * \interface iModelManager
   * Абстрактный интерфейс для доступа к моделям движения кораблей по маршруту/трактории прогноза
   */
   struct route_model_config;
   struct iModelManager : colreg::iReleasable
   {
      //! Установить интерфейс для получения информации об окружающей среде
      virtual void SetEnviromentProvider(const iEnvironmentProvider* depthProvider)const = 0;

      /*! Получить модель движения по маршруту
      \param[in]bool allowShipXTD Учитывать текушее отклонения от маршрута при определении траектории и проекции на маршрут
      */
      virtual iModel* ModelByRoute(const route_point* points, size_t size, const ship_info& info, bool allowShipXTD, const route_model_config* routeModelConfig = nullptr, const colreg::iEnvironmentProvider* depthProvider = nullptr) const = 0;

      /*! Получить модель движения по маршруту
      \param[in]bool allowShipXTD Учитывать текушее отклонения от маршрута при определении траектории и проекции на маршрут
      */
      virtual iModel* ModelByRoute(const route_point* points, size_t size, const ship_info& info, const colreg::iVesselMathModel* mathModel, bool allowShipXTD, const route_model_config* routeModelConfig = nullptr, const colreg::iEnvironmentProvider* depthProvider = nullptr) const = 0;

      //! Получить модель движения по траектории прогноза
      virtual iModel* ModelByPrediction(const track_point_info* points, size_t size, const ship_info& info, const route_model_config* routeModelConfig = nullptr) const = 0;

      //! Получить интерфейс для доступа к мат. моделям движения кораблей
      virtual const iMathModelManager* GetMathModelManager() const = 0;

      virtual void SetDebugInfo(dbg::iDebugInfo*)const = 0;

   };


   using ModelUniquePtr = std::unique_ptr< iModel, std::function<void(iModel*)>>;
   inline ModelUniquePtr createModel(iModel* p) { return ModelUniquePtr{ p, interfaceDeleter }; }


#pragma pack (pop)
}


#ifdef COLREGDLL_EXPORTS
#define ROUTE_MODEL_EXPORT_IMPORT __declspec(dllexport) // export DLL information
#else
#define ROUTE_MODEL_EXPORT_IMPORT __declspec(dllimport) // import DLL information
#endif
/*!
Создание манеджера моделей
\param[in] path путь к папке models с файлами моделей *.xml
*/
extern "C" ROUTE_MODEL_EXPORT_IMPORT colreg::iModelManager* __cdecl CreateRouteModelManager(const colreg::iMathModelManager* mathModelManager);


