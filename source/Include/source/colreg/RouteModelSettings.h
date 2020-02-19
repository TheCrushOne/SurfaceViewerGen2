#pragma once

//#include "CommonStructs.h"

namespace colreg
{
#pragma pack(push, 1)
   struct route_model_config
   {
      float in_route_max_course_diff = 45.0; //максимальное допустимое отклонение от курса на маршруте, градусы

      float close_to_route_zone_koef = 1.0; //коеффициент к обычному XTE , определ€ющий размер зоны CLOSE_TO_ROUTE
      float close_to_route_max_course_diff = 30.0; //максимальное допустимое отклонение от курса в зоне close_to_route , градусы

      float near_to_route_zone_koef = 3.0; //коеффициент к обычному XTE , определ€ющий размер зоны NEAR_TO_ROUTE
      float min_zone_width_koef = 10.0; //минимальна€ ширна зоны, коэффициент к ширине корабл€.

      bool use_extended_route_zones = true;   //использовать расширенные зоны дл€ анализа нахождени€ на маршруте
   };
#pragma pack(pop)
};