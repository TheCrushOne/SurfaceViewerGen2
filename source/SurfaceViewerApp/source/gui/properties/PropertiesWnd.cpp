
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "gui/MainFrm.h"
//#include "ColregTestApp.h"
#include "gui/selection/SelectedObjectManager.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace SV;

#define ID_COMBO_VIEW_TYPE          7120
#define ID_COMBO_VIEW_TYPE_LABEL    7121
#define ID_COMBO_UNIT_OBJECT        7122
#define ID_COMBO_UNIT_OBJECT_LABEL  7123
#define ID_COMBO_CHART_OBJECT       7124
#define ID_COMBO_CHART_OBJECT_LABEL 7125
#define ID_COMBO_SETTINGS           7126
#define ID_COMBO_SETTINGS_LABEL     7127

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

namespace
{
   std::string windowFailureMsg(LPCSTR name)
   {
      return std::string("Не удалось создать окно ") + name + "\n";
   }
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_CBN_SELENDOK(ID_COMBO_VIEW_TYPE, OnViewTypeComboChanged)
   ON_CBN_SELENDOK(ID_COMBO_UNIT_OBJECT, OnUnitObjectComboChanged)
   ON_CBN_SELENDOK(ID_COMBO_CHART_OBJECT, OnChartObjectComboChanged)
   ON_CBN_SELENDOK(ID_COMBO_SETTINGS, OnSettingsComboChanged)

   ON_CBN_EDITUPDATE(ID_COMBO_VIEW_TYPE, OnViewTypeComboEdited)
   ON_CBN_EDITUPDATE(ID_COMBO_UNIT_OBJECT, OnUnitObjectComboEdited)
   ON_CBN_EDITUPDATE(ID_COMBO_CHART_OBJECT, OnChartObjectComboEdited)
   ON_CBN_EDITUPDATE(ID_COMBO_SETTINGS, OnSettingsComboEdited)

   ON_CBN_DROPDOWN(ID_COMBO_VIEW_TYPE, reloadViewTypeList)
   ON_CBN_DROPDOWN(ID_COMBO_UNIT_OBJECT, reloadUnitObjectList)
   ON_CBN_DROPDOWN(ID_COMBO_CHART_OBJECT, reloadChartObjectList)
   ON_CBN_DROPDOWN(ID_COMBO_SETTINGS, reloadSettingsList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
   CPropertiesWndBase::OnSize(nType, cx, cy);
}

void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow)
{
   //Get rectangles
   CRect rctComboBox, rctDropDown;
   //Combo rect
   pMyComboBox->GetClientRect(&rctComboBox);
   //DropDownList rect
   pMyComboBox->GetDroppedControlRect(&rctDropDown);

   //Get Item height
   int itemHeight = pMyComboBox->GetItemHeight(-1);
   //Converts coordinates
   pMyComboBox->GetParent()->ScreenToClient(&rctDropDown);
   //Set height
   rctDropDown.bottom = rctDropDown.top + rctComboBox.Height() + itemHeight * itemsToShow;
   //apply changes
   pMyComboBox->MoveWindow(&rctDropDown);
}


