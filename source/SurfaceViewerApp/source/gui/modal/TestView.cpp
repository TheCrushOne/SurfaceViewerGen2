// TestView.cpp : implementation file
//

#include "stdafx.h"
//#include "ModalFrame1.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

CTestView::CTestView()
	: _bYellow(false)
{
}

CTestView::~CTestView()
{
}

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_COMMAND(ID_VIEW_GREEN, OnViewGreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GREEN, OnUpdateViewGreen)
	ON_COMMAND(ID_VIEW_YELLOW, OnViewYellow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_YELLOW, OnUpdateViewYellow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(10, 10);
	pDC->FillSolidRect(rect, _bYellow ? RGB(255, 255, 0) : RGB(0, 255, 0));
	pDC->DrawText(L"This is a test view", -1, rect, DT_CENTER | DT_VCENTER);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

void CTestView::OnFilePrintPreview()
{
	if (!AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		AfxMessageBox(_T("Application main window is not a CFrameWnd-derived class"));
	else
		CView::OnFilePrintPreview();
}	// CTestView::OnFilePrintPreview

void CTestView::OnViewGreen() {
	_bYellow = false; Invalidate(); }

void CTestView::OnUpdateViewGreen(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(!_bYellow); }

void CTestView::OnViewYellow() {
	_bYellow = true; Invalidate(); }

void CTestView::OnUpdateViewYellow(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(_bYellow); }
