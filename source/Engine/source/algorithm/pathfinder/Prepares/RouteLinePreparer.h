#pragma once

#include "common/pathfinder_structs.h"
#include "common/central_class.h"
#include "../PathFinder.h"

namespace SV::pathfinder
{
   struct counted_path_storage
   {
      std::unordered_map<size_t, std::unordered_map<size_t, CG::route_line>> land_path_map;
      std::unordered_map<size_t, std::unordered_map<size_t, CG::route_line>> air_path_map;
   };

   class RouteLinePreparer
      : public Central
   {
   public:
      RouteLinePreparer(central_pack* pack);
      ~RouteLinePreparer();

      void SetRawDataMatrix(const SharedRoutePointMatrix& data) { m_rawdata = data; }
      void SetCoverageMatrix(const SharedUnsignedMatrix& data) { m_coverage = data; }
      void PrepareTaskList(std::vector<SV::settings::route> routeList, bool isAir);
      void RestorePathList(std::vector<SV::settings::route> routeList, bool isAir);
      std::vector<path_finder_task>& GetCurrentTaskList() { return m_currentTaskList; }
   protected:
      void restoreAirPath(size_t idx, SV::settings::route&);
      void restoreLandPath(size_t idx, SV::settings::route&);

      void prepareAirPath(size_t idx, SV::settings::route&);
      void prepareLandPath(size_t idx, SV::settings::route&);
   private:
      std::vector<path_finder_task> m_currentTaskList;
      SharedRoutePointMatrix m_rawdata;
      SharedUnsignedMatrix m_coverage;
      std::unique_ptr<PathFinder> m_pathfinder;
      counted_path_storage m_pathStorage;
   };
}