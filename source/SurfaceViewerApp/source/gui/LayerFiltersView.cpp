
#include "stdafx.h"
#include "MainFrm.h"
#include "LayerFiltersView.h"
#include "Resource.h"
#include "SurfaceViewerGen2.h"


using namespace SV;

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
   friend class DebugFiltersView;

   DECLARE_SERIAL(CClassViewMenuButton)

public:
   CClassViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
   {
   }

   virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
      BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
   {
      pImages = CMFCToolBar::GetImages();

      CAfxDrawState ds;
      pImages->PrepareDrawImage(ds);

      CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

      pImages->EndDrawImage(ds);
   }
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LayerFiltersView::LayerFiltersView() noexcept
{
   m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

LayerFiltersView::~LayerFiltersView()
{
}

BEGIN_MESSAGE_MAP(LayerFiltersView, CDockablePane)
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_WM_CONTEXTMENU()
   ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
   ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
   ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
   ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
   ON_COMMAND(ID_DEBUG_UPDATE, OnUpdateDebug)
   ON_COMMAND(ID_AUTO_UPDATE, OnAutoUpdateDebug)
   ON_WM_PAINT()
   ON_WM_SETFOCUS()
   ON_UPDATE_COMMAND_UI_RANGE(ID_AUTO_UPDATE, ID_AUTO_UPDATE, OnUpdateSort)
   ON_NOTIFY(TVM_SETITEM, 2, &LayerFiltersView::OnTvnItemChangedTree)
   ON_NOTIFY(NM_CLICK, 2, &LayerFiltersView::OnNMClkTree1)
   ON_NOTIFY(NM_DBLCLK, 2, &LayerFiltersView::OnNMDblclkTree1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

void LayerFiltersView::OnTvnItemChangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
   NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
   auto check = m_treeFilters.GetCheck(pNMTVItemChange->hItem);
   // TODO: Add your control notification handler code here
   *pResult = 0;
}


int LayerFiltersView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CDockablePane::OnCreate(lpCreateStruct) == -1)
      return -1;

   CRect rectDummy;
   rectDummy.SetRectEmpty();

   // Create views:
   const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_CHECKBOXES;

   if (!m_treeFilters.Create(dwViewStyle, rectDummy, this, 2))
   {
      TRACE0("Failed to create Class View\n");
      return -1;      // fail to create
   }

   // Load images:
   m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE);
   m_wndToolBar.LoadToolBar(IDR_TIME_SCALE_TOOLBAR, 0, 0, TRUE /* Is locked */);

   //OnChangeVisualStyle();

   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

   m_wndToolBar.SetOwner(this);

   // All commands will be routed via this control , not via the parent frame:
   m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

   CMFCToolBarButton* button = DYNAMIC_DOWNCAST(CMFCToolBarButton, m_wndToolBar.GetButton(1));
   button->SetStyle(TBBS_CHECKBOX);
   fillClassView();

   return 0;
}

void LayerFiltersView::OnSize(UINT nType, int cx, int cy)
{
   CDockablePane::OnSize(nType, cx, cy);
   AdjustLayout();
}

void LayerFiltersView::fillSubFilters(HTREEITEM hParent, const filter_info& filter)
{
   std::unordered_map<std::string, HTREEITEM> currentChilds;

   auto child = m_treeFilters.GetNextItem(hParent, TVGN_CHILD);
   while (child)
   {
      auto curChild = child;
      std::string wname = m_treeFilters.GetItemText(child);

      child = m_treeFilters.GetNextItem(child, TVGN_NEXT);

      if (filter.childs.find(wname) != filter.childs.end())
      {
         //зарегистрируем элемент, чтобы его дальше снова не добавить
         currentChilds.insert_or_assign(wname, curChild);
      }
      else
      {
         //такого айтема уже нет, удалим его
         m_treeFilters.DeleteItem(curChild);
      }
   }

   for (const auto& child : filter.childs)
   {
      HTREEITEM hFilter = nullptr;

      auto itemIt = currentChilds.find(child.second.name);
      if (itemIt == currentChilds.end())
      {
         hFilter = m_treeFilters.InsertItem(child.second.name.c_str(), hParent, 0);
      }
      else
      {
         hFilter = itemIt->second;
      }


      m_treeFilters.SetItemState(hParent, child.second.childs.size() ? TVIS_BOLD : 0, TVIS_BOLD);

      for (const auto& subChild : child.second.childs)
         fillSubFilters(hFilter, child.second);

      //m_treeFilters.Expand(hFilter, TVE_EXPAND);
   }

}

void LayerFiltersView::fillClassView()
{
   const auto& filters = LayerFiltersManager::GetInstance().GetFilters();

   HTREEITEM hRoot = m_treeFilters.GetRootItem() ? m_treeFilters.GetRootItem() : m_treeFilters.InsertItem("DEBUG FILTERS", 0, 0);

   m_treeFilters.LockWindowUpdate();
   fillSubFilters(hRoot, filters);
   m_treeFilters.Expand(hRoot, TVE_EXPAND);
   m_treeFilters.UnlockWindowUpdate();
}

bool LayerFiltersView::OnScenarioScenarioStatusChanged(surface_simulation::SCENARIO_STATUS status)
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
   LayerFiltersManager::GetInstance().Reload();
   return res;
}

