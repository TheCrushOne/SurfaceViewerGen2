#include "stdafx.h"
#include "PropertiesWndBase.h"
#include "resource.h"
#include "gui/MainFrm.h"
//#include "ColregTestApp.h"

#define ID_PROPS 123


BEGIN_MESSAGE_MAP(CPropertiesWndBase, CDockablePane)
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
   ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
   ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
   
   ON_WM_SETFOCUS()
   ON_WM_SETTINGCHANGE()
   
END_MESSAGE_MAP()

void CPropertiesWndBase::adjustLayout(LPRECT rect)
{
   if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
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


   int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

   m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top , rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
   m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - (0 + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}



int CPropertiesWndBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   
   if (CDockablePane::OnCreate(lpCreateStruct) == -1)
      return -1;

   CRect rectDummy;
   rectDummy.SetRectEmpty();

   // Create combo:
   const DWORD dwViewStyle = WS_CHILD | WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWN | /*CBS_SIMPLE | */WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;


   if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, ID_PROPS))
   {
      TRACE0("Failed to create Properties Grid \n");
      return -1;      // fail to create
   }

   InitPropList();

   m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
   m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
   m_wndToolBar.CleanUpLockedImages();
   //m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
   m_wndToolBar.SetOwner(this);

   // All commands will be routed via this control , not via the parent frame:
   m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

   CString s1;
    GetWindowText(s1);
    CString s2;
    m_wndPropList.GetWindowText(s2);
   adjustLayout(0);
   return 0;
}




void CPropertiesWndBase::OnSize(UINT nType, int cx, int cy)
{
   CDockablePane::OnSize(nType, cx, cy);
   adjustLayout(0);
}

void CPropertiesWndBase::OnExpandAllProperties()
{
   m_wndPropList.ExpandAll();
}

void CPropertiesWndBase::OnSortProperties()
{
   m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWndBase::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWndBase::InitPropList()
{
   setFont();
   m_wndPropList.EnableHeaderCtrl(FALSE);
   m_wndPropList.EnableDescriptionArea();
   m_wndPropList.SetVSDotNetLook();
   m_wndPropList.MarkModifiedProperties();
}

void CPropertiesWndBase::OnSetFocus(CWnd* pOldWnd)
{
   CDockablePane::OnSetFocus(pOldWnd);
   m_wndPropList.SetFocus();
}

void CPropertiesWndBase::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
   CDockablePane::OnSettingChange(uFlags, lpszSection);
   setFont();
}


void CPropertiesWndBase::setFont()
{
   ::DeleteObject(m_fntPropList.Detach());

   LOGFONT lf;
   afxGlobalData.fontRegular.GetLogFont(&lf);

   NONCLIENTMETRICS info;
   info.cbSize = sizeof(info);

   afxGlobalData.GetNonClientMetrics(info);

   lf.lfHeight = info.lfMenuFont.lfHeight;
   lf.lfWeight = info.lfMenuFont.lfWeight;
   lf.lfItalic = info.lfMenuFont.lfItalic;

   m_fntPropList.CreateFontIndirect(&lf);

   m_wndPropList.SetFont(&m_fntPropList);
   
}

