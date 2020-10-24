#pragma once

namespace SV::surface_simulation
{
#pragma pack (push, 4)
   //!тип маршрута
   enum class ROUTE_TYPE : char
   {
      RT_COLREG = 0,    //< Для передачи в COLREG/pathfinder
      RT_SIMULAION,     //< Для симуляции движения
      RT_SOURSE,        //< Исходный маршрут движения из сценария
      RT_CONTROL,       //< Маршрут по контрольным точкам
      RT_DISIGION,      //< Маршрут-решение
      RT_SUB_OPTIMAL,   //< Маршрут-решение
   };

   enum class UNIT_TYPE : char
   {
      UT_ROVER = 0,
      UT_DRONE,
      UT_SHIP,
   };

   enum class SCENARIO_STATUS : char
   {
      SS_NOT_LOADED = 0,
      SS_MAP_CHECKOPENED,
      SS_MAP_PROCESSED,
      SS_MAPOBJ_PROCESSED,
      SS_PATHS_COUNTED,
      SS_OPT_PATHS_COUNTED,
   };

   enum class SIMULATION_STATUS : char
   {
      SS_RUN = 0,
      SS_PAUSE,
      SS_STOP,
   };

   enum class SIMULATION_PLAYER_TYPE : char
   {
      SPT_SCENARIO = 0,
      SPT_LOG,
      SPT_SIZE
   };
#pragma pack(pop)
}