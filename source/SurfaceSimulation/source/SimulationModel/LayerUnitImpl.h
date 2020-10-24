#pragma once
#include "simulation/LayerUnitInterface.h"
#include "layer_provider\trajectory_point.h"
#include "colreg/SimulationTypes.h"

namespace SV::surface_simulation
{
   //class ShipPathHolder : public ship_path_ref
   //{
   //public:
   //   void SetRoute(ColregRoutePoints&& r)
   //   {
   //      _routePtr = std::make_unique<RouteRefHolder>(std::move(r));
   //      route = _routePtr.get();
   //   }

   //   void SetTrack(ColregTrackPoints&& tr, ColregModelInfo&& modelInfo/*, ColregChartContext&& chartContext*/, ColregDomainBorder&& domainBorder)
   //   {
   //      _trackPtr = std::make_unique<TrackFullInfoRefHolder>(std::move(tr), std::move(modelInfo)/*, std::move(chartContext)*/, std::move(domainBorder));
   //      track = _trackPtr.get();
   //   }

   //private:
   //   std::unique_ptr<RouteRefHolder> _routePtr;
   //   std::unique_ptr<TrackFullInfoRefHolder> _trackPtr;
   //};
   constexpr double DefaultUnitSpeed = 10.;

   class LayerUnitImpl : public iLayerUnit
   {
   public:
      LayerUnitImpl()
      {
         createDomain(.5);
      }

      layer_provider::ship_info GetInfo() const override final { return m_info; }
      //const colreg::domain_scales& GetDomainScales() const override final { return m_domainScales; }
      const CG::layer_provider::trajectory_point& GetPos() const override final { return m_srcRoute.at(m_currentPositionIdx); }
      const CG::layer_provider::trajectory_point_vct& GetRoute(ROUTE_TYPE type) const override final;
      const CG::layer_provider::trajectory_point_vct& GetSrcPath() const override final { return m_srcRoute; }
      const CG::layer_provider::trajectory_point_vct& GetSrcControlPoints() const override final { return m_controlPoints; }
      size_t GetPosCount() const override final { return m_srcRoute.size(); }

      //const char* GetETA() const override final { return m_eta.c_str(); /*ETA formatted as: MMDDHHMM, (Month, day, hour, minute). Example: 02.06.1982 19:40 encoded as 06021940*/ }
      void SetSrcRoute(const settings::route& src);
      void SetSrcContolPoints(CG::layer_provider::trajectory_point_vct& route) { m_controlPoints = route; }
      void SetInfo(const layer_provider::ship_info& info) { m_info = info; }
      void SetCurrentPositionIdx(size_t idx) { ATLASSERT(idx < m_srcRoute.size()); m_currentPositionIdx = idx; }
      //void SetPosInfo(const CG::layer_provider::trajectory_point& info) { m_posInfo = info; }
   protected:
      void createDomain(double);
   private:
      layer_provider::ship_info m_info;
      //colreg::domain_scales m_domainScales;
      //colreg::domain_geometry_ref m_domainRef;
      //std::vector<std::vector<colreg::polar>> m_domainPts;
      //std::vector<colreg::domain_geometry> m_domain;
      CG::layer_provider::trajectory_point_vct m_srcRoute;
      CG::layer_provider::trajectory_point_vct m_controlPoints;
      //ShipPathHolder m_simulationPath; // Как двигается корабль реально
      //ShipPathHolder m_predictionPath; // предикшн чисто для информационных целей
      size_t m_currentPositionIdx;
      //CG::layer_provider::trajectory_point m_posInfo;

      //std::string m_eta;
   };

   class LayerShipImpl : public iLayerShip, public LayerUnitImpl
   {
   public:
      LayerShipImpl() {}
   };

   class LayerRoverImpl : public iLayerRover, public LayerUnitImpl
   {
   public:
      LayerRoverImpl() {}
   };

   class LayerDroneImpl : public iLayerDrone, public LayerUnitImpl
   {
   public:
      LayerDroneImpl() {}
   };
}