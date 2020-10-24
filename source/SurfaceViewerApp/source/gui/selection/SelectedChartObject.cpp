#include "stdafx.h"
#include "SelectedChartObject.h"
#include "simulator\simulator.h"
#include "gui/layers/renderhelper.h"
#include "gui/user_interface.h"

using namespace SV;

SelectedChartObject::SelectedChartObject(id_type id, chart_object::OBJECT_TYPE chart_object_type)
   : m_id{ id }
   , m_chart_object_type{ chart_object_type }
   , m_obj_id{ id }
{
   // NOTE: загрузка инфы по выбранному объекту
   auto* pObj = simulator::getSimulator()->GetState().GetChartObjectById(m_obj_id);

   //if (pObj && pObj->Get() && pObj->Get()->size)
   //{
   //   _chart_object_type = pObj->Get()->arr[0].type;
   //}
   m_info_folder = std::make_unique< FolderProperty>("chart object info");
   m_prop_id = std::make_unique< ValuePropertyHolder< SelectedChartObject, decltype(m_id)>>
      ("ID", "Chart object id", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectedChartObject::m_id, &SelectedChartObject::OnSimSettingChanged, this);
   m_info_folder->AddChild(m_prop_id.get());

   if (pObj)
   {
      //const auto obj = pObj.arr[0];
      //m_points = std::vector<math::geo_points>{ pObj->geom_contour_vct.begin(), pObj->geom_contour_vct.end() };

      size_t i = 0;
      for (const auto& p : pObj->GetProps())
      {
         m_props[i].key = p.key;
         m_props[i].value = p.val;
         bool readonly = true/*check_chart_obj_type(obj.type, colreg::OT_XTE_AREA) && p.key && strcmp(p.key, "STATISTIC_INFO") == 0;*/;
         m_props[i].prop_prop = std::make_unique<ValuePropertyHolder<prop_info, decltype(m_props[i].value)>>
            (p.key.c_str(), p.key.c_str(), readonly, VALUE_FORMAT_TYPE::VFT_NONE, &m_props[i], &prop_info::value, &SelectedChartObject::OnSimSettingChanged, this);
         m_info_folder->AddChild(m_props[i].prop_prop.get());
         ++i;
      }
   }
   else
   {
      SelectedObjectManager::GetInstance().Unselect();
   }

   m_strType = chart_object::chart_obj_type_to_str(m_chart_object_type);
   m_prop_type = std::make_unique< ValuePropertyHolder< SelectedChartObject, decltype(m_strType)>>
      ("Type", "Chart object type", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectedChartObject::m_strType, &SelectedChartObject::OnSimSettingChanged, this);
   m_info_folder->AddChild(m_prop_type.get());

   AddChild(m_info_folder.get());
}

void SelectedChartObject::OnSimSettingChanged()
{
   //std::vector<colreg::simple_prop> props;
   //for (size_t i{ 0 }; i < MAX_PROPS; ++i)
   //{
   //   if (!_props[i].prop_prop)
   //      break;

   //   props.emplace_back(colreg::simple_prop{ _props[i].key.c_str(), _props[i].value.c_str() });
   //}

   //colreg::chart_object object;
   //object.type = _chart_object_type;
   //object.id = _obj_id;

   //_points_ref.arr = _points.data();
   //_points_ref.size = _points.size();

   //_props_ref.arr = props.data();
   //_props_ref.size = props.size();

   //object.geom = colreg::object_geometry_ref{ &_points_ref, 1 };
   //object.props = _props_ref;
   //Delete();
   //ScenarioManager::GetInstance().AddObject(object);
}


