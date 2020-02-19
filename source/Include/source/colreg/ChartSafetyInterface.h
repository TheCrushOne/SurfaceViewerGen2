/*!
\file
\brief Заголовочный файл ChartSafetyInterface.h
Определяет интерфейсы и структуры данных для проверки безопасности маршрута на карте
*/
#pragma once

#include "CommonStructs.h"
#include "ChartSafetyStructs.h"
#include "CommonRefs.h"

interface ICommunicator;

namespace colreg
{
#pragma pack (push, 1)

   //! Результат проверки маршрутов на безопасность
   struct iSafetyRouteResult : public iReleasable
   {
      virtual const base_ref<safety_route_result>* Get() const = 0;
      virtual const base_ref<chart_object_id>* GetDeadZoneDangers() const = 0;
      virtual const base_ref< trajectory_safe_distances_ref >* GetTrajectoriesSafeDistances() const = 0;
   };

   using iChartObjects = iBaseRefReleasable<chart_object>;
   using ignoreObjects = base_ref<chart_object_id>;
   using tracksToEstimate = base_ref<geo_points_ex_ref>;
   using trackToAnalyze = base_ref<geo_point_ex>;

   //! Результат оценки на безопасность траекторий на карте
   using iEstimationResult = iBaseRefReleasable<iSafetyRouteResult*>;

   //! Результат анализа траекторий на карте
   using iAnalyzeResult = iBaseRefReleasable<base_ref<chart_context>>;

   //! Запрос на оценку по безопасности на карте
   struct estimation_query
   {
      tracksToEstimate tracks;               //! траектории движения для оценки на безопасность по карте
      const ship_info* shipInfo;             //! инфа по кораблю
      size_t deadZone = 0;                   //! мертвая зона 
      bool collectDeadZoneObjects = false;   //! возвращать объекты в мертвой зоне
      ignoreObjects ignoreObjectsIds;        //! Игнорируемые объекты (в мертвой зоне)
      double minSafetyDistance = 0.0;        //! Мин. дистанция для вычисления минимальной дистанции для сегментов, метры
   };

   struct analyze_query
   {
      trackToAnalyze points;               //! траектории движения для анализа на карте
      const ship_info* shipInfo;           //! инфа по кораблю
      double safeRadius = 0.;
      bool analyzeNarrowness = false;
   };

   using estimations_query_ref = base_ref<estimation_query>;
   using analyze_query_ref = base_ref<analyze_query>;

#pragma pack (pop)
   //! Интерфейс для проверки безопасности маршрута
   struct iChartSafetyCheck : iReleasable
   {
      virtual void __cdecl SetCommunicator(ICommunicator* comm) = 0;
      virtual bool __cdecl Init(LPCWSTR dataSetPath) = 0;
      virtual void __cdecl Reset() = 0;

      /*!
      Проверить траектории на безопасность
      \param[in] analysisObjsMask Маска объектов карты (OBJECT_TYPE), которые будут участвовать в анализе
      */
      virtual iEstimationResult* __cdecl EstimateChartObjectsDanger(const estimations_query_ref& queries, colreg::mask_type analysisObjsMask = OBJECT_TYPE::OT_SURFACE_OBJS |
                                                                                                          OBJECT_TYPE::OT_UNDERWATER_OBJS |
                                                                                                          OBJECT_TYPE::OT_TSS |
                                                                                                          OBJECT_TYPE::OT_DYNAMIC_AREAS ) const = 0;

      /*!
      Проанализировать траекторию движения по карте
      \param[in] analysisObjsMask Маска объектов карты (OBJECT_TYPE), которые будут участвовать в анализе
      */
      virtual iAnalyzeResult* __cdecl AnalizeTrajectory(const analyze_query_ref& queries,  colreg::mask_type analysisObjsMask = OBJECT_TYPE::OT_SURFACE_OBJS |
                                                                                            OBJECT_TYPE::OT_UNDERWATER_OBJS |
                                                                                            OBJECT_TYPE::OT_TSS |
                                                                                            OBJECT_TYPE::OT_DYNAMIC_AREAS | 
                                                                                            OBJECT_TYPE::OT_CHART_PASSING_STATISTIC_AREA ) const = 0;

      //! Получить статические объекты карты по маске (OBJECT_TYPE). Динамические объекты получатются отдельно
      virtual iChartObjects* __cdecl GetChartObjects(colreg::mask_type mask = OBJECT_TYPE::OT_ALL) const = 0;

      //! Получить объекты карты по id
      virtual iChartObjects* __cdecl GetChartObjects(const colreg::chart_object_id* ids, size_t size ) const = 0;


      /*!
      Добавить зону ограничения движения
      \param[in] points Точки замкнутого полигона
      \param[in] size Количество точек полигона
      \param[in] info Описание зоны ограничения движения
      \param[in] id Опционально - указать ID существующего объекта для его изменения (если он существует)
      \return Количество зон ограничения движения
      */
      virtual chart_object_id __cdecl AddNoGoArea(const geo_point* points, size_t size, const no_go_area_info& info, const chart_object_id* id = nullptr) = 0;

      /*!
      Добавить зону с установленными коэффициентами сжатия коридора домена безопасности
      \param[in] points Точки замкнутого полигона
      \param[in] size Количество точек полигона
      \param[in] info Описание зоны
      \param[in] id Опционально - указать ID существующего объекта для его изменения (если он существует)
      \return ID объекта
      */
      virtual chart_object_id __cdecl AddDomainArea(const geo_point* points, size_t size, const domain_area_info& info, const chart_object_id* id = nullptr) = 0;

      virtual bool __cdecl RemoveDynamicArea(chart_object_id id) = 0;

      //! Удалить все динамические объекты карты
      virtual void __cdecl ClearDynamicAreas() = 0;

      virtual void __cdecl SetCallbackInterface(iChartSafetyCheck* callbacks) const = 0;

      virtual void __cdecl PrintStatistic() const {}

      //используется для картографического анализа траффика вT rafficAnalyzer
      //возвращает глубину. для объектов суши - 0.0, в случае ошибки - NO_VALUE
      virtual double __cdecl GetDepthAtPoint(const geo_point& pointLocation) const = 0;

      //Получить безопасность сегмента траектории. используется для анализа движения в TrafficAnalyzer
      virtual segment_check_result __cdecl CheckTrajectorySegmentSafety(const geo_point& ptFrom, const geo_point& ptTo, double xteLeft, double xteRight,
         const ship_info& shipInfo, colreg::mask_type analysisObjsMask = OBJECT_TYPE::OT_SURFACE_OBJS |
                                                                         OBJECT_TYPE::OT_UNDERWATER_OBJS |
                                                                         OBJECT_TYPE::OT_TSS |
                                                                         OBJECT_TYPE::OT_DYNAMIC_AREAS) const = 0;
      virtual size_t GetObjectsUSN() const = 0; //Вернуть USN - для проверки состояния 
   };

#ifdef CHART_SAFETY_CHECKER_EXPORTS
#define CHART_SAFETY_CHECKER_EXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHART_SAFETY_CHECKER_EXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

   extern "C" CHART_SAFETY_CHECKER_EXPRTIMPRT iChartSafetyCheck* __cdecl CreateChartSafetyChecker();
}