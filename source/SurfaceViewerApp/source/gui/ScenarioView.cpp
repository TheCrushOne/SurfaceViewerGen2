#include "stdafx.h"

#include "ScenarioView.h"

#include "Render\GDIRender.h"
#include "Layers\UnitLayer.h"
#include "Layers\NetLayer.h"
//#include "Layers\NaveStateLayer.h"
#include "MainFrm.h"
#include "simulator\simulator.h"
//#include "Selection\SelectetObjectManager.h"
#include "scenario\ScenarioManager.h"


namespace
{
   ScenarioView* pView = nullptr;
   user_interface::EDIT_MODE s_mode = user_interface::EDIT_MODE::EM_DRAG;
}

void user_interface::InvalidateView()
{
   pView->InvalidateViewRequest();
}

void user_interface::SetCenter(double lt, double ln)
{
   pView->GetRenderer()->SetCenter(math::geo_point{ lt, ln });
}

void user_interface::ShowToolTip(const wchar_t* title, const wchar_t* description)
{
   pView->ShowToolTip(title, description);
}

void user_interface::HideToolTip()
{
   pView->HideToolTip();
}

user_interface::objects_to_draw user_interface::GetObjectsInsideScreen()
{
   /*const auto* sim = simulator::getSimulator();
   if (!sim)
      return {};
   const auto& simulationState = sim->GetState();

   std::vector<colreg::geo_point> ships;
   for (size_t iShip = 0; iShip < simulationState.GetShipCount(); ++iShip)
   {
      const auto& ship = simulationState.GetShip(iShip);
      const auto& center = ship.GetPos().point.pos;
      if (pView->GetRenderer()->IsNeedRender({ center }))
         ships.emplace_back(center);
   }*/
   return { /*ships, pView->GetRenderer()->GetObjectsInsideScreenPts()*/ };
}

void user_interface::SetEditMode(user_interface::EDIT_MODE mode, unsigned long long int userData)
{
   s_mode = mode;
   pView->SetMouseTool(mode, userData);
}

user_interface::EDIT_MODE user_interface::GetEditMode()
{
   return s_mode;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColregTestAppView

IMPLEMENT_DYNCREATE(ScenarioView, CView)

BEGIN_MESSAGE_MAP(ScenarioView, CView)
   // Standard printing commands
   ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ScenarioView::OnFilePrintPreview)
   ON_WM_CONTEXTMENU()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_RBUTTONDOWN()
   ON_WM_RBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_MOUSEHWHEEL()
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_WM_TIMER()
   ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void ScenarioView::InvalidateViewRequest()
{
   _needInvalidate = true;
}

ScenarioView::ScenarioView() noexcept
{
   // TODO: add construction code here

}

ScenarioView::~ScenarioView()
{
}

BOOL ScenarioView::PreTranslateMessage(MSG* pMsg)
{
   switch (pMsg->message)
   {
   case WM_MOUSEWHEEL:
   {
      CPoint curPoint;
      GetCursorPos(&curPoint);
      ScreenToClient(&curPoint);
      OnMouseHWheel(LOWORD(pMsg->wParam), HIWORD(pMsg->wParam), curPoint);
      break;
   }

   case  WM_MOUSEMOVE:
   {
      _tipCtrl->RelayEvent(pMsg);
      break;
   }
   case  WM_KEYDOWN:
   case  WM_CHAR:
   {

      break;
   }
   }

   return CView::PreTranslateMessage(pMsg);
}


BOOL ScenarioView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CView::PreCreateWindow(cs);
}
int ScenarioView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CView::OnCreate(lpCreateStruct) == -1)
      return -1;

   _renderer = std::make_unique<GDIRender>();
   _renderer->Init(m_hWnd);
   CRect rect;
   GetClientRect(rect);

   //_renderer->SetScale(30);
   _renderer->SetScale(300);
   _renderer->SetCenter(math::geo_point{ 0, 0 });

   _layers = std::make_unique<LayersContainer>();

   static CMFCToolTipInfo params;

   params.m_bBoldLabel = TRUE;
   params.m_bDrawDescription = TRUE;
   params.m_bDrawIcon = TRUE;
   params.m_bRoundedCorners = TRUE;
   params.m_bDrawSeparator = TRUE;
   params.m_bBalloonTooltip = TRUE;
   params.m_clrFill = RGB(235, 245, 30);
   params.m_bVislManagerTheme = FALSE;
   params.m_clrFillGradient = RGB(255, 255, 255);

   _tipCtrl = new CMFCToolTipCtrl(&params);
   _tipCtrl->Create(this);
   static CFont  font;
   font.CreateFont(15, 10, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, L"Courier New");
   _tipCtrl->SetFont(&font);
   _tipCtrl->SetDelayTime(1000);

   SetMouseTool(user_interface::EDIT_MODE::EM_DRAG);
   pView = this;
   SetTimer(1, 10, NULL);
   return 0;
}

