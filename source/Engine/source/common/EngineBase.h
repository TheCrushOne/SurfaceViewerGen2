#pragma once

#include "common/central_class.h"
#include "common/matrix_object.h"
#include "common\pathfinder_structs.h"
#include "algorithm\pathfinder\PathFinderPipeline.h"

namespace SV::engine
{
   class EngineBase
      : public Central
   {
   public:
      EngineBase(central_pack* pack);
   protected:
      void processPathFindInternal(const pathfinder::path_finder_indata&, std::function<void(void)>);
   protected:
      std::unique_ptr<pathfinder::PathFinderPipeline> m_pathfinder;
      std::shared_ptr<pathfinder::RoutePointMatrix> m_rawdata;
      std::shared_ptr<pathfinder::path_finder_indata> m_indata;
      std::shared_ptr<settings::application_settings> m_settings;
   };
}