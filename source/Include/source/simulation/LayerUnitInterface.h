#pragma once

#include "LayerEntityInterface.h"
#include "layer_provider/unit_info.h"
#include "layer_provider/trajectory_point.h"
#include "colreg/SimulationTypes.h"

namespace SV::surface_simulation
{
   struct iLayerUnit : iLayerEntity
   {
      //! Информационные данные юнита
      virtual layer_provider::ship_info GetInfo() const = 0;

      //! Количество точек траектории
      virtual size_t GetPosCount() const = 0;

      //! Полная информация по текущему местоположению
      virtual const CG::layer_provider::trajectory_point& GetPos() const = 0;

      //! Псевдосписок путей
      virtual const CG::layer_provider::trajectory_point_vct& GetRoute(ROUTE_TYPE type) const = 0;

      //! Исходный путь
      virtual const CG::layer_provider::trajectory_point_vct& GetSrcPath() const = 0;

      //! Исходный набор кт
      virtual const CG::layer_provider::trajectory_point_vct& GetSrcControlPoints() const = 0;

      /*! Получить топологию домена юнита в момент времени
      \param[in] scales Получить топологию с переданными коэффициентами сжатия
      */
      //virtual const colreg::domain_geometry_ref* GetDomainTopology(double time, const colreg::domain_scales* scales = nullptr) const = 0;

      virtual ~iLayerUnit() = default;
   };

   //! Интерфейс доступа к данным ровера
   struct iLayerRover
   {};

   //! Интерфейс доступа к данным дрона
   struct iLayerDrone
   {};

   //! Интерфейс доступа к данным корабля
   struct iLayerShip
   {
      //virtual const simulation_ship_settings& GetSimulationSettings() const = 0;

      //! Путь из модели colreg
      //virtual const ship_path_ref* GetModelPath() const = 0;

      //! Получить проекцию текущей позиции на маршрут
      //virtual colreg::track_point_info GetProjectionToRoute() const = 0;

      //virtual const colreg::iVesselMathModel* GetMathModel()const = 0;
      virtual bool IsRouteExtended() const { return false; }

      //для AIS Log должно возвращаться true
      //virtual bool IsReadOnly() const = 0; 

      //virtual const char* GetDestination() const = 0;
      //virtual const char* GetETA() const = 0;

      //virtual double GetColregRouteZoneWidth(unsigned int routeIndex, bool left, colreg::ROUTE_ZONE_TYPE zone) const = 0;
      //virtual const colreg::iModel* GetSourceRouteModel() const = 0;
      virtual ~iLayerShip() = default;
   };
}