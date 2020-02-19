
#include "stdafx.h"
#include "framework.h"
#include "mainfrm.h"
#include "FileView.h"
#include "common/file_storage.h"
#include "Resource.h"
#include "SurfaceViewerGen2.h"

#include "scenario\ScenarioManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const wchar_t* TEST_SCENARIOS_SRC_DIR = L"\\..\\scenarios";

namespace
{
	bool SHRemane(LPCTSTR from, LPCTSTR to)
	{
		SHFILEOPSTRUCT fileOp = { 0 };
		fileOp.wFunc = FO_RENAME;
		TCHAR newFrom[MAX_PATH];
		_tcscpy_s(newFrom, from);
		newFrom[_tcsclen(from) + 1] = NULL;
		fileOp.pFrom = newFrom;
		TCHAR newTo[MAX_PATH];
		_tcscpy_s(newTo, to);
		newTo[_tcsclen(to) + 1] = NULL;
		fileOp.pTo = newTo;
		fileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;

		int result = SHFileOperation(&fileOp);



		return result == 0;
	}

	std::wstring ExePath()
	{
		wchar_t buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		return std::wstring(buffer).substr(0, pos);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView() noexcept
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()

	//ON_COMMAND(ID_PROPERTIES, OnProperties)
	//ON_COMMAND(ID_OPEN, OnFileOpen)
	//ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	//ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	//ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	//ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)

	//ON_COMMAND(ID_SCENARIO_CREATE_FOLDER, OnScenarioCreateFolder)
	//ON_COMMAND(ID_NEW, OnScenarioCreate)
	//ON_COMMAND(ID_SCENARIO_OPEN, OnScenarioOpen)
	//ON_COMMAND(ID_SCENARIO_DELETE, OnScenarioDelete)
	//ON_COMMAND(ID_SCENARIO_EDIT, OnScenarioEdit)

	ON_NOTIFY(TVN_ENDLABELEDIT, 4, OnEndlabeledit)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, 4, &CFileView::OnNMDblclkTree1)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// обработчики сообщений CWorkspaceBar

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Создание представления:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_treeScenarios.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Не удалось создать представление файлов\n");
		return -1;      // не удалось создать
	}

	// Загрузить изображения представления:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_treeScenarios.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// Все команды будут перенаправлены через этот элемент управления, а не через родительскую рамку:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Введите некоторые данные статического представления в виде дерева (пустой код, ничего более)
	fillScenariosFiles();
	AdjustLayout();

	return 0;
}

void CFileView::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	m_treeScenarios.ScreenToClient(&curPoint);
	HTREEITEM hItemSel = m_treeScenarios.HitTest(curPoint, &nFlags);
	if (hItemSel != NULL && !m_treeScenarios.ItemHasChildren(hItemSel))
	{
		Simulate();	// TODO: Переделать!!!
		//ScenarioManager::GetInstance().Open(getFileName(hItemSel).c_str());      
		*pResult = 1;
	}
	else
		*pResult = 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::Simulate()
{
	HTREEITEM hItemSel = m_treeScenarios.GetSelectedItem();
	if (hItemSel != nullptr && m_treeScenarios.GetChildItem(hItemSel) == nullptr)
	{
		const auto name = getFileName(hItemSel);
		if (name.find('.', 0) == std::string::npos)
			return;
		ScenarioManager::GetInstance().Open(name.c_str());
	}
}

void CFileView::fillScenariosFiles()
{
	m_treeScenarios.DeleteAllItems();
	const auto testDir = ExePath().append(TEST_SCENARIOS_SRC_DIR);

	addScenarios(getItem(NULL, testDir.c_str()), testDir.c_str(), true);
	addScenarios(getItem(NULL, testDir.c_str()), testDir.c_str(), false);

	m_treeScenarios.Expand(m_treeScenarios.GetRootItem(), TVE_EXPAND);

}