void CPropertiesWnd::adjustLayout(LPRECT rect)
{
   if (!m_viewTypeComboLabel
      || !m_viewTypeCombo
      || !m_unitObjectComboLabel
      || !m_unitObjectCombo
      || !m_chartObjectComboLabel
      || !m_chartObjectsCombo
      || !m_settingsComboLabel
      || !m_settingsCombo
      || GetSafeHwnd () == nullptr 
      || (AfxGetMainWnd() != nullptr
         && AfxGetMainWnd()->IsIconic()))
   {
      return;
   }

   CRect rectClient;
   if (rect)
      rectClient = *rect;
   else
      GetClientRect(rectClient);

   int rectCenterHor = rectClient.left + (int)(rectClient.Width() * .5);
   int unitWidth = (int)(rectClient.Width() * .5);
   int vertCur = rectClient.top;
   int horCur = rectClient.left;
   m_viewTypeComboLabel.SetWindowPos(nullptr, horCur, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   m_viewTypeCombo.SetWindowPos(nullptr, rectCenterHor, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   vertCur += m_nComboHeight;
   m_unitObjectComboLabel.SetWindowPos(nullptr, horCur, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   m_unitObjectCombo.SetWindowPos(nullptr, rectCenterHor, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   vertCur += m_nComboHeight;
   m_chartObjectComboLabel.SetWindowPos(nullptr, horCur, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   m_chartObjectsCombo.SetWindowPos(nullptr, rectCenterHor, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   vertCur += m_nComboHeight;
   m_settingsComboLabel.SetWindowPos(nullptr, horCur, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   m_settingsCombo.SetWindowPos(nullptr, rectCenterHor, vertCur, unitWidth, m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);

   int dropDownHeight = 20;
   SetDropDownHeight(&m_viewTypeCombo, dropDownHeight);
   SetDropDownHeight(&m_unitObjectCombo, dropDownHeight);
   SetDropDownHeight(&m_chartObjectsCombo, dropDownHeight);
   SetDropDownHeight(&m_settingsCombo, dropDownHeight);
   
   rectClient.top += 4*m_nComboHeight;
   CPropertiesWndBase::adjustLayout(&rectClient);
}

void CPropertiesWnd::Render(render::iRender* renderer)
{
   _renderer = renderer;
    //if (!m_wndChartObjectsCombo.GetCount())
    //{
       //TODO implement virtual combobox!!!
       //fillChartObjectsIds(renderer->GetStaticIds());
       //_renderer = renderer;
    //}
}

void CPropertiesWnd::fillChartObjectsIds(const std::vector<id_type>& ids)
{
   //std::vector<std::pair<size_t, std::string>> vec; 
   //vec.reserve(ids.size());
   //std::for_each(ids.begin(), ids.end(),  [&vec](const auto& id)
   //{
   //   std::stringstream s; s << id;
   //   vec.emplace_back((size_t)id, s.str() );
   //});
   //
   //m_wndChartObjectsCombo.AddStringVector(std::move(vec));
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CPropertiesWndBase::OnCreate(lpCreateStruct) == -1)
   {
      return -1;
   }
   CRect rectDummy;
   rectDummy.SetRectEmpty();

   // Create combo:
   const DWORD comboDwViewStyle = WS_CHILD | WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWN | /*CBS_SIMPLE | */WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
   const DWORD labelDwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

   CString strViewTypeWnd("View Type: ");
   if (!m_viewTypeComboLabel.Create(strViewTypeWnd, labelDwViewStyle, rectDummy, this, ID_COMBO_VIEW_TYPE_LABEL))
   {
      TRACE0(windowFailureMsg(strViewTypeWnd.GetString()));
      return -1;      // fail to create
   }
   if (!m_viewTypeCombo.Create(comboDwViewStyle, rectDummy, this, ID_COMBO_VIEW_TYPE))
   {
      TRACE0(windowFailureMsg(strViewTypeWnd.GetString()));
      return -1;      // fail to create
   }

   CString strUnitObjectsWnd("Unit: ");
   if (!m_unitObjectComboLabel.Create(strUnitObjectsWnd, labelDwViewStyle, rectDummy, this, ID_COMBO_UNIT_OBJECT_LABEL))
   {
      TRACE0(windowFailureMsg(strUnitObjectsWnd.GetString()));
      return -1;      // fail to create
   }
   if (!m_unitObjectCombo.Create(comboDwViewStyle, rectDummy, this, ID_COMBO_UNIT_OBJECT))
   {
      TRACE0(windowFailureMsg(strUnitObjectsWnd.GetString()));
      return -1;      // fail to create
   }

   CString strChartObjectsWnd("Chart Obj: ");
   if (!m_chartObjectComboLabel.Create(strChartObjectsWnd, labelDwViewStyle, rectDummy, this, ID_COMBO_CHART_OBJECT_LABEL))
   {
      TRACE0(windowFailureMsg(strChartObjectsWnd.GetString()));
      return -1;      // fail to create
   }
   if (!m_chartObjectsCombo.Create(comboDwViewStyle, rectDummy, this, ID_COMBO_CHART_OBJECT))
   {
      TRACE0(windowFailureMsg(strChartObjectsWnd.GetString()));
      return -1;      // fail to create
   }

   CString strSettingsWnd("Settings: ");
   if (!m_settingsComboLabel.Create(strSettingsWnd, labelDwViewStyle, rectDummy, this, ID_COMBO_VIEW_TYPE_LABEL))
   {
      TRACE0(windowFailureMsg(strSettingsWnd.GetString()));
      return -1;      // fail to create
   }
   if (!m_settingsCombo.Create(comboDwViewStyle, rectDummy, this, ID_COMBO_VIEW_TYPE))
   {
      TRACE0(windowFailureMsg(strSettingsWnd.GetString()));
      return -1;      // fail to create
   }
 
   CRect rectCombo;
   m_viewTypeCombo.GetClientRect(&rectCombo);

   m_nComboHeight = rectCombo.Height();

   setFont();
   adjustLayout(0);

   return 0;
}

bool CPropertiesWnd::OnObjectSelected(iProperty* prop)
{
   auto currentObjectType = SelectedObjectManager::GetInstance().GetSelectedObjectType();
   bool fullReload = (currentObjectType != _selectedObjectType) || !prop;

   _selectedObjectType = currentObjectType;

   ShowProperties(prop, fullReload);

   return true;
}
 
/////////////////////////////////////////////////////////////////////////////

void CPropertiesWnd::OnViewTypeComboChanged()
{

}

void CPropertiesWnd::OnUnitObjectComboChanged()
{
   render::find_info info;
   info.id = m_unitObjectCombo.GetItemData(m_unitObjectCombo.GetCurSel());
   auto idxMeta = m_unitObjectIdxMap[info.id];
   switch (idxMeta.second)
   {
      case surface_simulation::UNIT_TYPE::UT_DRONE:
      {
         info.find_object_type = render::FIND_OBJECT_TYPE::FOT_DRONE;
         break;
      }
      case surface_simulation::UNIT_TYPE::UT_ROVER:
      {
         info.find_object_type = render::FIND_OBJECT_TYPE::FOT_ROVER;
         break;
      }
      case surface_simulation::UNIT_TYPE::UT_SHIP:
      {
         info.find_object_type = render::FIND_OBJECT_TYPE::FOT_SHIP;
         break;
      }
   }
   CString rawname;
   m_unitObjectCombo.GetLBText(m_unitObjectCombo.GetCurSel(), rawname);
   std::string name(rawname);
   SelectedObjectManager::GetInstance().Select(info);
   // NOTE: установка фокуса на объект
   for (size_t idx = 0; idx < simulator::getSimulator()->GetState().GetUnitCount(idxMeta.second); ++idx)
   {
      if (simulator::getSimulator()->GetState().GetUnitByIdx(idxMeta.second, idx)->GetInfo().id == info.id)
      {
         CG::geo_point position(simulator::getSimulator()->GetState().GetUnitByIdx(idxMeta.second, idx)->GetPos().pos);
         user_interface::SetCenter(position.lat, position.lon);
         user_interface::InvalidateView();
         return;
      }
   }
}

void CPropertiesWnd::OnChartObjectComboChanged()
{
   render::find_info info;
   info.id = m_chartObjectsCombo.GetItemData(m_chartObjectsCombo.GetCurSel());
   info.find_object_type = render::FIND_OBJECT_TYPE::FOT_CHART_OBJECT;
   SelectedObjectManager::GetInstance().Select(info);
   // NOTE: установка фокуса на объект
   auto* obj = simulator::getSimulator()->GetState().GetChartObjectById(info.id);
   size_t ptCount = obj->GetContourData().size();
   double mult = static_cast<double>(ptCount);
   CG::geo_point position(0., 0.);
   for (auto& contour : obj->GetContourData())
   {
      for (auto& point : contour)
      {
         CG::geo_point nw(point);
         position += nw / mult;
      }
   }
   user_interface::SetCenter(position.lat, position.lon);
   user_interface::InvalidateView();
   return;
   
}

void CPropertiesWnd::OnSettingsComboChanged()
{

}

/////////////////////////////////////////////////////////////////////////////

void CPropertiesWnd::OnViewTypeComboEdited()
{
}

void CPropertiesWnd::OnUnitObjectComboEdited()
{
}

void CPropertiesWnd::OnChartObjectComboEdited()
{
}

void CPropertiesWnd::OnSettingsComboEdited()
{
}

/////////////////////////////////////////////////////////////////////////////

void CPropertiesWnd::reloadViewTypeList()
{
   clearViewTypeList();
   auto* sim = simulator::getSimulator();
   if (!sim)
      return;
   std::vector<std::pair<size_t, std::string>> data;
   for (const auto& elem : viewTypeMeta)
      data.emplace_back(std::pair<size_t, std::string>{ static_cast<size_t>(elem.first), elem.second });
   addViewTypeVector(std::move(data));
}

void CPropertiesWnd::reloadUnitObjectList()
{
   clearUnitList();
   if (!simulator::getSimulator())
      return;
   auto& state = simulator::getSimulator()->GetState();
   size_t count = state.GetUnitCount(surface_simulation::UNIT_TYPE::UT_DRONE)
      + state.GetUnitCount(surface_simulation::UNIT_TYPE::UT_ROVER)
      + state.GetUnitCount(surface_simulation::UNIT_TYPE::UT_SHIP);
   DropDownDataStorage data;
   m_unitObjectIdxMap.clear();
   auto unitFiller = [this, &data, &state](surface_simulation::UNIT_TYPE type)
   {
      for (size_t idx = 0; idx < state.GetUnitCount(type); idx++)
      {
         auto info = state.GetUnitByIdx(type, idx)->GetInfo();
         data.emplace_back(DropDownDataElement(static_cast<size_t>(info.id), info.name));
         m_unitObjectIdxMap[static_cast<size_t>(info.id)] = std::pair<size_t, surface_simulation::UNIT_TYPE>(idx, type);
      }
   };
   unitFiller(surface_simulation::UNIT_TYPE::UT_DRONE);
   unitFiller(surface_simulation::UNIT_TYPE::UT_ROVER);
   unitFiller(surface_simulation::UNIT_TYPE::UT_SHIP);

   std::sort(data.begin(), data.end(), [](std::pair<size_t, std::string> const& a, std::pair<size_t, std::string> const& b) -> bool { return a.first < b.first; });
   addUnitObjectVector(std::move(data));
}

void CPropertiesWnd::reloadChartObjectList()
{
   clearChartList();
   if (!simulator::getSimulator())
      return;
   DropDownDataStorage data;
   auto& state = simulator::getSimulator()->GetState();
   for (size_t idx = 0; idx < state.GetChartObjectCount(); idx++)
   {
      const auto* obj = state.GetChartObjectByIdx(idx);
      auto type = obj->GetType();
      auto id = obj->GetId();
      data.emplace_back(DropDownDataElement(static_cast<size_t>(id), chart_object::chart_obj_type_to_str(type)));
   }

   std::sort(data.begin(), data.end(), [](DropDownDataElement const& a, DropDownDataElement const& b) -> bool { return a.first < b.first; });
   addChartObjectVector(std::move(data));
}

void CPropertiesWnd::reloadSettingsList()
{
   clearSettingsList();
   if (!simulator::getSimulator())
      return;
   std::vector<std::pair<size_t, std::string>> data;
   for (const auto& elem : settingsMeta)
      data.emplace_back(std::pair<size_t, std::string>{ static_cast<size_t>(elem.first), elem.second });
   addSettingsVector(std::move(data));
}

/////////////////////////////////////////////////////////////////////////////

void CPropertiesWnd::clearViewTypeList()
{
   m_viewTypeCombo.ResetContent();
}

void CPropertiesWnd::clearUnitList()
{
   m_unitObjectCombo.ResetContent();
}

void CPropertiesWnd::clearChartList()
{
   m_chartObjectsCombo.ResetContent();
}

void CPropertiesWnd::clearSettingsList()
{
   m_settingsCombo.ResetContent();
}

/////////////////////////////////////////////////////////////////////////////

void CPropertiesWnd::addViewTypeVector(std::vector<std::pair<size_t, std::string>>&& data)
{
   m_viewTypeCombo.AddStringVector(std::move(data));
}

void CPropertiesWnd::addUnitObjectVector(std::vector<std::pair<size_t, std::string>>&& data)
{
   m_unitObjectCombo.AddStringVector(std::move(data));
}

void CPropertiesWnd::addChartObjectVector(std::vector<std::pair<size_t, std::string>>&& data)
{
   m_chartObjectsCombo.AddStringVector(std::move(data));
}

void CPropertiesWnd::addSettingsVector(std::vector<std::pair<size_t, std::string>>&& data)
{
   m_settingsCombo.AddStringVector(std::move(data));
}

/////////////////////////////////////////////////////////////////////////////

void CPropertiesWnd::setFont()
{
   if (m_viewTypeCombo)
      m_viewTypeCombo.SetFont(&getFont());

   if (m_unitObjectCombo)
      m_unitObjectCombo.SetFont(&getFont());

   if (m_chartObjectsCombo)
      m_chartObjectsCombo.SetFont(&getFont());

   if (m_settingsCombo)
      m_settingsCombo.SetFont(&getFont());
}