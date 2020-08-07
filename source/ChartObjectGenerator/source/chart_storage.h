#pragma once

#include <vector>
#include "colreg/CommonStructs.h"
#include "colreg/PropertyInterface.h"

namespace chart_object
{
   struct chart_storage
   {
      std::vector<std::vector<colreg::geo_point>> geom_contour_vct;
      //std::vector<colreg::geo_points_ref> geom_contour_ref_vct;
      std::vector<colreg::simple_prop> prop_vct;
      //std::vector<colreg::simple_prop_holder> prop_holder_vct; // NOTE: костыльный вектор для хранения свойств

      colreg::OBJECT_TYPE type;

      //void Commit()
      //{
      //   for (const auto& elem : geom_contour_vct)
      //      geom_contour_ref_vct.emplace_back(elem.data(), elem.size());
      //}
   };
}