void CFileView::addScenarios(HTREEITEM hRoot, const wchar_t* dir, bool bAddFolder)
{
	WIN32_FIND_DATA	FindFileData;
	HANDLE	hFind;
	std::wstring findpath(dir);
	{
		findpath += L"\\*";
		hFind = ::FindFirstFile(findpath.c_str(), &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			while (hFind)
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (FindFileData.cFileName[0] != '.')
					{
						std::wstring cfn = FindFileData.cFileName;
						std::wstring subdir = dir;
						subdir += L"\\";
						subdir += cfn;
						addScenarios(getItem(hRoot, cfn.c_str()), subdir.c_str(), bAddFolder);
					}
				}
				else if (!bAddFolder)
				{
					std::wstring fname_w = FindFileData.cFileName;
					std::string fname(fname_w.begin(), fname_w.end());
					//fpath strFile(FindFileData.cFileName);
					// NOTE: отломан чек на xml
					if (fname.find(".meta") != std::string::npos)
						m_treeScenarios.InsertItem(FindFileData.cFileName, 2, 2, hRoot);
				}
				if (!::FindNextFile(hFind, &FindFileData))
					break;
			}
			::FindClose(hFind);
		}
	}
}

//void CFileView::FillFileView()
//{
//	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("Файлы FakeApp"), 0, 0);
//	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
//
//	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("Исходные файлы FakeApp"), 0, 0, hRoot);
//
//	m_wndFileView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
//	m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
//	m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
//	m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
//	m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
//	m_wndFileView.InsertItem(_T("pch.cpp"), 1, 1, hSrc);
//
//	HTREEITEM hInc = m_wndFileView.InsertItem(_T("Файлы заголовка FakeApp"), 0, 0, hRoot);
//
//	m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
//	m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
//	m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
//	m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
//	m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
//	m_wndFileView.InsertItem(_T("pch.h"), 2, 2, hInc);
//
//	HTREEITEM hRes = m_wndFileView.InsertItem(_T("Файлы ресурсов FakeApp"), 0, 0, hRoot);
//
//	m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
//	m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
//	m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
//	m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);
//
//	m_wndFileView.Expand(hRoot, TVE_EXPAND);
//	m_wndFileView.Expand(hSrc, TVE_EXPAND);
//	m_wndFileView.Expand(hInc, TVE_EXPAND);
//}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_treeScenarios;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Выбрать нажатый элемент:
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
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_treeScenarios.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

//void CFileView::OnProperties()
//{
//	AfxMessageBox(_T("Свойства...."));
//}

//void CFileView::OnFileOpen()
//{
//	// TODO: добавьте сюда код обработчика команд
//}
//
//void CFileView::OnFileOpenWith()
//{
//	// TODO: добавьте сюда код обработчика команд
//}
//
//void CFileView::OnDummyCompile()
//{
//	// TODO: добавьте сюда код обработчика команд
//}
//
//void CFileView::OnEditCut()
//{
//	// TODO: добавьте сюда код обработчика команд
//}
//
//void CFileView::OnEditCopy()
//{
//	// TODO: добавьте сюда код обработчика команд
//}
//
//void CFileView::OnEditClear()
//{
//	// TODO: добавьте сюда код обработчика команд
//}

void CFileView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItemSel = m_treeScenarios.GetSelectedItem();

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

	std::wstring pathFrom(getFileName(hItemSel).begin(), getFileName(hItemSel).end());

	m_treeScenarios.SetItem(&newitem);

	if (hItemSel != nullptr)
	{
		std::wstring pathTo(getFileName(newitem.hItem).begin(), getFileName(newitem.hItem).end());
		SHRemane(pathFrom.c_str(), pathTo.c_str());
	}
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // контекст устройства для рисования

	CRect rectTree;
	m_treeScenarios.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_treeScenarios.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Не удается загрузить точечный рисунок: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_treeScenarios.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


