#pragma once

namespace layer_provider
{
   struct unit_info
   {

   };

   // TODO: ���������� �� ��� ����
   //! ���������� �� �������
   struct ship_info
   {
      ship_info()
      {
         memset(this, 0, sizeof(ship_info));
         safety_contour = draft = -1;
      }

      char                 name[SVCG::MAX_SHIP_NAME_LENGHT];         ///< �������� �����
      id_type              id;                                 ///< ���������� ������������� �����
      int                  length;                             ///< �����, �����
      int                  width;                              ///< ������, �����
      int                  draft;                              ///< ������, �����
      int                  deadweight;                         ///< �������
      bool                 dangerous_cargo;                    ///< ������� ����
      int                  safety_contour;                     ///< �����, ���. ������� ��� ����������� �������� �������, ����� ���������� ����� (���������). �� ����� ���� ������ draft
   };
}

namespace SVCG
{
   namespace layer_provider
   {
      struct trajectory_point
      {
         trajectory_point()
            : speed{ NO_VALUE }
            , radius{ NO_VALUE }
            , left_XTE{ NO_VALUE }
            , right_XTE{ NO_VALUE }
         {}

         trajectory_point(const geo_point& p, double v_ = -1, double r = NO_VALUE, double lEXT = NO_VALUE, double rEXT = NO_VALUE)
            : pos(p)
            , speed(v_)
            , radius(r)
            , left_XTE{ lEXT }
            , right_XTE{ rEXT }
         {}

         trajectory_point(const trajectory_point&) = default;
         trajectory_point(trajectory_point&&)noexcept = default;
         trajectory_point& operator= (const trajectory_point& other) = default;
         trajectory_point& operator= (trajectory_point&& other)noexcept = default;

         inline bool operator == (const trajectory_point& b) const { return pos == b.pos; }
         inline operator geo_point() const { return pos; }
         inline bool is_valid() const { return pos.is_valid(); }

         geo_point pos;
         double    heading;   ///< �����������, �������
         double    speed;     ///< ��������, ����
         double    radius;    ///< ������ ��������, ���� 
         double    left_XTE;  ///< ������������ ������� �������� ����� �� ����� ��������, ����
         double    right_XTE; ///< ������������ ������� �������� ������ �� ����� ��������, ����
      };

      typedef std::vector<trajectory_point> trajectory_point_vct;
   }
}