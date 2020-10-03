#pragma once
#include "SelectedObjectManager.h"


class SelectedChartObject : public iSelected
{

public:
   SelectedChartObject(colreg::id_type id, colreg::OBJECT_TYPE chart_object_type);

   void Render(render::iRender* renderer)override;

   void StartEdit(render::iRender* renderer, CPoint point, render::find_info info)override;
   void Edit(render::iRender* renderer, CPoint point)override;
   void EndEdit() override;
   void Delete() override;
   bool IsCanDelete() override { return true; }
private:
   void OnSimSettingChanged();
   size_t findObjectPointIndex(const colreg::geo_point& geopt) const;
private:
   colreg::id_type m_id;
   colreg::OBJECT_TYPE m_chart_object_type;
   std::string m_strType;
   std::string m_propValue;
   std::vector<math::geo_points> m_points;
   colreg::geo_points_vct m_point_vct;
   colreg::object_props_vct m_prop_vct;

   std::unique_ptr<FolderProperty> m_info_folder;
   iPropertyPtr m_prop_id;
   iPropertyPtr m_prop_type;

   static constexpr size_t MAX_PROPS = 20;
   struct prop_info
   {
      std::string key;
      std::string value;
      iPropertyPtr prop_prop;
   };
   prop_info m_props[MAX_PROPS];

   enum SELECT_TYPE
   {
      ST_POINT = 0,
      SP_SEGMENT,
   };

   SELECT_TYPE m_selectedType;
   colreg::geo_point m_geoEdit;
   size_t m_index = 0;

   colreg::chart_object_id m_obj_id;
};

