#include "stdafx.h"
#include "ChartLayer.h"
#include "simulator\simulator.h"

// HACK: ������ ����...�� ��� ���� �� ��������� �������������
// HACK: ���� ������ ������ ������ � ������ ������ ���� �� ������ �� .vcxproj, ����� ��������� ������
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
   if (!sim || sim->GetSimulatorScenarioState() == ColregSimulation::SCENARIO_STATUS::SS_NOT_LOADED)
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

bool ChartLayer::synchronize_map(/*const colreg::iChartSafetyCheck* checker, */render::iRender* renderer, const colreg::chart_objects_ref& chartObjects)
{
   //m_chartUSN = checker->GetObjectsUSN();
   renderer->Clear();

   for (size_t iObj = 0; iObj < chartObjects.size; ++iObj)
   {
      const auto& obj = chartObjects.arr[iObj];
      addChartObject(renderer, obj);
   }

   return true;
}

void ChartLayer::addChartObject(render::iRender* renderer, const colreg::chart_object& obj)
{
   using namespace render;
   math::geo_points points{ obj.geom.arr->begin(), obj.geom.arr->end() };
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
   for (auto& prop : obj.props)
   {
      if (strcmp(prop.key, "Color") == 0)
      {
         colorOverride = true;
         sscanf(prop.val, "%i %i %i", &r, &g, &b);
      }
   }
   if (colorOverride)
      info.color = RGB(r, g, b);
   //if (colreg::check_chart_obj_type(colreg::OT_POINTS, obj.type))
      //info.width = 3;

   //const auto id = colreg::check_chart_obj_type(colreg::OT_DYNAMIC_AREAS | colreg::OT_STATISTIC_AREA, obj.type) ? obj.id.dynamic_id : obj.id.static_id;
   //    if (id == 147838)
   //       info.width = 10;
   renderer->AddObject({ points, info, {render::FIND_TYPE::FT_FIND_DETAILED, obj.id, render::FIND_OBJECT_TYPE::FOT_CHART_OBJECT, 0, obj.type}, minScale }, false);
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

void ChartLayer::initObjInfo()
{
   m_objInfo[colreg::OBJECT_TYPE::OT_BORDER_AREA].color = RGB(250, 200, 100);
   m_objInfo[colreg::OBJECT_TYPE::OT_BORDER_AREA].width = 2;
   m_objInfo[colreg::OBJECT_TYPE::OT_BORDER_AREA].fill = render::FILL_TYPE::FT_NONE;

   m_objInfo[colreg::OBJECT_TYPE::OT_ISOLINE].color = RGB(0, 127, 255);
   m_objInfo[colreg::OBJECT_TYPE::OT_ISOLINE].width = 2;

   m_objInfo[colreg::OBJECT_TYPE::OT_NO_GO_AREA].color = RGB(255, 0, 0);
   m_objInfo[colreg::OBJECT_TYPE::OT_NO_GO_AREA].width = 3;
   m_objInfo[colreg::OBJECT_TYPE::OT_NO_GO_AREA].style = render::LINE_STYLE::LL_SOLID;
   m_objInfo[colreg::OBJECT_TYPE::OT_NO_GO_AREA].fill = render::FILL_TYPE::FT_DIAGCROSS;

   m_objInfo[colreg::OBJECT_TYPE::OT_NO_FLY_AREA].color = RGB(200, 200, 0);
   m_objInfo[colreg::OBJECT_TYPE::OT_NO_FLY_AREA].width = 1;
   m_objInfo[colreg::OBJECT_TYPE::OT_NO_FLY_AREA].style = render::LINE_STYLE::LL_DASH_DOT;
   m_objInfo[colreg::OBJECT_TYPE::OT_NO_FLY_AREA].fill = render::FILL_TYPE::FT_FDIAGONAL;
}