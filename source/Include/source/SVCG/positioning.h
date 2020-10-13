#pragma once

#include "route_point.h"
#include "position_point.h"
#include "common/header_collector.h"
#include "math/point.h"

// TODO: пофиксить преобразования координат
namespace SVCG
{
   /*inline route_point PositionPointToRoutePoint(const position_point& pPoint, const settings::environment_settings settings = settings::environment_settings())
   {
      return route_point{};
   }*/

   inline math::point PositionPointToMathPoint(const position_point& pPoint, const settings::environment_settings settings = settings::environment_settings())
   {
      auto& info = settings.mtx_info;
      auto ord_spos = info.scale*(pPoint.lat * sin(info.angle) - pPoint.lon * cos(info.angle));
      auto abs_spos = info.scale*(pPoint.lon * cos(info.angle) - pPoint.lat * sin(info.angle));
      return math::point(info.ordinate_bias + ord_spos, info.abscissa_bias + abs_spos);
   }

   inline position_point MathPointToPositionPoint(const math::point& pPoint, const settings::environment_settings settings = settings::environment_settings())
   {
      auto& info = settings.gcs_info;
      auto ord_spos = info.scale * (pPoint.lon * cos(info.angle) - pPoint.lat * sin(info.angle));
      auto abs_spos = info.scale * (pPoint.lon * sin(info.angle) + pPoint.lat * cos(info.angle));
      return position_point(- info.ordinate_bias + ord_spos, - info.abscissa_bias + abs_spos);
   }

   inline math::point RoutePointToMathPoint(const route_point& pPoint, const settings::environment_settings settings = settings::environment_settings())
   {
      auto& info = settings.mtx_info;
      auto ord_spos = info.scale * (static_cast<double>(pPoint.row) * cos(info.angle) - static_cast<double>(pPoint.col) * sin(info.angle));
      auto abs_spos = info.scale * (static_cast<double>(pPoint.row) * sin(info.angle) + static_cast<double>(pPoint.col) * cos(info.angle));
      return math::point(-info.ordinate_bias + ord_spos, -info.abscissa_bias + abs_spos);
   }

   inline position_point RoutePointToPositionPoint(const route_point& pPoint, const settings::environment_settings settings = settings::environment_settings())
   {
      return MathPointToPositionPoint(RoutePointToMathPoint(pPoint, settings), settings);
   }
}