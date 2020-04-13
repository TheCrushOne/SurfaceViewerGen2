#include "stdafx.h"
#include "ProgressView.h"
#include "Resource.h"
#include "MainFrm.h"
//#include "colreg\Simulator.h"

#include <windows.h>
#include <commctrl.h>
#include "resource.h"

BEGIN_MESSAGE_MAP(CProgressViewWnd, CDockablePane)
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_WM_ERASEBKGND()

   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIMULATION_SLIDER, &CProgressViewWnd::OnReleasedCaptureSlider)
   ON_BN_CLICKED(IDC_SIMULATION_TIME_SET_BUTTON, &CProgressViewWnd::OnClickedSetButton)
   ON_UPDATE_COMMAND_UI(IDC_SIMULATION_TIME_SET_BUTTON, &CProgressViewWnd::OnUpdateSetButton)
END_MESSAGE_MAP()

void CProgressViewWnd::AdjustLayout()
{
   CRect Rect;
   GetWindowRect(&Rect);

   int DW = Rect.right - Rect.left;
   int DH = 35;

   SetWindowPos(nullptr, Rect.left, Rect.top, DW, DH, SWP_NOACTIVATE | SWP_NOZORDER);

   ShowPane(active, false, false);

   adjustElementLayout();
}

void CProgressViewWnd::adjustElementLayout()
{
   CRect Rect;
   GetWindowRect(&Rect);

   const int DW = Rect.Width();
   const int DH = Rect.Height();

   GetDlgItem(IDC_SIMULATION_SLIDER)->SetWindowPos(nullptr, 0, 0, (int)(DW * .5), DH, SWP_NOACTIVATE | SWP_NOZORDER);
   GetDlgItem(IDC_SIMULATION_TIME)->SetWindowPos(nullptr, (int)(DW * .5), 0, (int)(DW * .25), DH, SWP_NOACTIVATE | SWP_NOZORDER);
   GetDlgItem(IDC_SIMULATION_TIME_SET_BUTTON)->SetWindowPos(nullptr, (int)(DW * 3. / 4.), 0, (int)(DW * .25), DH, SWP_NOZORDER);
}

ColregSimulation::iSimulator* CProgressViewWnd::getSimulator()
{
   auto* sim = simulator::getSimulator();
   ATLASSERT(sim);
   return sim;
}

bool CProgressViewWnd::OnScenarioLoad()
{
   active = true;//getSimulator()->GetSimulationType() == ColregSimulation::SIMULATION_PLAYER_TYPE::SPT_LOG;
   if (active)
   {
      ShowPane(true, false, false);
      CSliderCtrl* slider = ((CSliderCtrl*)GetDlgItem(IDC_SIMULATION_SLIDER));
      /*slider->SetPos(getSimulator()->GetCurrentControlPointIdx());
      slider->SetRange(0, getSimulator()->GetControlPointsCount());*/
      slider->SetTicFreq(1);
      slider->Invalidate();
   }
   else
      ShowPane(false, false, false);
   return true;
}

bool CProgressViewWnd::OnScenarioPathFound()
{
   if (active)
   {
      CSliderCtrl* slider = ((CSliderCtrl*)GetDlgItem(IDC_SIMULATION_SLIDER));
      slider->SetPos(getSimulator()->GetCurrentControlPointIdx());
      slider->SetRange(0, getSimulator()->GetControlPointsCount());
      slider->Invalidate();
   }
   return true;
}


bool CProgressViewWnd::OnScenarioTimeChanged(double time)
{
   if (active)
   {
      ((CSliderCtrl*)GetDlgItem(IDC_SIMULATION_SLIDER))->SetPos(getSimulator()->GetCurrentControlPointIdx());
      CTime ctime = timestampToTime(time);
      ((CDateTimeCtrl*)GetDlgItem(IDC_SIMULATION_TIME))->SetTime(&ctime);
   }
   return true;
}

int CProgressViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CDockablePane::OnCreate(lpCreateStruct) == -1)
      return -1;

   InitCommonControls();
   HWND hwnd = this->GetSafeHwnd();
   m_slider = createSlider(hwnd, IDC_SIMULATION_SLIDER);
   m_editbox = createEditBox(hwnd, IDC_SIMULATION_TIME);
   m_button = createButton(hwnd, IDC_SIMULATION_TIME_SET_BUTTON);

   ModifyStyle(WS_SYSMENU, 0, SWP_FRAMECHANGED);
   active = false;
   return 0;
}

