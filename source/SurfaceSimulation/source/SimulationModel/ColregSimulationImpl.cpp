#include "stdafx.h"
#include "ColregSimulationImpl.h"

using namespace ColregSimulation;

const ship_path_ref* SimulationUnit::GetRoute(ROUTE_TYPE type) const
{
   switch (type)
   {
   //case ROUTE_TYPE::RT_COLREG:   return GetModelPath();
   case ROUTE_TYPE::RT_SIMULAION:return GetSimulationPath();
   case ROUTE_TYPE::RT_SOURSE:   return GetSrcPath();
   }

   return nullptr;
}