bool LayerFiltersView::OnScenarioTimeChanged(double time)
{
   CMFCToolBarButton* button = DYNAMIC_DOWNCAST(CMFCToolBarButton, m_wndToolBar.GetButton(2));
   if (_autoUpdate)
      fillClassView();
   return true;
}

bool LayerFiltersView::onScenarioCheckOpened()
{
   m_treeFilters.DeleteAllItems();
   fillClassView();
   return true;
}

bool LayerFiltersView::onScenarioMapProcessed()
{
   return true;
}

bool LayerFiltersView::onScenarioMapObjProcessed()
{
   return true;
}

bool LayerFiltersView::onScenarioPathFound()
{
   return true;
}

bool LayerFiltersView::onScenarioOptPathFound()
{
   return true;
}

void LayerFiltersView::OnContextMenu(CWnd* pWnd, CPoint point)
{
   CTreeCtrl* pWndTree = (CTreeCtrl*)&m_treeFilters;
   ASSERT_VALID(pWndTree);

   if (pWnd != pWndTree)
   {
      CDockablePane::OnContextMenu(pWnd, point);
      return;
   }

   if (point != CPoint(-1, -1))
   {
      // Select clicked item:
      CPoint ptTree = point;
      pWndTree->ScreenToClient(&ptTree);

      UINT flags = 0;
      HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
      if (hTreeItem != nullptr)
      {
         pWndTree->SelectItem(hTreeItem);
      }
   }

   pWndTree->SetFocus();
   CMenu menu;
   menu.LoadMenu(IDR_POPUP_SORT);

   CMenu* pSumMenu = menu.GetSubMenu(0);

   if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
   {
      CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

      if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
         return;

      ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
      UpdateDialogControls(this, FALSE);
   }
}

void LayerFiltersView::AdjustLayout()
{
   if (GetSafeHwnd() == nullptr)
   {
      return;
   }

   CRect rectClient;
   GetClientRect(rectClient);

   int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

   m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
   m_treeFilters.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL LayerFiltersView::PreTranslateMessage(MSG* pMsg)
{
   return CDockablePane::PreTranslateMessage(pMsg);
}

void LayerFiltersView::OnUpdateSort(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(_autoUpdate);
}

void LayerFiltersView::OnClassAddMemberFunction()
{
   AfxMessageBox(_T("Add member function..."));
}

void LayerFiltersView::OnClassAddMemberVariable()
{
   // TODO: Add your command handler code here
}

void LayerFiltersView::OnClassDefinition()
{
   // TODO: Add your command handler code here
}

void LayerFiltersView::OnClassProperties()
{
   // TODO: Add your command handler code here
}

void LayerFiltersView::OnUpdateDebug()
{
   fillClassView();
}

void LayerFiltersView::OnAutoUpdateDebug()
{
   _autoUpdate = !_autoUpdate;
}

void LayerFiltersView::OnPaint()
{
   CPaintDC dc(this); // device context for painting

   CRect rectTree;
   m_treeFilters.GetWindowRect(rectTree);
   ScreenToClient(rectTree);

   rectTree.InflateRect(1, 1);
   dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void LayerFiltersView::OnSetFocus(CWnd* pOldWnd)
{
   CDockablePane::OnSetFocus(pOldWnd);

   m_treeFilters.SetFocus();
}

void LayerFiltersView::OnChangeVisualStyle()
{
   m_ClassViewImages.DeleteImageList();

   UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

   CBitmap bmp;
   if (!bmp.LoadBitmap(uiBmpId))
   {
      TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
      ASSERT(FALSE);
      return;
   }

   BITMAP bmpObj;
   bmp.GetBitmap(&bmpObj);

   UINT nFlags = ILC_MASK;

   nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

   m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
   m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

   m_treeFilters.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

   m_wndToolBar.CleanUpLockedImages();
   m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}

bool LayerFiltersView::showFilter(HTREEITEM hItem, bool show, bool showParent)
{
   LayerFiltersManager::GetInstance().ShowFilter(getFilterPath(hItem, show, showParent), show);
   return true;
}

void LayerFiltersView::recursive_set_check(HTREEITEM hRoot, bool check)
{
   HTREEITEM hItemChild = m_treeFilters.GetChildItem(hRoot);
   while (hItemChild)
   {
      m_treeFilters.SetCheck(hItemChild, check);
      showFilter(hItemChild, check, false);
      if (m_treeFilters.ItemHasChildren(hItemChild))
      {
         recursive_set_check(hItemChild, check);
      }
      hItemChild = m_treeFilters.GetNextItem(hItemChild, TVGN_NEXT);
   }
}
void LayerFiltersView::OnNMClkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
   UINT nFlags;
   CPoint curPoint;
   GetCursorPos(&curPoint);
   m_treeFilters.ScreenToClient(&curPoint);

   HTREEITEM hItemSel = m_treeFilters.HitTest(curPoint, &nFlags);
   if (TVHT_ONITEMSTATEICON & nFlags)
   {
      auto check = !m_treeFilters.GetCheck(hItemSel);

      showFilter(hItemSel, check, true);
      recursive_set_check(hItemSel, check);


      user_interface::InvalidateView();
      *pResult = 1;
   }

   *pResult = 0;
}

void LayerFiltersView::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
   UINT nFlags;
   CPoint curPoint;
   GetCursorPos(&curPoint);
   m_treeFilters.ScreenToClient(&curPoint);

   HTREEITEM hItemSel = m_treeFilters.HitTest(curPoint, &nFlags);


   user_interface::InvalidateView();
   *pResult = 1;

}