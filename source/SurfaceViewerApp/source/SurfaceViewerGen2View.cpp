
// SurfaceViewerGen2View.cpp: реализация класса CSurfaceViewerGen2View
//

#include "stdafx.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SurfaceViewerGen2.h"
#endif

#include "SurfaceViewerGen2Doc.h"
#include "SurfaceViewerGen2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "gui/modal/ModalFrame.h"
#include "gui/modal/TestFrame.h"
#include "gui/modal/TestView.h"

#include "gui/CModalDirectX.h"

// CSurfaceViewerGen2View

IMPLEMENT_DYNCREATE(CSurfaceViewerGen2View, CFormView)

BEGIN_MESSAGE_MAP(CSurfaceViewerGen2View, CFormView)
	//ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RENDER, OnRender)
	// Стандартные команды печати
	/*ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSurfaceViewerGen2View::OnFilePrintPreview)*/
	//ON_WM_CONTEXTMENU()
	//ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CSurfaceViewerGen2View

CSurfaceViewerGen2View::CSurfaceViewerGen2View() noexcept
	: CFormView(CSurfaceViewerGen2View::IDD)
{
	// TODO: добавьте код создания
}

CSurfaceViewerGen2View::~CSurfaceViewerGen2View()
{
}

void CSurfaceViewerGen2View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMFCD3DView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

//int CSurfaceViewerGen2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CFormView::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//
//	return 0;
//}

BOOL CSurfaceViewerGen2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSurfaceViewerGen2View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//m_dlg = new CModalDirectxDlg(this);
	//m_dlg->DoModal();
	//m_dlg->Create(IDD_DXMODAL, this);
	//m_dlg->ShowWindow(SW_SHOWNORMAL);
	//dlg.DoModal();
	//CModalDirectxDlg dlg(this);
	//dlg.DoModal();

	/*CCreateContext context;
	context.m_pCurrentDoc = GetDocument();
	context.m_pNewViewClass = RUNTIME_CLASS(CEditView);

	if (!CModalFrame::Run(*RUNTIME_CLASS(CTestFrame),
		_bPlacement,
		IDR_MODALFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, this, &context))
	{
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
	}*/
	/*m_pD3DWnd->SubclassDlgItem(IDC_RENDER, this);

	if (FAILED(m_pD3DWnd->Initialize()))
		PostQuitMessage(1);

	//RECT rect;
	//GetParentFrame()->GetParentFrame()->GetWindowRect(&rect);
	//m_pD3DWnd->MoveWindow(0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);

	m_pD3DWnd->ResetEnvironment();*/
}

// Рисование CSurfaceViewerGen2View

//void CSurfaceViewerGen2View::OnDraw(CDC* /*pDC*/)
//{
//	CSurfaceViewerGen2Doc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	// TODO: добавьте здесь код отрисовки для собственных данных
//}


// Печать CSurfaceViewerGen2View


//void CSurfaceViewerGen2View::OnFilePrintPreview()
//{
//#ifndef SHARED_HANDLERS
//	AFXPrintPreview(this);
//#endif
//}
//
//BOOL CSurfaceViewerGen2View::OnPreparePrinting(CPrintInfo* pInfo)
//{
//	// подготовка по умолчанию
//	return DoPreparePrinting(pInfo);
//}
//
//void CSurfaceViewerGen2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: добавьте дополнительную инициализацию перед печатью
//}
//
//void CSurfaceViewerGen2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: добавьте очистку после печати
//}
//
//void CSurfaceViewerGen2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}
//
//void CSurfaceViewerGen2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}

void CSurfaceViewerGen2View::OnRender()
{
	//m_pD3DWnd->RenderEnvironment();

	// update D3D fps status bar pane
	//m_pMainFrame->UpdateStatusBar(ID_INDICATOR_FPS, m_pD3DWnd->GetFrameStats());
}


// Диагностика CSurfaceViewerGen2View

#ifdef _DEBUG
void CSurfaceViewerGen2View::AssertValid() const
{
	CFormView::AssertValid();
}

void CSurfaceViewerGen2View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSurfaceViewerGen2Doc* CSurfaceViewerGen2View::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSurfaceViewerGen2Doc)));
	return (CSurfaceViewerGen2Doc*)m_pDocument;
}
#endif //_DEBUG

LRESULT CSurfaceViewerGen2View::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CFormView::WindowProc(message, wParam, lParam);
}
// Обработчики сообщений CSurfaceViewerGen2View
