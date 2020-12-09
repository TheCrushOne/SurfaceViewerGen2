#pragma once

#include "colreg/ModuleGuard.h"
#include "common/header_collector.h"
#include "common\communicator.h"
#include "common\pathfinder_structs.h"
#include "crossdllinterface\PathfindingEngineInterface.h"
//#include "base/instance.h"
#include "common/central_class.h"
#include "settings/settings.h"
#include <vector>
#include "common/EngineBase.h"

//#define CURTIME_MS() QDateTime::currentDateTime().toMSecsSinceEpoch()

//template<class T>
//struct GlobalExperimentStatistic;

namespace SV::engine
{
   class PathfindingEngine
      : public iPathfindingEngine
      , public EngineBase
   {
   public:
      PathfindingEngine(central_pack* pack);

      void ProcessPathFind(const pathfinder::path_finder_indata&, const pathfinder::GeoMatrix&, std::shared_ptr<settings::application_settings>, std::function<void(void)>) override final;
      const pathfinder::route_data& GetLastProcessedPaths() const override final { return m_pathfinder->GetPaths(); }
      const pathfinder::UnsignedMatrix& GetLandUnitExplication() const override final;
      const pathfinder::UnsignedMatrix& GetAirUnitExplication() const override final;
      const std::vector<pathfinder::SharedUnsignedMatrix>& GetCoverageHistory() const override final { return m_pathfinder->GetCoverageHistory(); }
      void Release() override final { delete this; }
   private:
      void processPathFind(const pathfinder::path_finder_indata&, const pathfinder::GeoMatrix&, std::function<void(void)>);

      pathfinder::check_fly_zone_result checkFlyZone(const pathfinder::GeoMatrix&, int, int);
      void convertMap(const pathfinder::GeoMatrix&, std::shared_ptr<pathfinder::RoutePointMatrix>);
      pathfinder::check_go_zone_result checkGoZone(const pathfinder::GeoMatrix&, int, int);
      pathfinder::check_go_zone_result checkAngles(double, double, double, double, double, double, double, double, double);
      //SurfaceElement recountElement(const std::shared_ptr<QHeightMapSurfaceDataProxy>, int, int, int, int);
   //signals:
   protected:
      /*void converted() {  }
      void percent(int prcnt) {  }
      void drop() {  }*/
   private:
      // NOTE: экспликации для разных типов роботов внутри содержатся
      std::shared_ptr<pathfinder::route_data> m_routedata;
      //std::shared_ptr<pathfinder::route_data> m_routedata;
   };
}