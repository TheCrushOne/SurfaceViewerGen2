#include "stdafx.h"
#include "ChartLayer.h"
#include "simulator\simulator.h"
#include "gui\helpers\LayerFiltersManager.h"
#include "common/pathfinder_types.h"
#include "SVCG/positioning.h"

#define CR_CHART_LAYER_PROP(iPropPtr, name, description, prStruct, obj, field) PROPHELPER_CREATEHOLDER_L(iPropPtr, name, description, prStruct, obj, field, &ChartLayer::OnShowDepthChartObjectsChanged)

using namespace SV;
// HACK: шайтан каст...но без него не линкуется автоконтейнер
// HACK: этот объект должен стоять в первом классе слоёв по списку из .vcxproj, иначе игнорятся модули
AutoContainer< LayersContainer>::ListObjects AutoContainer< LayersContainer>::_objects;
namespace
{
   ChartLayer layer;
   const render::object_info coverage_obj_info()
   {
      return {
         5,
         render::LINE_STYLE::LL_DASH,
         render::FILL_TYPE::FT_NONE,
         RGB(110, 110, 110)
      };
   }

   const render::object_info explication_obj_info()
   {
      return {
         5,
         render::LINE_STYLE::LL_DASH,
         render::FILL_TYPE::FT_NONE,
         RGB(110, 110, 110)
      };
   }

   const render::object_info contour_point_obj_info()
   {
      return { 
         3,
         render::LINE_STYLE::LL_DASH,
         render::FILL_TYPE::FT_NONE,
         RGB(110, 110, 110)
      };
   }

   const render::find_info contour_find_info(chart_object_id id, chart_object::OBJECT_TYPE type)
   {
      return {
         render::FIND_TYPE::FT_FIND_DETAILED,
         id,
         render::FIND_OBJECT_TYPE::FOT_CHART_OBJECT,
         0,
         type
      };
   }
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
   const auto& env_stt = sim->GetAppSettings().env_stt;
   
   //colreg::ReleaseGuard<colreg::iChartObjects> chartObjs(safetyChecker->GetChartObjects());

   synchronize_map(renderer, simulationState, env_stt);

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

namespace
{
   unsigned long air_affilation_color(size_t uaff)
   {
      pathfinder::FlyZoneAffilation aff = static_cast<pathfinder::FlyZoneAffilation>(uaff);
      switch (aff)
      {
      case pathfinder::FlyZoneAffilation::FZA_NORMAL:
         return RGB(50, 255, 50);
      case pathfinder::FlyZoneAffilation::FZA_FORBIDDEN:
         return RGB(255, 50, 50);
      case pathfinder::FlyZoneAffilation::FZA_DANGEROUS:
         return RGB(255, 255, 50);
      default:
         ATLASSERT(false);
         return RGB(0, 0, 0);
      }
   }

   unsigned long land_affilation_color(size_t uaff)
   {
      pathfinder::GoZoneAffilation aff = static_cast<pathfinder::GoZoneAffilation>(uaff);
      switch (aff)
      {
      case pathfinder::GoZoneAffilation::GZA_NORMAL:
         return RGB(50, 255, 50);
      case pathfinder::GoZoneAffilation::GZA_FORBIDDEN:
         return RGB(255, 50, 50);
      case pathfinder::GoZoneAffilation::GZA_DANGEROUS:
         return RGB(255, 255, 50);
      default:
         ATLASSERT(false);
         return RGB(0, 0, 0);
      }
   }

