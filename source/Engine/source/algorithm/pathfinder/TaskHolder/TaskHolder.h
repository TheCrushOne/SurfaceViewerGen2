#pragma once
#include <functional>
#include "common/pathfinder_structs.h"

namespace pathfinder
{
   struct task_unit
   {
      std::function<route(route&, const std::shared_ptr<Matrix<SVCG::route_point>>, size_t, bool)> runnable;

   };

   class TaskHolder
   {

   };
}