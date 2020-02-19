// TestFrame.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TestFrame.h"
#include "modalframe.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestFrame

IMPLEMENT_DYNCREATE(CTestFrame, CFrameWnd)

CTestFrame::CTestFrame()
{
}

CTestFrame::~CTestFrame()
{
}

BEGIN_MESSAGE_MAP(CTestFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CTestFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MODAL_FRAME, OnNewModalFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestFrame message handlers


void CMyCreateContext::Test()
{
	//MessageBox("CMyCreateContext::Test");
};

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

int CTestFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (lpCreateStruct->hwndParent == NULL)
		ModifyStyleEx(0, WS_EX_APPWINDOW);

	if (!_tbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)
	||	!_tbar.LoadToolBar(m_nIDHelp)
	||	!_sbar.Create(this)
	||	!_sbar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		return -1;
	}
	return 0;
}

void CTestFrame::OnClose()
{
	CModalFrame::End(this, -1);
}

void CTestFrame::OnNewModalFrame()
{
	CMyCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CTestView);
	context.pszMessage = _T("Hello!");

	if (!CModalFrame::Run(*RUNTIME_CLASS(CTestFrame),
						  false,
						  IDR_MAINFRAME,
						  WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, this, &context))
	{
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
	}
}