void ScenarioView::SetMouseTool(user_interface::EDIT_MODE mode, unsigned long long int userData)
{
   _mouseTool = create_mouse_tool(mode, _renderer.get(), userData);
}

void ScenarioView::ShowToolTip(const wchar_t* title, const wchar_t* description)
{
   _tipCtrl->AddTool(this, description);

   HICON hIco = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

   _tipCtrl->SetTitle((UINT)hIco, title);
   _tipCtrl->SetDescription(description);
   _tipCtrl->Activate(TRUE);
}

void ScenarioView::OnSize(UINT nType, int cx, int cy)
{
   _renderer->SetSize(cx, cy);
   CView::OnSize(nType, cx, cy);
}

// CColregTestAppView drawing

void ScenarioView::OnDraw(CDC* pDC)
{
   if (!_renderer)
      return;
   _layers->RenderLayers(_renderer.get());
   _renderer->Render((size_t)pDC);
}

BOOL ScenarioView::OnEraseBkgnd(CDC* pDC)
{
   return TRUE;
}


void ScenarioView::OnMouseMove(UINT nFlags, CPoint point)
{
   _mouseTool->MouseMove(point);

}

void ScenarioView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint point)
{
   _mouseTool->MouseWheele(zDelta, point);
}

void ScenarioView::OnLButtonDown(UINT /* nFlags */, CPoint point)
{
   SetFocus();
   SetActiveWindow();
   _mouseTool->LmouseDown(point);
}

void ScenarioView::OnRButtonDown(UINT /* nFlags */, CPoint point)
{
   _mouseTool->RmouseDown(point);
}

void ScenarioView::OnLButtonUp(UINT /* nFlags */, CPoint point)
{
   _mouseTool->LmouseUp(point);
}

void ScenarioView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
   _mouseTool->RmouseUp(point);
   ClientToScreen(&point);
   OnContextMenu(this, point);
}


bool ScenarioView::OnScenarioLoad(file_utils::sqlite_database_file_storage& name)
{
   _renderer->Clear();
   math::geo_point center = simulator::getCenter();
   if (!center.lat && !center.lon)
   {
      _needResetCenter = true;
   }
   else
   {
      _needResetCenter = false;
      _renderer->SetCenter(center);
   }

   RedrawWindow();
   return true;
}

bool ScenarioView::OnScenarioModified()
{
   _renderer->Clear();
   return true;
}

void ScenarioView::setTimer()
{
   SetTimer(0, 1000 / ScenarioManager::GetInstance().GetTimeScale(), NULL);
}

bool ScenarioView::OnScenarioStatusChanged(CSENARIO_STATUS status)
{
   switch (status)
   {
   case CSENARIO_STATUS::SS_RUN:
      setTimer();
      break;
   case CSENARIO_STATUS::SS_PAUSE:
      KillTimer(0);
      break;
   case CSENARIO_STATUS::SS_STOP:
      KillTimer(0);
      break;
   }

   InvalidateView();
   return true;
}

void ScenarioView::OnTimer(UINT_PTR nIDEvent)
{
   if (nIDEvent == 0)
   {
      ScenarioManager::GetInstance().Step();

      if (_needResetCenter)
      {
         math::geo_point center = simulator::getCenter();
         if (center.lat || center.lon)
         {
            _needResetCenter = false;
            _renderer->SetCenter(center);
         }
      }
   }
   else
   {
      if (_needInvalidate)
      {
         _needInvalidate = false;
         InvalidateView();
      }
   }
}


void ScenarioView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{

}


void ScenarioView::OnFilePrintPreview()
{

}

BOOL ScenarioView::OnPreparePrinting(CPrintInfo* pInfo)
{
   // default preparation
   return DoPreparePrinting(pInfo);
}

void ScenarioView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
   // TODO: add extra initialization before printing
}

void ScenarioView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
   // TODO: add cleanup after printing
}