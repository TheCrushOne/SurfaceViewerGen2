#include "stdafx.h"
#include "framework.h"
#include "mainfrm.h"
#include "AppStatusView.h"
#include "Resource.h"
#include "SurfaceViewerGen2.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CAppStatusView* view = nullptr;

void user_interface::SetProgress(unsigned int progress)
{
	view->SetProgress(progress);
}

/////////////////////////////////////////////////////////////////////////////
// CAppStatusView

CAppStatusView::CAppStatusView() noexcept
{
	view = this;
}

CAppStatusView::~CAppStatusView()
{
}

BEGIN_MESSAGE_MAP(CAppStatusView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()

	ON_NOTIFY(TVN_ENDLABELEDIT, 4, OnEndlabeledit)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, 4, &CAppStatusView::OnNMDblclkTree1)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// обработчики сообщений CWorkspaceBar

int CAppStatusView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;

	if (!m_overallProgress.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Ќе удалось создать представление файлов\n");
		return -1;      // не удалось создать
	}
	m_overallProgress.SetWindowTextW(L"Overall Progress");
	m_overallProgress.SetRange(0, 100);
	m_overallProgress.SetStep(5);
	m_overallProgress.SetPos(55);
	m_overallProgress.StepIt();

	// «агрузить изображени€ представлени€:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* «аблокирован */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ¬се команды будут перенаправлены через этот элемент управлени€, а не через родительскую рамку:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ¬ведите некоторые данные статического представлени€ в виде дерева (пустой код, ничего более)
	AdjustLayout();

	return 0;
}

void CAppStatusView::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	*pResult = 1;
}

void CAppStatusView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CAppStatusView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
}

void CAppStatusView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_overallProgress.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.top + 2*cyTlb + 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CAppStatusView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM newitem;
	newitem = pTVDispInfo->item;
	if (newitem.pszText == 0)
	{
		*pResult = 0;
		return;
	}
	*pResult = 0;
	newitem.mask = TVIF_TEXT;
}

void CAppStatusView::OnPaint()
{
	CPaintDC dc(this); // контекст устройства дл€ рисовани€

	CRect rectTree;
	m_overallProgress.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CAppStatusView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_overallProgress.SetFocus();
}

void CAppStatusView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* «аблокирован */);

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Ќе удаетс€ загрузить точечный рисунок: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;
}

void CAppStatusView::SetProgress(unsigned int progress)
{
	m_overallProgress.SetPos(progress);
}