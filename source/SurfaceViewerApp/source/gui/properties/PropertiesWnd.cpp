
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




#define ID_COMBO 124
#define ID_COMBO_CHART 125

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar


BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_CBN_SELENDOK(ID_COMBO, OnComboChanged)
   ON_CBN_SELENDOK(ID_COMBO_CHART, OnComboChartChanged)
   ON_CBN_EDITUPDATE(ID_COMBO, OnComboEdited)
   ON_CBN_DROPDOWN(ID_COMBO, reloadUnitList)
   ON_CBN_DROPDOWN(ID_COMBO_CHART, onChartCombopDrop)
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
	if (!m_wndShipsCombo || !m_wndChartObjectsCombo || GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
   if (rect)
   {
      rectClient = *rect;
   }
   else
   {
      GetClientRect(rectClient);
   }
	

   m_wndShipsCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, (int)(rectClient.Width()*.5), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   m_wndChartObjectsCombo.SetWindowPos(nullptr, (int)(rectClient.right - rectClient.Width()*.5), rectClient.top, (int)(rectClient.Width()*.5), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
   SetDropDownHeight(&m_wndShipsCombo, 20);
   SetDropDownHeight(&m_wndChartObjectsCombo, 20);
   
   rectClient.top += m_nComboHeight;
   CPropertiesWndBase::adjustLayout(&rectClient);
}

void CPropertiesWnd::Render(render::iRender*renderer)
{
    if (!m_wndChartObjectsCombo.GetCount())
    {
       //TODO implement virtual combobox!!!
       //fillChartObjectsIds(renderer->GetStaticIds());
       _renderer = renderer;
    }
}

void CPropertiesWnd::fillChartObjectsIds(const std::vector<colreg::id_type>& ids)
{
   std::vector<std::pair<size_t, std::string>> vec; 
   vec.reserve(ids.size());
   std::for_each(ids.begin(), ids.end(),  [&vec](const auto& id)
   {
      std::stringstream s; s << id;
      vec.emplace_back((size_t)id, s.str() );
   });
   
   m_wndChartObjectsCombo.AddStringVector(std::move(vec));
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
	const DWORD dwViewStyle = WS_CHILD | WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWN | /*CBS_SIMPLE | */WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndShipsCombo.Create(dwViewStyle, rectDummy, this, ID_COMBO))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}
   if (!m_wndChartObjectsCombo.Create(dwViewStyle, rectDummy, this, ID_COMBO_CHART))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}
 
   
	CRect rectCombo;
	m_wndShipsCombo.GetClientRect (&rectCombo);

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

 
void CPropertiesWnd::clearUnitList()
{
   m_wndShipsCombo.ResetContent();
   //m_wndChartObjectsCombo.ResetContent();
}

void CPropertiesWnd::addUnitVector(std::vector<std::pair<size_t, std::string>> &&data)
{  
   m_wndShipsCombo.AddStringVector(std::move(data));
}

void CPropertiesWnd::OnComboChanged()
{
   render::find_info info;
   info.id = m_wndShipsCombo.GetItemData(m_wndShipsCombo.GetCurSel());
   auto idxMeta = m_unitIdxMap[info.id];
   switch (idxMeta.second)
   {
      case ColregSimulation::UNIT_TYPE::UT_DRONE:
      {
         info.find_object_type = render::FIND_OBJECT_TYPE::FOT_DRONE;
         break;
      }
      case ColregSimulation::UNIT_TYPE::UT_ROVER:
      {
         info.find_object_type = render::FIND_OBJECT_TYPE::FOT_ROVER;
         break;
      }
      case ColregSimulation::UNIT_TYPE::UT_SHIP:
      {
         info.find_object_type = render::FIND_OBJECT_TYPE::FOT_SHIP;
         break;
      }
   }
   CString rawname;
   m_wndShipsCombo.GetLBText(m_wndShipsCombo.GetCurSel(), rawname);
   std::wstring name(rawname);
   SelectedObjectManager::GetInstance().Select(info);
   // NOTE: установка фокуса на объект
   for (size_t idx = 0; idx < simulator::getSimulator()->GetState().GetUnitCount(idxMeta.second); ++idx)
   {
      if (simulator::getSimulator()->GetState().GetUnitByIdx(idxMeta.second, idx)->GetInfo().id == info.id)
      {
         colreg::geo_point position(simulator::getSimulator()->GetState().GetUnitByIdx(idxMeta.second, idx)->GetPos().point.pos);
         user_interface::SetCenter(position.lat, position.lon);
         user_interface::InvalidateView();
         return;
      }
   }
}

void CPropertiesWnd::OnComboChartChanged()
{
   const auto chartId = m_wndChartObjectsCombo.GetItemData(m_wndChartObjectsCombo.GetCurSel());
   if (_renderer)
   {
      //SelectedObjectManager::GetInstance().Select({ render::FIND_TYPE::FT_FIND_DETAILED, chartId, render::FIND_OBJECT_TYPE::FOT_ROUTE_POINT, 0,  colreg::OBJECT_TYPE::OT_DEPTH_AREA });
      //const auto p = _renderer->GetStaticCenterId(chartId);
      //ATLASSERT(p);
      //if (p)
      //{
      //   _renderer->SetCenter(*p);
      //   user_interface::InvalidateView();
      //}
   }
}

void CPropertiesWnd::OnComboEdited()
{
}

void CPropertiesWnd::onChartCombopDrop()
{
   CString str;
   m_wndChartObjectsCombo.GetWindowText(str);
   m_wndChartObjectsCombo.AddString(str);
   std::string num = SVGUtils::wstringToString(std::wstring(str));
   m_wndChartObjectsCombo.SetItemData(m_wndChartObjectsCombo.GetCount() - 1, atoi(num.c_str()));
}

void CPropertiesWnd::reloadUnitList()
{
   clearUnitList();
   if (!simulator::getSimulator())
      return;
   auto& state = simulator::getSimulator()->GetState();
   size_t count = state.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_DRONE)
               + state.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_ROVER)
               + state.GetUnitCount(ColregSimulation::UNIT_TYPE::UT_SHIP);
   std::vector<std::pair<size_t, std::string>> data;
   m_unitIdxMap.clear();
   auto unitFiller = [this, &data, &state](ColregSimulation::UNIT_TYPE type)
   {
      for (size_t idx = 0; idx < state.GetUnitCount(type); idx++)
      {
         auto info = state.GetUnitByIdx(type, idx)->GetInfo();
         data.emplace_back(static_cast<size_t>(info.id), info.name);
         m_unitIdxMap[static_cast<size_t>(info.id)] = std::pair<size_t, ColregSimulation::UNIT_TYPE>( idx, type );
      }
   };
   unitFiller(ColregSimulation::UNIT_TYPE::UT_DRONE);
   unitFiller(ColregSimulation::UNIT_TYPE::UT_ROVER);
   unitFiller(ColregSimulation::UNIT_TYPE::UT_SHIP);
   data.reserve(count);

   std::sort(data.begin(), data.end(), [](std::pair<size_t, std::string> const & a, std::pair<size_t, std::string> const & b) -> bool { return a.first < b.first; });
   addUnitVector(std::move(data));
}


void CPropertiesWnd::setFont()
{   
   if (m_wndShipsCombo)
   {
      m_wndShipsCombo.SetFont(&getFont());
   }
   
   if (m_wndChartObjectsCombo)
   {
      m_wndChartObjectsCombo.SetFont(&getFont());
   }
}