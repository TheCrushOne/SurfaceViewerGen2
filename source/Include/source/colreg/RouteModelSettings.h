#pragma once

//#include "CommonStructs.h"

namespace colreg
{
#pragma pack(push, 1)
   struct route_model_config
   {
      float in_route_max_course_diff = 45.0; //������������ ���������� ���������� �� ����� �� ��������, �������

      float close_to_route_zone_koef = 1.0; //����������� � �������� XTE , ������������ ������ ���� CLOSE_TO_ROUTE
      float close_to_route_max_course_diff = 30.0; //������������ ���������� ���������� �� ����� � ���� close_to_route , �������

      float near_to_route_zone_koef = 3.0; //����������� � �������� XTE , ������������ ������ ���� NEAR_TO_ROUTE
      float min_zone_width_koef = 10.0; //����������� ����� ����, ����������� � ������ �������.

      bool use_extended_route_zones = true;   //������������ ����������� ���� ��� ������� ���������� �� ��������
   };
#pragma pack(pop)
};