HWND CProgressViewWnd::createSlider(HWND hWndParent, UINT uId)
{
   CRect rcl;
   ::GetClientRect(hWndParent, &rcl);

   HWND hWndLV = CreateWindow(L"msctls_trackbar32", L"",
      WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | TBS_AUTOTICKS,
      0, 0, (int)(rcl.Width() * 0.5), rcl.Height(),
      hWndParent, (HMENU)uId, GetModuleHandle(NULL), NULL);

   return hWndLV;
}

HWND CProgressViewWnd::createEditBox(HWND hWndParent, UINT uId)
{
   CRect rcl;
   ::GetClientRect(hWndParent, &rcl);

   HWND hWndLV = CreateWindow(L"SysDateTimePick32", L"",
      DTS_RIGHTALIGN | WS_TABSTOP | WS_CHILD | WS_BORDER | WS_VISIBLE,
      (int)(rcl.Width() * .25), 0, (int)(rcl.Height() * .25), rcl.Height(),
      hWndParent, (HMENU)uId, GetModuleHandle(NULL), NULL);
   ((CDateTimeCtrl*)GetDlgItem(IDC_SIMULATION_TIME))->SetFormat(L"dd-MM-yy HH:mm:ss");
   return hWndLV;
}

HWND CProgressViewWnd::createButton(HWND hWndParent, UINT uId)
{
   CRect rcl;
   ::GetClientRect(hWndParent, &rcl);

   HWND hWndLV = CreateWindow(L"BUTTON", L"Set",
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      (int)(rcl.Width() * 3. / 4.), 0, (int)(rcl.Width() * .25), rcl.Height(),
      hWndParent, (HMENU)uId, GetModuleHandle(NULL), NULL);

   return hWndLV;
}

void CProgressViewWnd::OnSize(UINT nType, int cx, int cy)
{
   CDockablePane::OnSize(nType, cx, cy);
   adjustElementLayout();
}

BOOL CProgressViewWnd::OnEraseBkgnd(CDC* pDC)
{
   return CWnd::OnEraseBkgnd(pDC);
}

bool CProgressViewWnd::OnObjectSelected(iProperty* prop)
{
   return true;
}

inline CTime CProgressViewWnd::timestampToTime(double stamp)
{
   time_t ctime = (time_t)stamp;
   CTime time(ctime);
   return time;
}

void CProgressViewWnd::OnReleasedCaptureSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
   // TODO: Add your control notification handler code here
   CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(pNMHDR->idFrom);
   auto sim = getSimulator();
   if (!sim->PlayFrom(slider->GetPos()))
   {
      AfxMessageBox(L"Error! Play from isn`t supported or simulation have failed!");
      return;
   }
   simulator::simulatorStep();
   CTime time = timestampToTime(sim->GetState().GetTime());
   ((CDateTimeCtrl*)GetDlgItem(IDC_SIMULATION_TIME))->SetTime(&time);
   *pResult = 0;
   user_interface::InvalidateView();
}

inline double CProgressViewWnd::timeToTimestamp(CTime time)
{
   return (double)time.GetTime();
}

void CProgressViewWnd::OnClickedSetButton()
{
   CTime time;
   auto sim = simulator::getSimulator();
   ATLASSERT(sim);
   ((CDateTimeCtrl*)GetDlgItem(IDC_SIMULATION_TIME))->GetTime(time);
   double timeVal = timeToTimestamp(time);
   for (size_t iControlPnt = 0; iControlPnt < sim->GetControlPointsCount(); ++iControlPnt)
   {
      const ColregSimulation::control_point_info& ctrlPt = sim->GetControlPointInfo(iControlPnt);
      if (ctrlPt.time > timeVal)
      {
         if (!sim->PlayFrom(iControlPnt))
         {
            AfxMessageBox(L"Error! Play from isn`t supported or simulation have failed!");
            return;
         }
         ((CSliderCtrl*)GetDlgItem(IDC_SIMULATION_SLIDER))->SetPos(iControlPnt);
         simulator::simulatorStep();
         user_interface::InvalidateView();
         return;
      }
   }
}

void CProgressViewWnd::OnUpdateSetButton(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(true);
}