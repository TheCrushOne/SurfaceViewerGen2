#pragma once

namespace colreg
{
   //! ����� �������� ������
   enum class MODEL_MOVE_MODE : int
   {
      MMM_FORWARD = 0,        ///< �� ������ � ���������� ���������
      MMM_CIRCULATION_LEFT,   ///< ���������� �����
      MMM_CIRCULATION_RIGHT,  ///< ���������� ������
      MMM_ACCSELERATION_UP,   ///< ���������
      MMM_ACCSELERATION_DOWN, ///< ����������
   };

#pragma pack (push, 1)

   struct model_point_info
   {
      size_t route_segment_index = 0;                  ///< ����� �������� ��������, �������� ����������� ����� �����
      MODEL_MOVE_MODE model_mode = MODEL_MOVE_MODE::MMM_FORWARD;   ///< ����� �������� ������
   };

   //! ���������� � ������� �������
   struct maneuver_route_info
   {
      double dist_to = 0.;                            //< ���������� �� ���������� ������� �������, ����
      double direction = 0.;                          //< ����������� ������� ������� �������, �������
      double speed = 0.;                              //< �������� ������� ������� �������, ����
      int    time_to = 0;                             //< ����� �� �������, �������
      MODEL_MOVE_MODE maneuver_type;                  //< ��� �������
      double next_direction = 0.;                     //< ����������� ���������� ������� �������, �������
      double next_speed = 0.;                         //< �������� �� ��������� ������� �������, ����
   };

#pragma pack (pop)
} //namespace colreg