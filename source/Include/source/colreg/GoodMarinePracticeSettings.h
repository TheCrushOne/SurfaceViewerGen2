#pragma once

#pragma pack(push, 1)


namespace colreg
{
/*
recomended weight
   enum class PenaltyCategorySeverety //categories and weights
   {
      PCS_LOWEST = 1,
      PCS_LOW = 5,
      PCS_BELOW_NORMAL = 15,
      PCS_NORMAL = 20,
      PCS_ABOVE_NORMAL = 25,
      PCS_HIGH = 40,
   };
   */

   //���� ������� ��� ����������� ���������� �������
   struct gmp_heading_courses_maneuver_cfg
   {           //                                                                                                           ____
      double badHeadingMeneuver = 50.0;   // �������� ������������� ��� GMP. ������������ ������ �� �������� �������������  __/    \_
      double secondTurnPenalty = 40.0;    // ��� ������ - ������ ������� � ����� P2 (���� ������ ���� � 4-�� ��������)
      double thirdTurnPenalty = 40.0;     // ��� ������ - ������ ������� � ����� P3 (���� ������ ���� � 3-�� ��������, � ����� ����� �� ������)
      double offsetPenalty = 40.0;        // ��� ������ �� ������� ������� ��������� ����������� �� ����� �������� ��������
      double maxParallelAngleDelta = 3.0; // ���� ���������� ������� ������ �� ����� � ������� ��������� �������. � ������ ��� ������ ���� ����������� 
   };

   //���� ������� ��� �������� ������
   struct gmp_crossing_right_cfg
   {           
      double minPenalty = 10.0;
      double turnToSternPenalty = 40.0;  //��� ������ �� ������� ������ ��� �� ����� ������� �������
      double leaveBehindPenalty = 40.0;  //��� ������ �� �� ���������� ������� �� ������ (������� ������� ������ ��� �� ����� �� ������)
   };

   /*
   ��������� ��� ������� �������� ����� ������� ������� ��������
   */
   struct good_marine_practice_settings
   {
      bool checkTargetAlarm;           //< ��� ����������� ���������, ��� ������ �� "������" ��������� �������
      bool checkCourseIntersection;    //< ��� ����������� ���������, ��� ������ �� �������� � �������� ����������� ������ ������ ��������
      double safeCouseIntersection;    //< CPA *  safeCouseIntersaction - ���������� ��������� ����������� ����� ������� �������
      double maxPenalty;               //< ������������ ����� ��� �������� ������� ������� �������� (GMP)
      gmp_heading_courses_maneuver_cfg headingCoursesPenalties; //< ��������� ������� ��� GMP ��������� ������
      gmp_crossing_right_cfg           crossingRightPenalties;  //< ��������� ������� ��� �������� �������
      good_marine_practice_settings()
         : checkTargetAlarm{ true }
         , checkCourseIntersection{ true }
         , safeCouseIntersection{ 2. }
         , maxPenalty{100.0}
      {}
   };
}
#pragma pack(pop)