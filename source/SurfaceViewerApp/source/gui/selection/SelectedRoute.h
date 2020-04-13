#pragma once
#include "SelectedObjectManager.h"

class SelectRouteBase : public iSelected
{
public:

   SelectRouteBase(colreg::id_type id, size_t data);
   bool IsCanDelete() override { return true; }
protected:
   colreg::id_type _id;
   size_t _index;
   ColregSimulation::ROUTE_TYPE _format_type;

   std::unique_ptr<FolderProperty> _ship_info_folder;
   iPropertyPtr _prop_id;
   iPropertyPtr _prop_name;

   std::unique_ptr<FolderProperty> _route_info_folder;
   iPropertyPtr _route_type;

   colreg::geo_point _geoEdit;
   std::vector<colreg::route_point>_route;

   std::string _typeName;
private:
   void OnEmpty() {}
};


class SelectedRoutePoint : public SelectRouteBase
{
public:

   SelectedRoutePoint(colreg::id_type id, size_t data);
   void Render(render::iRender* renderer) override;

   void StartEdit(render::iRender* renderer, CPoint point, render::find_info info) override;
   void Edit(render::iRender* renderer, CPoint point)override;
   void EndEdit() override;
   void Delete() override;

private:
   void OnSimSettingChanged();

private:
   colreg::route_point _point;

   std::unique_ptr<FolderProperty> _point_folder;
   iPropertyPtr _prop_index;
   iPropertyPtr _prop_radius;

};



class SelectedRouteSegment : public SelectRouteBase
{
public:
   SelectedRouteSegment(colreg::id_type id, size_t data);
   void Render(render::iRender* renderer) override;

   void StartEdit(render::iRender* renderer, CPoint point, render::find_info info) override;
   void Edit(render::iRender* renderer, CPoint point)override;
   void EndEdit() override;
   void Delete() override;

private:
   void OnSimSettingChanged();

private:
   double _direction;
   double _distance;
   double _xte_left;
   double _xte_right;
   colreg::route_point _pointFrom;
   colreg::route_point _pointTo;

   std::unique_ptr<FolderProperty> _segment_folder;
   iPropertyPtr _prop_index;
   iPropertyPtr _prop_speed;
   iPropertyPtr _prop_direction;
   iPropertyPtr _prop_distance;
   iPropertyPtr _prop_xte_left;
   iPropertyPtr _prop_xte_right;

   enum SEGMENT_PART
   {
      SP_START_POINT = 0,
      SP_END_POINT,
      SP_SEGMENT
   };
   SEGMENT_PART _editPart;
};
