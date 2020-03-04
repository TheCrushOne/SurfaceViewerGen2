#pragma once
#include "colreg/ColregSimulation.h"
//#include "colreg/ColregContainers.h"

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

   //   void SetTrack(ColregTrackPoints&& tr, ColregModelInfo&& modelInfo, ColregChartContext&& chartContext, ColregDomainBorder&& domainBorder)
   //   {
   //      _trackPtr = std::make_unique<TrackFullInfoRefHolder>(std::move(tr), std::move(modelInfo), std::move(chartContext), std::move(domainBorder));
   //      track = _trackPtr.get();
   //   }

   //private:
   //   std::unique_ptr<RouteRefHolder> _routePtr;
   //   std::unique_ptr<TrackFullInfoRefHolder> _trackPtr;
   //};

   class SimulationUnit : public iUnit
   {
   public:
      SimulationUnit() {}

      colreg::ship_info GetInfo() const override final { return m_info; }
      const colreg::domain_scales& GetDomainScales() const override final { return m_domainScales; }
      track_point_full_info GetPos() const override final { return m_posInfo; }
      const ship_path_ref* GetRoute(ROUTE_TYPE type) const override final;
      const ship_path_ref* GetSrcPath() const override final { return nullptr/*&m_srcPath*/; }
      const ship_path_ref* GetSimulationPath() const override final { return nullptr/*&m_simulationPath*/; }
      const ship_path_ref* GetPredictionPath() const override final { return nullptr/*&m_predictionPath*/; }
      const colreg::domain_geometry_ref* GetDomainTopology(double time, const colreg::domain_scales* scales = nullptr) const override final { return nullptr; }

      const char* GetETA() const override final { return m_eta.c_str(); /*ETA formatted as: MMDDHHMM, (Month, day, hour, minute). Example: 02.06.1982 19:40 encoded as 06021940*/ }


      void SetPosInfo(const track_point_full_info& info) { m_posInfo = info; }
   private:
      colreg::ship_info m_info;
      colreg::domain_scales m_domainScales;

      //ShipPathHolder m_srcPath;
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