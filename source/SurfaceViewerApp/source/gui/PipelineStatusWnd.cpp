#include "stdafx.h"
#include "PipelineStatusWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

using namespace SV;

BEGIN_MESSAGE_MAP(CPipeplineStatusWnd, CDockablePane)
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_WM_ERASEBKGND()
   ON_WM_SYSCOMMAND()
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()

   ON_NOTIFY(NM_DBLCLK, IDC_LIST_STATUSES, &CPipeplineStatusWnd::OnDoubleClickedDangerList)
   ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STATUSES, &CPipeplineStatusWnd::OnNMCustomDrawList)
END_MESSAGE_MAP()

void CPipeplineStatusWnd::AdjustLayout()
{
   int SHIFT_TOP = 4;      // высота верхней панели
   int SHIFT_LEFT = 4;     // толщина бортика
   int SHIFT_RIGHT = 4;    // ширина правой панели
   int SHIFT_BOTTON = 4;   // высота нижней панели

   CRect winRect, Rect;
   GetClientRect(&winRect);
   Rect = winRect;
   Rect.left += SHIFT_LEFT;
   Rect.top += SHIFT_TOP;
   Rect.right -= SHIFT_RIGHT;
   Rect.bottom -= SHIFT_BOTTON;

   // сдвинули окна
   int DW = Rect.right - Rect.left;
   int DH = Rect.bottom - Rect.top;
   int shipIdWidth = 60;
   int dangerIdWidth = 75;

   m_wndStatuses.MoveWindow(SHIFT_LEFT, SHIFT_TOP, DW, DH, TRUE);
   m_wndStatuses.ResetLayout();
}

/*bool CPipeplineStatusWnd::OnScenarioLoad(const char* name)
{
   clear();
   update_statuses();
   return true;
}*/

bool CPipeplineStatusWnd::OnScenarioModified()
{
   clear();
   update_statuses();
   return true;
}

int CPipeplineStatusWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CDockablePane::OnCreate(lpCreateStruct) == -1)
      return -1;

   CRect rectDummy;
   rectDummy.SetRectEmpty();

   // Create combo:
   const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;
   InitCommonControls();

   if (!m_wndStatuses.Create(WS_CHILD | LVS_REPORT, CRect(0, 0, 200, 200), this, IDC_LIST_STATUSES))
   {
      TRACE0("Failed to create Dangers window\n");
      return FALSE;
   }
   ListView_SetExtendedListViewStyleEx(m_wndStatuses.m_hWnd, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);
   ::ShowWindow(m_wndStatuses, SW_SHOW);

   return 0;
}

HWND CPipeplineStatusWnd::createListView(HWND hWndParent, UINT uId)
{
   INITCOMMONCONTROLSEX icex;
   icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
   icex.dwICC = ICC_LISTVIEW_CLASSES;
   InitCommonControlsEx(&icex);

   RECT rcl;
   ::GetClientRect(hWndParent, &rcl);

   HWND hWndLV = CreateWindow(WC_LISTVIEW, L"",
      WS_CHILD | LVS_REPORT,
      0, 0, rcl.right - rcl.left, rcl.bottom - rcl.top - 50,
      hWndParent, (HMENU)uId, GetModuleHandle(NULL), NULL);

   // Чтобы определялись строка (item) и столбец (subitem) обязательно устанавливаем
   // расширенный стиль LVS_EX_FULLROWSELECT.
   ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

   return (hWndLV);
}

void CPipeplineStatusWnd::OnSize(UINT nType, int cx, int cy)
{
   CDockablePane::OnSize(nType, cx, cy);
   AdjustLayout();
}

BOOL CPipeplineStatusWnd::OnEraseBkgnd(CDC* pDC)
{
   return CWnd::OnEraseBkgnd(pDC);
}

bool CPipeplineStatusWnd::OnObjectSelected(iPropertyInterface* prop)
{
   return true;
}

bool CPipeplineStatusWnd::OnScenarioTimeChanged(double time)
{
   update_statuses();
   return true;
}

/*bool CPipeplineStatusWnd::OnScenarioStatusChanged(CSENARIO_STATUS status)
{
   return true;
}*/

void CPipeplineStatusWnd::clear()
{}

void CPipeplineStatusWnd::update_statuses()
{
   //for (auto& rec : m_records)
      //rec.second.status = RS_OLD;
   std::vector<decltype(m_records)::key_type> toRemove;
   for (auto& rec : m_records)
   {
      // тут не удаляется item, т.к. он еще задействован в таблице и удалится автоматически после
      if (rec.second.status == RS_OLD)
         toRemove.emplace_back(rec.first);
   }
   for (auto& key : toRemove)
      m_records.erase(key);
   m_wndStatuses.LoadData();
}

void CPipeplineStatusWnd::OnDoubleClickedDangerList(NMHDR* pNMHDR, LRESULT* pResult)
{
   user_interface::InvalidateView();
}

void CPipeplineStatusWnd::OnNMCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
   CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST_STATUSES);
   NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
   *pResult = CDRF_DODEFAULT;
   if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
      *pResult = CDRF_NOTIFYITEMDRAW;

   else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
   {
      COLORREF crText = RGB(0, 0, 0);
      PipelineStatusItemData* data = (PipelineStatusItemData*)m_wndStatuses.GetItemData(pLVCD->nmcd.dwItemSpec);
      /*const colreg::SOLUTION_TYPE solutionType = data->solution;
      if (solutionColor.find(solutionType) != solutionColor.end())
         crText = solutionColor.at(solutionType);*/
      //pLVCD->clrText = crText;
      *pResult = CDRF_DODEFAULT;
   }
}