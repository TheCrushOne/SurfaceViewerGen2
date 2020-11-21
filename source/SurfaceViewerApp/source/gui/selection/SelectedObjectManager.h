#pragma once
#include "common\base_class.h"
#include "properties/PropertyModify.h"
#include "properties/FolderPropertyHolder.h"
//#include "colreg/ChartSafetyStructs.h"
//#include "ColregSimulation.h"
#include "common\observer.h"
#include "gui/layers\RenderLayers.h"
#include "scenario/ScenarioManager.h"

namespace SV
{
   class SelectObjectObserver
      : public AutoContainer<SelectObjectObserver>
   {
   public:
      static void SelectObject(iProperty* prop)
      {
         for (auto& obj : _objects)
            obj->OnObjectSelected(prop);
      }
   protected:
      virtual bool OnObjectSelected(iProperty* prop) = 0;
   };

   struct iSelected
      : public FolderProperty
      , public LayersContainer
   {
      virtual void StartEdit(render::iRender* renderer, CPoint point, render::find_info info) {}
      virtual void Edit(render::iRender* renderer, CPoint point) {}
      virtual void EndEdit() {}
      virtual void Delete() {}
      virtual bool IsCanDelete() { return false; }
      virtual bool IsUpdateable() const { return false; }
   };

   using iSelectablePtr = std::unique_ptr<iSelected>;

   class SelectedObjectManager
      : public Singleton<SelectedObjectManager>
      , public ScenarioObserverBase
   {
   public:
      SelectedObjectManager();

      void Unselect();
      bool Select(const render::find_info& info);
      void Delete();

      iSelected* GetSelected() { return m_selected.get(); }
      id_type GetSelectedShip() const { return m_selected_ship_id; }
      chart_object_id GetSelectedChartObj() const { return m_selected_chart_id; }
      render::FIND_OBJECT_TYPE GetSelectedObjectType() const { return m_info.find_object_type; }

      // NOTE: на всякий...
      // TODO: починить!!!
      bool OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status) override { /*Unselect();*/ return true; }
      bool OnScenarioTimeChanged(double time) { Update();  return true; }
      bool OnScenarioModified() { Update();  return true; }
   private:
      void Update();
   private:
      render::find_info m_info;
      iSelectablePtr m_selected;
      id_type m_selected_ship_id = INVALID_ID;
      chart_object_id m_selected_chart_id;
   };
}