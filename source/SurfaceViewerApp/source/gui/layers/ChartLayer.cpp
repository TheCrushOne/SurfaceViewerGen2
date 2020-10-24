#include "stdafx.h"
#include "ChartLayer.h"
#include "simulator\simulator.h"

using namespace SV;
// HACK: шайтан каст...но без него не линкуется автоконтейнер
// HACK: этот объект должен стоять в первом классе слоёв по списку из .vcxproj, иначе игнорятся модули
AutoContainer< LayersContainer>::ListObjects AutoContainer< LayersContainer>::_objects;
namespace
{
   ChartLayer layer;
}

void ChartLayer::Render(render::iRender* renderer)
{
   if (!LayersContainer::isLayerEnabled())
   {
      renderer->Clear();
      return;
   }
   const auto* sim = simulator::getSimulator();
   if (!sim || sim->GetSimulatorScenarioState() < surface_simulation::SCENARIO_STATUS::SS_MAPOBJ_PROCESSED)
      return;
   const auto& simulationState = sim->GetState();

   
   //colreg::ReleaseGuard<colreg::iChartObjects> chartObjs(safetyChecker->GetChartObjects());

   synchronize_map(renderer, simulationState.GetChartObjects());

   //size_t statAreaCount = sim->GetStatisticsAreaObjectsCount();

   //for (size_t i = 0; i < statAreaCount; ++i)
   //{
   //   const auto obj = sim->GetStatisticsAreaObjects(i);
   //   if (obj.id.dynamic_id == colreg::INVALID_ID && obj.id.static_id == colreg::INVALID_ID)
   //   {
   //      continue;
   //   }
   //   addChartObject(renderer, obj);
   //}
}
void ChartLayer::OnShowDepthChartObjectsChanged()
{
   user_interface::InvalidateView();
}

void ChartLayer::onLayerEnabledChanged()
{
}

bool ChartLayer::synchronize_map(render::iRender* renderer, const SV::layer_provider::layer_chart_object_vct* chartObjects)
{
   //m_chartUSN = checker->GetObjectsUSN();
   renderer->Clear();

   for (size_t iObj = 0; iObj < (*chartObjects).size(); ++iObj)
   {
      const auto& obj = (*chartObjects).at(iObj);
      addChartObject(renderer, obj);
   }

   return true;
}

void ChartLayer::addChartObject(render::iRender* renderer, const SV::layer_provider::layer_chart_object& obj)
{
   using namespace render;
   const auto itf = m_objInfo.find(obj.type);
   render::object_info info;
   if (itf != m_objInfo.end())
      info = (*itf).second;

   float minScale = 0.0;
   //if (obj.props.arr/* && _stricmp(obj.props.arr[0].key, "depth") == 0*/)
   //{
   //   /*if (!_showDepthChartObjects)
   //   {
   //      return;
   //   }*/
   //   std::stringstream s;
   //   s << std::setprecision(2) << atof(obj.props.arr[0].val) << "m";
   //   info.text = s.str();
   //   //minScale = _minScale2ShowDepthObjects;
   //}

   int r = 0, g = 0, b = 0;
   bool colorOverride = false;
   for (auto& prop : obj.prop_vct)
   {
      if (strcmp(prop.key.c_str(), "Color") == 0)
      {
         colorOverride = true;
         sscanf(prop.val.c_str(), "%i %i %i", &r, &g, &b);
      }
   }
   if (colorOverride)
      info.color = RGB(r, g, b);
   //if (colreg::check_chart_obj_type(colreg::OT_POINTS, obj.type))
      //info.width = 3;

   //const auto id = colreg::check_chart_obj_type(colreg::OT_DYNAMIC_AREAS | colreg::OT_STATISTIC_AREA, obj.type) ? obj.id.dynamic_id : obj.id.static_id;
   //    if (id == 147838)
   //       info.width = 10;

   for (auto& contour : obj.geom_contour_vct)
      renderer->AddObject({ contour, info, {render::FIND_TYPE::FT_FIND_DETAILED, obj.id, render::FIND_OBJECT_TYPE::FOT_CHART_OBJECT, 0, obj.type}, minScale }, false);
   render::object_info ptInfo{ 1, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, RGB(110, 110, 110) };
   ptInfo.alpha = 255;
   if (colorOverride)
      ptInfo.color = RGB(r, g, b);
   ptInfo.width += 2;
   for (auto& contour : obj.geom_contour_vct)
      for (size_t i = 0; i < contour.size(); i++)
         renderer->AddObject({ {contour[i]}, ptInfo });
}

