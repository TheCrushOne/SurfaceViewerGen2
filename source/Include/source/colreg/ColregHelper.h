#pragma once
#include <sstream>
#include <iomanip>
#include <string>
#include "ColregInterfaces.h"

namespace colreg
{
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