void SelectedChartObject::Render(render::iRender* renderer)
{
   if (chart_object::check_chart_obj_type(chart_object::OBJECT_TYPE::OT_AREAS, m_chart_object_type))
   {
      for (auto& contour : m_points)
      {
         renderer->AddObject({ contour, { 2, render::LINE_STYLE::LL_NONE, render::FILL_TYPE::FT_SOLID, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
         renderer->AddObject({ contour, { 4, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, user_interface::selectedColor, "", 0, 0, 200} });
      }
   }
   else if (chart_object::check_chart_obj_type(chart_object::OT_LINES, m_chart_object_type))
   {
      for (auto& contour : m_points)
      {
         renderer->AddObject({ contour,{ 4, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, user_interface::selectedColor, "", 0, 0, 200 } });
      }
   }
   for (auto& contour : m_points)
   {
      for (size_t i = 0; i < contour.size(); ++i)
         renderer->AddObject({ { contour[i] }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(200, 0, 0), "" }
                             , { render::FIND_TYPE::FT_FIND_FAST, static_cast<id_type>(i), render::FIND_OBJECT_TYPE::FOT_SELECTED, SELECT_TYPE::ST_POINT } });
   }

}
void SelectedChartObject::StartEdit(render::iRender* renderer, CPoint point, render::find_info info)
{
   //_selectedType = (SELECT_TYPE)info.user_data;
   //_geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
   //_index = findObjectPointIndex(_geoEdit);
}

size_t SelectedChartObject::findObjectPointIndex(const CG::geo_point& geopt) const
{
   std::vector<double> distances;
   distances.resize(m_points.size(), std::numeric_limits<double>::max());
   for (size_t i = 0; i < m_points.size(); ++i)
   {
      for (size_t jdx = 0; jdx < m_points[i].size(); jdx++)
         distances[i] = std::min(distances[i], math::distance(m_points[i][jdx], geopt));
   }

   double minDist = NO_VALUE;
   size_t bestIndex = INVALID_INDEX;
   for (size_t i = 0; i < distances.size(); ++i)
   {
      if (distances[i] < minDist)
      {
         minDist = distances[i];
         bestIndex = i;
      }
   }
   return bestIndex;
}

void SelectedChartObject::Edit(render::iRender* renderer, CPoint point)
{
   // NOTE: не надо, пока что...
   return;
   /*if (m_index == colreg::INVALID_INDEX)
   {
      return;
   }
   m_geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });

   if (!check_chart_obj_type(colreg::OBJECT_TYPE::OT_AREAS, m_chart_object_type) || m_points.size() < 3)
   {
      if (m_index > 0)
      {
         renderer->AddObject({ { m_points[m_index - 1], m_geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
         std::stringstream text;
         text << "dir: " << get_formated_value(math::direction(m_points[m_index - 1], m_geoEdit), VALUE_FORMAT_TYPE::VFT_COURSE);
         text << ", d: " << get_formated_value(math::distance(m_points[m_index - 1], m_geoEdit), VALUE_FORMAT_TYPE::VFT_DISTANCE);

         renderer->AddObject({ {m_geoEdit }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, text.str(), 25, 0, user_interface::selectedAlpha } });
      }

      if (m_index < m_points.size() - 1)
         renderer->AddObject({ { m_geoEdit, m_points[m_index + 1] }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
   }
   else
   {

      colreg::geo_point pt1;
      colreg::geo_point pt2;
      if (m_index == 0)
      {
         pt1 = m_points[1];
         pt2 = m_points[m_points.size() - 2];
      }
      else if (m_index == (m_points.size() - 1))
      {
         pt1 = m_points[m_points.size() - 2];
         pt2 = m_points[1];
      }
      else
      {

         pt1 = m_points[m_index - 1];
         pt2 = m_points[m_index + 1];
      }


      renderer->AddObject({ { pt1, m_geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });

      renderer->AddObject({ { m_geoEdit }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 25, 0, user_interface::selectedAlpha } });

      renderer->AddObject({ { m_geoEdit,  pt2 }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
   }*/

}

void SelectedChartObject::EndEdit()
{
   //if (m_index == colreg::INVALID_INDEX || !m_points.size())
   //{
   //   return;
   //}
   //switch (m_selectedType)
   //{
   //case  ST_POINT:
   //{
   //   m_points[m_index] = m_geoEdit;

   //   //if (check_chart_obj_type(colreg::OBJECT_TYPE::OT_AREAS, _chart_object_type))
   //   {//поддержим замкнутость 
   //      if (m_index == 0)
   //      {
   //         m_points.back() = m_geoEdit;
   //      }
   //      else if (m_index == (m_points.size() - 1))
   //      {
   //         m_points[0] = m_geoEdit;
   //      }
   //   }

   //   break;
   //}
   //case SP_SEGMENT:
   //{
   //   break;
   //}
   //}


   //colreg::chart_object object;
   //object.type = _chart_object_type;
   //object.id = _obj_id;

   //object.geom = colreg::object_geometry_ref{ &colreg::geo_points_ref{ reinterpret_cast<colreg::geo_point*>(_points.data()), _points.size() } , 1 };

   Delete();

   //ScenarioManager::GetInstance().AddObject(object);
}

void SelectedChartObject::Delete()
{
   //colreg::chart_object_id cid;
   //colreg::check_chart_obj_type(colreg::OT_DYNAMIC_AREAS | colreg::OT_STATISTIC_AREA, _chart_object_type) ? cid.dynamic_id = _id : cid.static_id = _id;
   //ScenarioManager::GetInstance().RemoveObject(cid, _chart_object_type);
}