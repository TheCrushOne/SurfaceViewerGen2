
#include "stdafx.h"
#include "framework.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "common/utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd() noexcept
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
   ON_WM_CREATE()
   ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CDockablePane::OnCreate(lpCreateStruct) == -1)
      return -1;

   CRect rectDummy;
   rectDummy.SetRectEmpty();

   // Create tabs window:
   if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
   {
      TRACE0("Failed to create output tab window\n");
      return -1;      // fail to create
   }

   // Create output panes:
   const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_READONLY | ES_AUTOHSCROLL | ES_MULTILINE;

   for (size_t i = 0; i < user_interface::OT_SIZE; ++i)
   {
      if (!m_wndOutput[i].Create(dwStyle, rectDummy, &m_wndTabs, i + 2))
      {
         TRACE0("Failed to create output windows\n");
         return -1;      // fail to create
      }
   }
   UpdateFonts();

   CString strTabName;

   for (size_t i = 0; i < user_interface::OT_SIZE; ++i)
   {
      strTabName.LoadString(IDS_INFO_TAB + i);
      m_wndTabs.AddTab(&m_wndOutput[i], strTabName, (UINT)1);
   }

   return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
   CDockablePane::OnSize(nType, cx, cy);

   // Tab control should cover the whole client area:
   m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
   CClientDC dc(this);
   CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

   int cxExtentMax = 0;

   for (int i = 0; i < wndListBox.GetCount(); i++)
   {
      CString strItem;
      wndListBox.GetText(i, strItem);

      cxExtentMax = std::max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
   }

   wndListBox.SetHorizontalExtent(cxExtentMax);
   dc.SelectObject(pOldFont);
}


void COutputWnd::UpdateFonts()
{
   for (size_t i = 0; i < user_interface::OT_SIZE; ++i)
      m_wndOutput[i].SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList() noexcept
{

}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
   ON_WM_CONTEXTMENU()
   ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
   ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
   ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
   ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
   CMenu menu;
   menu.LoadMenu(IDR_OUTPUT_POPUP);

   CMenu* pSumMenu = menu.GetSubMenu(0);

   if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
   {
      CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

      if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
         return;

      ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
      UpdateDialogControls(this, FALSE);
   }

   SetFocus();
}

void COutputList::OnEditCopy()
{
   MessageBox(_T("Copy output"));
}

void COutputList::OnEditClear()
{
   MessageBox(_T("Clear output"));
}

void COutputList::OnViewOutput()
{
   CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
   CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

   if (pMainFrame != nullptr && pParentBar != nullptr)
   {
      pMainFrame->SetFocus();
      pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
      pMainFrame->RecalcLayout();
   }
}

void COutputWnd::SetOutputText(user_interface::OUTPUT_TYPE mode, const char* text, COLORREF color, bool bold, bool italic)
{
   CHARFORMAT cf = { 0 };
   cf.cbSize = sizeof(cf);
   cf.dwMask = (bold ? CFM_BOLD : 0) | (italic ? CFM_ITALIC : 0) | CFM_COLOR;
   cf.dwEffects = (bold ? CFE_BOLD : 0) | (italic ? CFE_ITALIC : 0) | ~CFE_AUTOCOLOR;
   cf.crTextColor = color;
   auto& wnd = m_wndOutput[mode];

   const auto textSize = strlen(text);
   int len = wnd.GetTextLength();
   wnd.SetSel(len, -1);
   std::wstring str = SVGUtils::stringToWstring(text);
   wnd.ReplaceSel(str.c_str());
   len = wnd.GetTextLength();
   const auto lineCount = wnd.GetLineCount();
   wnd.SetSel(len - textSize - lineCount, len - lineCount);
   wnd.SetSelectionCharFormat(cf);
   wnd.SetSel(wnd.GetTextLength(), -1);
   wnd.ReplaceSel(L"\r\n");
}
