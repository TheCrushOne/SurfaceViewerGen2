#pragma once
#include <sstream>
#include <iomanip>
#include <string>
#include "ColregInterfaces.h"

namespace colreg
{
   inline const char* shallow_water_effect_to_string(SHALLOW_WATER_EFFECT_TYPE shallow)
   {
      switch (shallow)
      {
      case colreg::SHALLOW_WATER_EFFECT_TYPE::SWT_UNKNOWN:       return "";
      case colreg::SHALLOW_WATER_EFFECT_TYPE::WST_DEEP_WATER:    return "deep water";
      case colreg::SHALLOW_WATER_EFFECT_TYPE::WST_HIGHT_WATER:   return "high water";
      case colreg::SHALLOW_WATER_EFFECT_TYPE::WST_MEDIUM_WATER:  return "medium water";
      case colreg::SHALLOW_WATER_EFFECT_TYPE::WST_SHALLOW_WATER: return "shallow water";
      }
      return "";
   }


   inline const char* move_mode_to_string(MODEL_MOVE_MODE mode)
   {
      switch (mode)
      {
      case colreg::MODEL_MOVE_MODE::MMM_FORWARD:            return "move forward";
      case colreg::MODEL_MOVE_MODE::MMM_CIRCULATION_LEFT:   return "turn left";
      case colreg::MODEL_MOVE_MODE::MMM_CIRCULATION_RIGHT:  return "turn right";
      case colreg::MODEL_MOVE_MODE::MMM_ACCSELERATION_UP:   return "acceleration up";
      case colreg::MODEL_MOVE_MODE::MMM_ACCSELERATION_DOWN: return "acceleration down";
      }
      return "";
   }

   inline bool is_circulation(MODEL_MOVE_MODE mode) { return mode == MODEL_MOVE_MODE::MMM_CIRCULATION_LEFT || mode == MODEL_MOVE_MODE::MMM_CIRCULATION_RIGHT; }
   inline bool is_acceleration(MODEL_MOVE_MODE mode) { return mode == MODEL_MOVE_MODE::MMM_ACCSELERATION_UP || mode == MODEL_MOVE_MODE::MMM_ACCSELERATION_DOWN; }

   inline std::string seconds_to_string(int time_in_sec)
   {
      std::stringstream s;
      const int hour = time_in_sec / 3600;
      const int min = (time_in_sec % 3600) / 60;
      const int sec = (time_in_sec % 3600) % 60;
      
      if(hour)    s << hour << " h ";
      if (min)    s << min << " min ";
      if (sec)    s << sec << " sec";
      return s.str();
   }

   inline std::string maneuver_route_info_to_string(const maneuver_route_info& action)
   {
      std::stringstream s;
      s.setf(std::ios::fixed);
      s << "Go course " << std::setprecision(1) << action.direction << "° with " << std::setprecision(1) << action.speed << " kts " << std::setprecision(2) << action.dist_to << "Nm (" << seconds_to_string(action.time_to) << "), after " << move_mode_to_string(action.maneuver_type);
      if (is_circulation(action.maneuver_type))
      {
         s << " on course " << std::setprecision(1) << action.next_direction << "°";
      }
      else if (is_acceleration(action.maneuver_type))
         s << " to " << std::setprecision(1) << action.next_speed << " kts";
     
      return s.str();
   }

   inline std::string maneuver_actions_to_string(const maneuver_route_info_ref* actions)
   {
      std::stringstream s;
      if (actions)
      {
         for (const auto& action : *actions)
            s << maneuver_route_info_to_string(action) << "\r\n";
      }
      return s.str();
   }

   inline std::string route_to_string(const route_ref* route)
   {
      std::stringstream s;
      ATLASSERT(route && route->size > 1);
      size_t i{ 0 };
      for(; i < route->size - 1; ++i)
      {
         const auto&p = route->arr[i];
         s << i << " : " << p.pos.lat << " " << p.pos.lon << " speed = " << p.speed << " xtd left = " << p.left_XTE << " xtd right = " << p.right_XTE << " radius = " << p.radius
            << " direction = " << math::direction(p.pos, route->arr[i + 1].pos)
            << " distance = " <<  math::distance(p.pos, route->arr[i + 1].pos) << "\r\n";
      }
      const auto&p = route->back();
      s << i << " : " << p.pos.lat << " " << p.pos.lon << " speed = " << p.speed << " xtd left = " << p.left_XTE << " xtd right = " << p.right_XTE << " radius = " << p.radius << "\r\n";
      return s.str();
   }
}