iProperty* ChartLayer::GetProperties()
{
   if (!_props)
   {
      auto folderProps = std::make_unique< FolderProperty>("Chart Layer");

      folderProps->AddChild(GetLayerEnabledProperty());
      _prop_showDepthChartObjects = std::make_unique< ValuePropertyHolder< ChartLayer, decltype(_showDepthChartObjects)>>
         ("Show Depth Objects", "Show Depth Chart Objects", false, VALUE_FORMAT_TYPE::VFT_NONE, this, &ChartLayer::_showDepthChartObjects, &ChartLayer::OnShowDepthChartObjectsChanged, this);

      folderProps->AddChild(_prop_showDepthChartObjects.get());

      _prop_minScale2ShowDepthObjects = std::make_unique< ValuePropertyHolder< ChartLayer, decltype(_minScale2ShowDepthObjects)>>
         ("Min. Scale 2 Show Depth Objects", "Minimum zoom scale to show depth objects", false, VALUE_FORMAT_TYPE::VFT_NONE, this, &ChartLayer::_minScale2ShowDepthObjects, &ChartLayer::OnShowDepthChartObjectsChanged, this);

      folderProps->AddChild(_prop_minScale2ShowDepthObjects.get());
      _props = std::move(folderProps);
   }

   return _props.get();
}

bool ChartLayer::OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status)
{
   bool res = true;
   switch (status)
   {
   case surface_simulation::SCENARIO_STATUS::SS_MAP_CHECKOPENED:
      res &= onScenarioCheckOpened();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_MAP_PROCESSED:
      res &= onScenarioMapProcessed();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_MAPOBJ_PROCESSED:
      res &= onScenarioMapObjProcessed();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_PATHS_COUNTED:
      res &= onScenarioPathFound();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_OPT_PATHS_COUNTED:
      res &= onScenarioOptPathFound();
      break;
   case surface_simulation::SCENARIO_STATUS::SS_NOT_LOADED:
   default:
      break;
   }
   return res;
}

bool ChartLayer::onScenarioCheckOpened()
{
   m_chartUSN = INVALID_ID;
   return true;
}

bool ChartLayer::onScenarioMapProcessed()
{
   return true;
}

bool ChartLayer::onScenarioMapObjProcessed()
{
   return true;
}

bool ChartLayer::onScenarioPathFound()
{
   return true;
}

bool ChartLayer::onScenarioOptPathFound()
{
   return true;
}

void ChartLayer::initObjInfo()
{
   m_objInfo[chart_object::OBJECT_TYPE::OT_BORDER_AREA].color = RGB(250, 200, 100);
   m_objInfo[chart_object::OBJECT_TYPE::OT_BORDER_AREA].width = 2;
   m_objInfo[chart_object::OBJECT_TYPE::OT_BORDER_AREA].fill = render::FILL_TYPE::FT_NONE;

   m_objInfo[chart_object::OBJECT_TYPE::OT_ISOLINE].color = RGB(0, 127, 255);
   m_objInfo[chart_object::OBJECT_TYPE::OT_ISOLINE].width = 2;

   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_GO_AREA].color = RGB(255, 0, 0);
   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_GO_AREA].width = 3;
   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_GO_AREA].style = render::LINE_STYLE::LL_SOLID;
   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_GO_AREA].fill = render::FILL_TYPE::FT_DIAGCROSS;

   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_FLY_AREA].color = RGB(200, 200, 0);
   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_FLY_AREA].width = 1;
   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_FLY_AREA].style = render::LINE_STYLE::LL_DASH_DOT;
   m_objInfo[chart_object::OBJECT_TYPE::OT_NO_FLY_AREA].fill = render::FILL_TYPE::FT_FDIAGONAL;
}