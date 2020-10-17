#pragma once

#include "common/base_types.h"

namespace SV::layer_provider
{
   // TODO: ���������� �� ��� ����
   //! ���������� �� �������
   struct ship_info
   {
      ship_info()
      {
         memset(this, 0, sizeof(ship_info));
         safety_contour = draft = -1;
      }

      char                 name[MAX_SHIP_NAME_LENGHT];         ///< �������� �����
      id_type              id;                                 ///< ���������� ������������� �����
      int                  length;                             ///< �����, �����
      int                  width;                              ///< ������, �����
      int                  draft;                              ///< ������, �����
      int                  deadweight;                         ///< �������
      bool                 dangerous_cargo;                    ///< ������� ����
      int                  safety_contour;                     ///< �����, ���. ������� ��� ����������� �������� �������, ����� ���������� ����� (���������). �� ����� ���� ������ draft
   };
}