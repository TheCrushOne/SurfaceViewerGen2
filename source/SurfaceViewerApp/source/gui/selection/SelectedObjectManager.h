#pragma once
#include "common\base_class.h"
#include "property.h"
//#include "colreg/ChartSafetyStructs.h"
//#include "ColregSimulation.h"
#include "common\observer.h"
#include "gui/layers\RenderLayers.h"
#include "scenario/ScenarioManager.h"


class SelectObjectObserver : public AutoContainer< SelectObjectObserver>
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
   virtual bool IsUpdateable()const { return false; }
};

using iSelectablePtr = std::unique_ptr< iSelected>;

class SelectedObjectManager
   : public Singleton< SelectedObjectManager>
   , public ScenarioObserverBase
{
public:
   SelectedObjectManager();

   void Unselect();
   bool Select(const render::find_info& info);
   void Delete();

   iSelected* GetSelected() { return _selected.get(); }
   colreg::id_type GetSelectedShip()const { return _selected_ship_id; }
   colreg::chart_object_id GetSelectedChartObj()const { return _selected_chart_id; }
   render::FIND_OBJECT_TYPE GetSelectedObjectType() const { return _info.find_object_type; }

   bool OnScenarioCheckOpened() override { Unselect(); return true; }
   bool OnScenarioTimeChanged(double time) { Update();  return true; }
   bool OnScenarioModified() { Update();  return true; }
private:
   void Update();
private:
   render::find_info _info;
   iSelectablePtr _selected;
   colreg::id_type _selected_ship_id = colreg::INVALID_ID;
   colreg::chart_object_id _selected_chart_id;
};