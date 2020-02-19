#pragma once
#include "CommonStructs.h"
#include "CommonRefs.h"
#include <vector>

namespace colreg
{
#pragma pack (push, 4)

   //! ������ �����, ������������� ����� �������, ����������� ��� ��������(0-359). ����������� �� ������� �������
   struct forbidden_turn_sector
   {
      forbidden_turn_sector() = default;
      forbidden_turn_sector(double af, double at) 
         : angleFrom{ af }
         , angleTo{ at } 
      {}
      double angleFrom = NO_VALUE;  //< ��������� ���� �������, �������
      double angleTo = NO_VALUE;    //< �������� ���� �������, �������
   };

   //! ���������� � ������
   struct weather_info
   {
      double windDirection = 0.;     // ����������� ����� (����������� ������ ����) [�������]
      double windSpeed = 0.;         // �������� ����� [�/�]
      double currentDirection = 0.;  // ����������� ������� (����������� ���� �����) [�������]
      double currentSpeed = 0.;      // �������� ������� [�/�]
      double waveHeight = 0.;        // ������ �������� ����� - wind wave significant 1/3 height [�]
      double waveDirection = 0.;     // ����������� �������� ����� (����������� ������ ����) [�������]
      double swellLength = 0.;       // ����� ���� [�]
      double swellHeight = 0.;       // ������ ���� [�]
      double swellDirection = 0.;    // ����������� ���� (����������� ������ ����) [�������]
      //bool isFog = false;            // �����
   };

   // ������������ ����������� 360
   inline double normal_360(double K) { K -= (int)(K / 360) * 360.; return K < 0 ? 360. + K : K; }

   inline weather_info normalize(const weather_info& w, const track_point_info& move_info)
   {
      weather_info res = w;
      res.windDirection    = normal_360(res.windDirection - move_info.course);
      res.currentDirection = normal_360(res.currentDirection - move_info.course);
      res.waveDirection    = normal_360(res.waveDirection - move_info.course);
      res.swellDirection   = normal_360(res.swellDirection - move_info.course);
      return res;
   }

   using ForbiddenTurnSectors = std::vector<forbidden_turn_sector>;

   //! ������������� ������ ��������
   inline ForbiddenTurnSectors process_forbidden_turn_sector(const forbidden_turn_sector& fts)
   {
      ForbiddenTurnSectors res;
      if (fts.angleFrom == colreg::NO_VALUE || fts.angleTo == colreg::NO_VALUE)
         return res;

      const auto start = normal_360(fts.angleFrom);
      const auto end   = normal_360(fts.angleTo);
      if (start > end)
      {
         res.emplace_back(forbidden_turn_sector{ start, 360. });
         res.emplace_back(forbidden_turn_sector{ 0., end });
      }
      else
         res.emplace_back(forbidden_turn_sector{ start, end });

      return res;
   }

   struct weather_influence_info
   {
      weather_info            weather;
      forbidden_turn_sector   fts;        //< ������ ����� ����������� ��� �������� (�� ������ ��������� ������� � ��)
   };

#pragma pack (pop)

   //! ��������� ��� ����� ������� ������
   struct iWeatherInfluence : iReleasable
   {
      /*! ������� ������� ������ �� �������
      \param[in] info ��������� �������
      \param[in] move_info ������� + ��������� �������� �������
      */
      virtual weather_info GetWeatherInfo(const geo_point& pos) const = 0;
      virtual forbidden_turn_sector Estimate(const ship_info& info, const track_point_info& move_info) const = 0;
      virtual bool IsFog(const geo_point& pos) const = 0;
   };

}