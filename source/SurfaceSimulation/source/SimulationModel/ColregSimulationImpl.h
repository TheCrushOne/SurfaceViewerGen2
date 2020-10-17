#pragma once
#include "colreg/ColregSimulation.h"
#include "colreg/ColregContainers.h"
#include "SVCG/base_structs.h"

namespace ColregSimulation
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

   class SimulationUnit : public iUnit
   {
   public:
      SimulationUnit()
      {
         createDomain(.5);
      }

      SVCG::ship_info GetInfo() const override final { return m_info; }
      //const colreg::domain_scales& GetDomainScales() const override final { return m_domainScales; }
      track_point_full_info GetPos() const override final { return m_posInfo; }
      const SVCG::trajectory_point_vct& GetRoute(ROUTE_TYPE type) const override final;
      const SVCG::trajectory_point_vct& GetSrcPath() const override final { return m_srcPath; }
      const SVCG::trajectory_point_vct& GetSrcControlPoints() const override final { return m_controlPoints; }

      void createDomain(double);

      //const char* GetETA() const override final { return m_eta.c_str(); /*ETA formatted as: MMDDHHMM, (Month, day, hour, minute). Example: 02.06.1982 19:40 encoded as 06021940*/ }

      void SetSrcRoute(SVCG::trajectory_point_vct& route) { m_srcPath = route; }
      void SetSrcContolPoints(SVCG::trajectory_point_vct& route) { m_controlPoints = route; }
      void SetInfo(const SVCG::ship_info& info) { m_info = info; }
      void SetPosInfo(const track_point_full_info& info) { m_posInfo = info; }
   private:
      SVCG::ship_info m_info;
      //colreg::domain_scales m_domainScales;
      //colreg::domain_geometry_ref m_domainRef;
      //std::vector<std::vector<colreg::polar>> m_domainPts;
      //std::vector<colreg::domain_geometry> m_domain;
      SVCG::trajectory_point_vct m_srcPath;
      SVCG::trajectory_point_vct m_controlPoints;
      //ShipPathHolder m_simulationPath; // Как двигается корабль реально
      //ShipPathHolder m_predictionPath; // предикшн чисто для информационных целей

      track_point_full_info m_posInfo;

      std::string m_eta;
   };

   class SimulationShip : public iShip, public SimulationUnit
   {
   public:
      SimulationShip() {}
   };

   class SimulationRover : public iRover, public SimulationUnit
   {
   public:
      SimulationRover() {}
   };

   class SimulationDrone : public iDrone, public SimulationUnit
   {
   public:
      SimulationDrone() {}
   };
}