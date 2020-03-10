#include "stdafx.h"
#include "SelectedChartObject.h"
#include "simulator\simulator.h"
#include "gui/layers/renderhelper.h"
#include "gui/user_interface.h"

SelectedChartObject::SelectedChartObject(colreg::id_type id, colreg::OBJECT_TYPE chart_object_type)
   : _id{ id }
   , _chart_object_type{ chart_object_type }
   , _obj_id{ id }
{
   // NOTE: загрузка инфы по выбранному объекту
   auto pObj = simulator::getSimulator()->GetState().GetChartObject(_obj_id);

   //if (pObj && pObj->Get() && pObj->Get()->size)
   //{
   //   _chart_object_type = pObj->Get()->arr[0].type;
   //}
   _info_folder = std::make_unique< FolderProperty>("chart object info");
   _prop_id = std::make_unique< ValuePropertyHolder< SelectedChartObject, decltype(_id)>>
      ("ID", "Chart object id", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectedChartObject::_id, &SelectedChartObject::OnSimSettingChanged, this);
   _info_folder->AddChild(_prop_id.get());

   if (pObj)
   {
      //const auto obj = pObj.arr[0];
      _points = math::geo_points{ pObj->geom.arr->begin(), pObj->geom.arr->end() };

      if ((pObj->props.arr)
         && _stricmp(pObj->props.arr[0].key, "depth") == 0)
      {
         size_t i = 0;
         for (const auto& p : pObj->props)
         {
            _props[i].key = p.key;
            _props[i].value = p.val;
            bool readonly = true/*check_chart_obj_type(obj.type, colreg::OT_XTE_AREA) && p.key && strcmp(p.key, "STATISTIC_INFO") == 0;*/;
            _props[i].prop_prop = std::make_unique< ValuePropertyHolder< prop_info, decltype(_props[i].value)>>
               (p.key, p.key, readonly, VALUE_FORMAT_TYPE::VFT_NONE, &_props[i], &prop_info::value, &SelectedChartObject::OnSimSettingChanged, this);
            _info_folder->AddChild(_props[i].prop_prop.get());
            ++i;
         }
      }
      //pObj->Release();
   }
   else
   {
      SelectedObjectManager::GetInstance().Unselect();
   }

   _strType = colreg::chart_obj_type_to_str(_chart_object_type);
   _prop_type = std::make_unique< ValuePropertyHolder< SelectedChartObject, decltype(_strType)>>
      ("Type", "Chart object type", true, VALUE_FORMAT_TYPE::VFT_NONE, this, &SelectedChartObject::_strType, &SelectedChartObject::OnSimSettingChanged, this);
   _info_folder->AddChild(_prop_type.get());

   AddChild(_info_folder.get());
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
   if (colreg::check_chart_obj_type(colreg::OBJECT_TYPE::OT_AREAS, _chart_object_type))
   {
      renderer->AddObject({ _points,{ 2, render::LINE_STYLE::LL_NONE, render::FILL_TYPE::FT_SOLID, user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
      renderer->AddObject({ _points,{ 4, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, user_interface::selectedColor, "", 0, 0, 200} });
   }
   else if (colreg::check_chart_obj_type(colreg::OT_LINES, _chart_object_type))
   {
      renderer->AddObject({ _points,{ 4, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE, user_interface::selectedColor, "", 0, 0, 200 } });
   }

   for (size_t i = 0; i < _points.size(); ++i)
      renderer->AddObject({ { _points[i] },{ 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE, RGB(200, 0, 0), ""}
                                                            ,{render::FIND_TYPE::FT_FIND_FAST, (colreg::id_type) i, render::FIND_OBJECT_TYPE::FOT_SELECTED, SELECT_TYPE::ST_POINT } });

}
void SelectedChartObject::StartEdit(render::iRender* renderer, CPoint point, render::find_info info)
{
   //_selectedType = (SELECT_TYPE)info.user_data;
   //_geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });
   //_index = findObjectPointIndex(_geoEdit);
}

size_t SelectedChartObject::findObjectPointIndex(const colreg::geo_point& geopt) const
{
   std::vector< double > distances;
   distances.resize(_points.size());
   for (size_t i = 0; i < _points.size(); ++i)
   {
      distances[i] = math::distance(_points[i], geopt);
   }

   double minDist = colreg::NO_VALUE;
   size_t bestIndex = colreg::NO_VALUE;
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
   if (_index == colreg::INVALID_INDEX)
   {
      return;
   }
   _geoEdit = renderer->PixelToGeo(math::point{ (double)point.y, (double)point.x });

   if (!check_chart_obj_type(colreg::OBJECT_TYPE::OT_AREAS, _chart_object_type) || _points.size() < 3)
   {
      if (_index > 0)
      {
         renderer->AddObject({ { _points[_index - 1], _geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
         std::stringstream text;
         text << "dir: " << get_formated_value(math::direction(_points[_index - 1], _geoEdit), VALUE_FORMAT_TYPE::VFT_COURSE);
         text << ", d: " << get_formated_value(math::distance(_points[_index - 1], _geoEdit), VALUE_FORMAT_TYPE::VFT_DISTANCE);

         renderer->AddObject({ {_geoEdit }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, text.str(), 25, 0, user_interface::selectedAlpha } });
      }

      if (_index < _points.size() - 1)
         renderer->AddObject({ { _geoEdit,  _points[_index + 1] }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
   }
   else
   {

      colreg::geo_point pt1;
      colreg::geo_point pt2;
      if (_index == 0)
      {
         pt1 = _points[1];
         pt2 = _points[_points.size() - 2];
      }
      else if (_index == (_points.size() - 1))
      {
         pt1 = _points[_points.size() - 2];
         pt2 = _points[1];
      }
      else
      {

         pt1 = _points[_index - 1];
         pt2 = _points[_index + 1];
      }


      renderer->AddObject({ { pt1, _geoEdit }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });

      renderer->AddObject({ {_geoEdit }, { 5, render::LINE_STYLE::LL_SOLID, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 25, 0, user_interface::selectedAlpha } });

      renderer->AddObject({ { _geoEdit,  pt2 }, { 5, render::LINE_STYLE::LL_DASH, render::FILL_TYPE::FT_NONE , user_interface::selectedColor, "", 0, 0, user_interface::selectedAlpha } });
   }

}

void SelectedChartObject::EndEdit()
{
   if (_index == colreg::INVALID_INDEX || !_points.size())
   {
      return;
   }
   switch (_selectedType)
   {
   case  ST_POINT:
   {
      _points[_index] = _geoEdit;

      //if (check_chart_obj_type(colreg::OBJECT_TYPE::OT_AREAS, _chart_object_type))
      {//поддержим замкнутость 
         if (_index == 0)
         {
            _points.back() = _geoEdit;
         }
         else if (_index == (_points.size() - 1))
         {
            _points[0] = _geoEdit;
         }
      }

      break;
   }
   case SP_SEGMENT:
   {
      break;
   }
   }


   //colreg::chart_object object;
   //object.type = _chart_object_type;
   //object.id = _obj_id;

   //object.geom = colreg::object_geometry_ref{ &colreg::geo_points_ref{ reinterpret_cast<colreg::geo_point*>(_points.data()), _points.size() } , 1 };

   Delete();

   //ScenarioManager::GetInstance().AddObject(object);
}

void SelectedChartObject::Delete()
{
   colreg::chart_object_id cid;
   //colreg::check_chart_obj_type(colreg::OT_DYNAMIC_AREAS | colreg::OT_STATISTIC_AREA, _chart_object_type) ? cid.dynamic_id = _id : cid.static_id = _id;
   //ScenarioManager::GetInstance().RemoveObject(cid, _chart_object_type);
}

