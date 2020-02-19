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
   colreg::id_type _id;
   colreg::OBJECT_TYPE _chart_object_type;
   std::string _strType;
   std::string  _propValue;
   colreg::geo_points _points;
   colreg::geo_points_ref _points_ref;
   colreg::object_props_ref _props_ref;

   std::unique_ptr<FolderProperty> _info_folder;
   iPropertyPtr _prop_id;
   iPropertyPtr _prop_type;

   static constexpr size_t MAX_PROPS = 20;
   struct prop_info
   {
      std::string key;
      std::string value;
      iPropertyPtr prop_prop;
   };
   prop_info _props[MAX_PROPS];

   enum SELECT_TYPE
   {
      ST_POINT = 0,
      SP_SEGMENT,
   };

   SELECT_TYPE _selectedType;
   colreg::geo_point _geoEdit;
   size_t _index = 0;

   colreg::chart_object_id _obj_id;
   colreg::OBJECT_TYPE  _type;

};

