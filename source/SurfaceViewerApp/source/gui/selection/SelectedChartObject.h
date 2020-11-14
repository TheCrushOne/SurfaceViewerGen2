#pragma once
#include "SelectedObjectManager.h"

namespace SV
{
   class SelectedChartObject
      : public iSelected
   {

   public:
      SelectedChartObject(id_type id, chart_object::OBJECT_TYPE chart_object_type);

      void Render(render::iRender* renderer) override;

      void StartEdit(render::iRender* renderer, CPoint point, render::find_info info) override;
      void Edit(render::iRender* renderer, CPoint point) override;
      void EndEdit() override;
      void Delete() override;
      bool IsCanDelete() override { return true; }
   private:
      void OnSimSettingChanged();
      size_t findObjectPointIndex(const CG::geo_point& geopt) const;
   private:
      id_type m_id;
      chart_object::OBJECT_TYPE m_chart_object_type;
      std::string m_strType;
      std::string m_propValue;
      CG::geo_contour_vct m_points;
      CG::geo_contour_vct m_point_vct;
      properties::simple_prop_vct m_prop_vct;

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
   public:
      enum SELECT_TYPE
      {
         ST_POINT = 0,
         SP_SEGMENT,
      };
   protected:
      SELECT_TYPE m_selectedType;
      CG::geo_point m_geoEdit;
      size_t m_index = 0;

      chart_object_id m_obj_id;
   };
}