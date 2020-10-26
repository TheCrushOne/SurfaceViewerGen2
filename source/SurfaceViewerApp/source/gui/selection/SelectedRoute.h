#pragma once
#include "SelectedObjectManager.h"
#include "colreg/PropertyInterface.h"

namespace SV
{
   class SelectRouteBase : public iSelected
   {
   public:
      SelectRouteBase(id_type id, size_t data);
      bool IsCanDelete() override { return true; }
   protected:
      id_type m_id;
      size_t m_index;
      surface_simulation::ROUTE_TYPE m_format_type;

      std::unique_ptr<FolderPropertyHolder> m_ship_info_folder;
      iPropertyInterfacePtr m_prop_id;
      iPropertyInterfacePtr m_prop_name;

      std::unique_ptr<FolderPropertyHolder> m_route_info_folder;
      iPropertyInterfacePtr m_route_type;

      CG::geo_point m_geoEdit;
      CG::layer_provider::trajectory_point_vct m_route;

      std::string m_typeName;
   private:
      void OnEmpty() {}
   };

   class SelectedRoutePoint : public SelectRouteBase
   {
   public:

      SelectedRoutePoint(id_type id, size_t data);
      void Render(render::iRender* renderer) override;

      void StartEdit(render::iRender* renderer, CPoint point, render::find_info info) override;
      void Edit(render::iRender* renderer, CPoint point)override;
      void EndEdit() override;
      void Delete() override;

   private:
      void OnSimSettingChanged();

   private:
      CG::layer_provider::trajectory_point m_point;

      std::unique_ptr<FolderPropertyHolder> m_point_folder;
      iPropertyInterfacePtr m_prop_index;
      iPropertyInterfacePtr m_prop_radius;
   };

   class SelectedRouteSegment : public SelectRouteBase
   {
   public:
      SelectedRouteSegment(id_type id, size_t data);
      void Render(render::iRender* renderer) override;

      void StartEdit(render::iRender* renderer, CPoint point, render::find_info info) override;
      void Edit(render::iRender* renderer, CPoint point)override;
      void EndEdit() override;
      void Delete() override;

   private:
      void OnSimSettingChanged();

   private:
      double m_direction;
      double m_distance;
      double m_xte_left;
      double m_xte_right;
      CG::layer_provider::trajectory_point m_pointFrom;
      CG::layer_provider::trajectory_point m_pointTo;

      std::unique_ptr<FolderPropertyHolder> m_segment_folder;
      iPropertyInterfacePtr m_prop_index;
      iPropertyInterfacePtr m_prop_speed;
      iPropertyInterfacePtr m_prop_direction;
      iPropertyInterfacePtr m_prop_distance;
      iPropertyInterfacePtr m_prop_xte_left;
      iPropertyInterfacePtr m_prop_xte_right;

      enum SEGMENT_PART
      {
         SP_START_POINT = 0,
         SP_END_POINT,
         SP_SEGMENT
      };
      SEGMENT_PART m_editPart;
   };
}