   unsigned int getCoveragePointColor(size_t isCoveredU)
   {
      bool isCovered = isCoveredU >= 1;
      return isCovered ? RGB(100, 100, 255) : RGB(50, 50, 50);
   }
}

bool ChartLayer::synchronize_map(render::iRender* renderer, const SV::surface_simulation::iSimulationState& state, const settings::environment_settings& env_stt)
{
   //m_chartUSN = checker->GetObjectsUSN();
   renderer->Clear();

   if (LayerFiltersManager::GetInstance().IsFilterVisible({
      layer_filter_tag::explications,
      layer_filter_tag::air
   }))
      addExplication(renderer, state.GetAirUnitExplication(), &air_affilation_color, env_stt);
   if (LayerFiltersManager::GetInstance().IsFilterVisible({
      layer_filter_tag::explications,
      layer_filter_tag::land
   }))
      addExplication(renderer, state.GetLandUnitExplication(), &land_affilation_color, env_stt);

   for (size_t idx = 0; idx < state.GetCoverageHistory().size(); idx++)
   {
      if (LayerFiltersManager::GetInstance().IsFilterVisible({
         layer_filter_tag::coverages,
         layer_filter_tag::step_templ + std::to_string(idx)
      }))
         addCoverage(renderer, state.GetCoverageHistory().at(idx), env_stt);
   }

   for (size_t iObj = 0; iObj < state.GetChartObjectCount(); iObj++)
   {
      const auto* obj = state.GetChartObjectByIdx(iObj);
      addChartObject(renderer, obj);
   }

   return true;
}

void ChartLayer::addChartObject(render::iRender* renderer, const SV::surface_simulation::iLayerChartObject* obj)
{
   using namespace render;
   const auto itf = m_objInfo.find(obj->GetType());
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
   const auto* props = obj->GetProps();
   bool colorOverride = false;
   if (props)
   {
      for (auto& prop : *props)
      {
         if (strcmp(prop.key.c_str(), "Color") == 0)
         {
            colorOverride = true;
            sscanf(prop.val.c_str(), "%i %i %i", &r, &g, &b);
         }
      }
      if (colorOverride)
         info.color = RGB(r, g, b);
   }
   //if (colreg::check_chart_obj_type(colreg::OT_POINTS, obj.type))
      //info.width = 3;

   //const auto id = colreg::check_chart_obj_type(colreg::OT_DYNAMIC_AREAS | colreg::OT_STATISTIC_AREA, obj.type) ? obj.id.dynamic_id : obj.id.static_id;
   //    if (id == 147838)
   //       info.width = 10;

   const auto& contour_data = obj->GetContourData();
   chart_object_id id = obj->GetId();
   chart_object::OBJECT_TYPE type = obj->GetType();
   for (auto& contour : contour_data)
   {
      renderer->AddObject({
         contour,
         info,
         contour_find_info(id, type),
         minScale
      }, false);
   }
   render::object_info ptInfo = contour_point_obj_info();
   ptInfo.alpha = 255;
   if (colorOverride)
      ptInfo.color = RGB(r, g, b);
   for (auto& contour : contour_data)
   {
      for (size_t i = 0; i < contour.size(); i++)
      {
         renderer->AddObject({
            { contour[i] },
            ptInfo
         });
      }
   }
}

void ChartLayer::addCoverage(render::iRender* renderer, const pathfinder::UnsignedMatrix& coverage, const settings::environment_settings& env_stt)
{
   for (size_t ridx = 0; ridx < coverage.GetRowCount(); ridx++)
   {
      for (size_t cidx = 0; cidx < coverage.GetColCount(); cidx++)
      {
         auto ptInfo = coverage_obj_info();
         ptInfo.color = getCoveragePointColor(coverage.Get(ridx, cidx));
         renderer->AddObject({
            { CG::geo_point(static_cast<CG::geo_point>(transfercase::RoutePointToPositionPoint(CG::route_point(ridx, cidx), env_stt))) },
            ptInfo
         });
      }
   }
}

void ChartLayer::addExplication(render::iRender* renderer, const pathfinder::UnsignedMatrix& explication, std::function<unsigned long(size_t)> colorizer, const settings::environment_settings& env_stt)
{
   for (size_t ridx = 0; ridx < explication.GetRowCount(); ridx++)
   {
      for (size_t cidx = 0; cidx < explication.GetColCount(); cidx++)
      {
         auto ptInfo = explication_obj_info();
         ptInfo.color = colorizer(explication.Get(ridx, cidx));
         renderer->AddObject({
            { CG::geo_point(static_cast<CG::geo_point>(transfercase::RoutePointToPositionPoint(CG::route_point(ridx, cidx), env_stt))) },
            ptInfo
         });
      }
   }
}

iProperty* ChartLayer::GetProperties()
{
   if (!_props)
   {
      auto folderProps = std::make_unique<FolderProperty>("Chart Layer");

      folderProps->AddChild(GetLayerEnabledProperty());
      CR_CHART_LAYER_PROP(_prop_showDepthChartObjects, "Show Depth Objects", "Show Depth Chart Objects", ChartLayer, this, _showDepthChartObjects);

      folderProps->AddChild(_prop_showDepthChartObjects.get());
      CR_CHART_LAYER_PROP(_prop_minScale2ShowDepthObjects, "Min. Scale 2 Show Depth Objects", "Minimum zoom scale to show depth objects", ChartLayer, this, _minScale2ShowDepthObjects);

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