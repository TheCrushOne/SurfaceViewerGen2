/////////////////////////////////////////////////////////////////////////
//
// CModalFrame					Version 1.2
//
// Created: May 28, 2003		Last modified: Aug 5, 2005
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 2003-2005 by Alexey Nikitin. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// nick@complex-a5.ru.
/////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxpriv.h>
#include "ModalFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{

	class CModalFrameLock : public CWnd
	{
	public:
		CModalFrameLock(CWnd* pParentWnd);   // standard constructor
		~CModalFrameLock();

		int DoModal();
		void EndModal(int nResult);

		bool				_bTopLevelParentPlacement;
		CRuntimeClass* _pFrameWndClass;
		UINT				_nIDResource;
		DWORD				_dwDefaultStyle;
		CCreateContext* _pContext;

		static CModalFrameLock* FromHandle(HWND hwndFrame);

	private:
		CModalFrameLock* _prev;
		CModalFrameLock* _next;

		CWnd* _pParentWnd;
		CFrameWnd* _pFrameWnd;
		WNDPROC		_WndProc;

		static LRESULT CALLBACK FrameSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static CModalFrameLock _barier;

		CFrameWnd* CreateFrame(CWnd* pParentWnd);
	};

} // namespace

/////////////////////////////////////////////////////////////////////////////
// CModalFrame::

void CModalFrame::End(CFrameWnd* pModalFrameWnd, int nResult)
{
	ASSERT(pModalFrameWnd && ::IsWindow(pModalFrameWnd->m_hWnd));
	if (CModalFrameLock* pLock = CModalFrameLock::FromHandle(pModalFrameWnd->m_hWnd))
	{
		pLock->EndModal(nResult);
		return;
	}
	ASSERT(false);
}	// CModalFrame::End

bool CModalFrame::Run(CRuntimeClass& FrameWndClass,
	bool bTopLevelParentPlacement,
	UINT nIDResource,
	DWORD dwDefaultStyle,
	CWnd* pParentWnd,
	CCreateContext* pContext)
{
	ASSERT(!(dwDefaultStyle & WS_CHILD));
	if (dwDefaultStyle & WS_CHILD)
		return false;

	ASSERT(nIDResource != 0); // must have a resource ID to load from
	ASSERT(&FrameWndClass);
	ASSERT(FrameWndClass.IsDerivedFrom(RUNTIME_CLASS(CFrameWnd)));

	CModalFrameLock dlg(pParentWnd);

	dlg._pFrameWndClass = &FrameWndClass;
	dlg._bTopLevelParentPlacement = bTopLevelParentPlacement;
	dlg._nIDResource = nIDResource;
	dlg._dwDefaultStyle = dwDefaultStyle;
	dlg._pContext = pContext;

	return (dlg.DoModal() != IDCANCEL);
}	// CModalFrame::Run

/////////////////////////////////////////////////////////////////////////////
// CModalFrameLock::

CModalFrameLock::CModalFrameLock(CWnd* pParentWnd)
	: _pParentWnd(pParentWnd)
	, _pFrameWnd(NULL)
	, _WndProc(NULL)
{
	if (this == &_barier)
		_prev = _next = this;
	else
		(((_next = _barier._next)->_prev = this)->_prev = &_barier)->_next = this;
}

CModalFrameLock::~CModalFrameLock()
{
	if (this != &_barier)
	{
		(_prev->_next = _next)->_prev = _prev;
		_prev = _next = NULL;
	}
}

CModalFrameLock CModalFrameLock::_barier(NULL);

CModalFrameLock* CModalFrameLock::FromHandle(HWND hwnd)
{
	for (CModalFrameLock* c = _barier._next; c != &_barier; c = c->_next)
	{
		if (c->m_hWnd == hwnd)
		{
			ASSERT(c->_pFrameWnd && c->_pFrameWnd->m_hWnd == hwnd);
			return c;
		}
	}
	return NULL;
}

LRESULT CALLBACK CModalFrameLock::FrameSubclassProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	CModalFrameLock* pLock = FromHandle(hwnd);

#if 0
	if (uMsg == WM_NCACTIVATE)
	{
		bool bRestoreFlag = false;
		if ((pLock->_pFrameWnd->m_nFlags & WF_STAYACTIVE) != 0)
		{
			bRestoreFlag = true;
			pLock->_pFrameWnd->m_nFlags &= ~WF_STAYACTIVE;
		}
		LRESULT lr = CallWindowProc(pLock->_WndProc, hwnd, uMsg, wParam, lParam);
		if (bRestoreFlag)
			pLock->_pFrameWnd->m_nFlags |= WF_STAYACTIVE;
		return lr;
	}
#endif
	if (uMsg == WM_NCDESTROY)
	{
		if (pLock->m_nFlags & WF_CONTINUEMODAL)
			pLock->EndModal(-1);

		// check for correct subclassing
		ASSERT(::GetWindowLongPtr(hwnd, GWLP_WNDPROC) == LONG_PTR(FrameSubclassProc));

		// unsubclass frame window
		WNDPROC prevWndProc = pLock->_WndProc;
		pLock->_WndProc = NULL;
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)prevWndProc);

		// detach lock from frame
		pLock->m_hWnd = NULL;
		pLock->_pFrameWnd = NULL;

		return CallWindowProc(prevWndProc, hwnd, uMsg, wParam, lParam);
	}
	if (uMsg == WM_KICKIDLE)
	{
		// send WM_IDLEUPDATECMDUI to the frame window
		CWnd* pFrame = pLock->_pFrameWnd;
		if (pFrame->IsWindowVisible())
		{
			AfxCallWndProc(pFrame, pFrame->m_hWnd,
				WM_IDLEUPDATECMDUI, (WPARAM)true, 0);
			pFrame->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
				(WPARAM)true, 0, true, true);
		}
	}
	return CallWindowProc(pLock->_WndProc, hwnd, uMsg, wParam, lParam);
}	// FrameSubclassProc

CFrameWnd* CModalFrameLock::CreateFrame(CWnd* pParentWnd)
{
	CFrameWnd* pFrame = static_cast<CFrameWnd*>(_pFrameWndClass->CreateObject());
	if (pFrame == NULL)
	{
		TRACE1("Warning: Dynamic create of frame %hs failed.\n",
			_pFrameWndClass->m_lpszClassName);
		return NULL;
	}

#ifdef _DEBUG
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (_pContext && _pContext->m_pNewViewClass == NULL)
		TRACE0("Warning: creating frame with no default view.\n");
#endif

	// create new frame from resource
	if (!pFrame->LoadFrame(_nIDResource, _dwDefaultStyle, pParentWnd, _pContext))
	{
		TRACE0("Warning: couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}
	// Subclass frame window.
	{
		WNDPROC wndproc = FrameSubclassProc;
		_WndProc = (WNDPROC)SetWindowLongPtr(*pFrame, GWLP_WNDPROC, LONG_PTR(wndproc));
	}
	m_hWnd = (_pFrameWnd = pFrame)->m_hWnd;
	return pFrame;
}	// CModalFrameLock::CreateFrame

void CModalFrameLock::EndModal(int nResult)
{
	ASSERT(::IsWindow(m_hWnd));
	if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL))
		EndModalLoop(nResult);
}	// CModalFrameLock::EndModal

int CModalFrameLock::DoModal()
{
	// disable parent (before creating dialog)
	if (CWinApp* pApp = AfxGetApp())
		pApp->EnableModeless(false);

	HWND hWndTop;

	// find parent HWND and return it to use as parent for dialog
	HWND hWndParent = CWnd::GetSafeOwner_(_pParentWnd->GetSafeHwnd(), &hWndTop);

	bool bEnableParent = false;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, false);
		bEnableParent = true;
	}

	m_nModalResult = -1;
	m_nFlags |= WF_CONTINUEMODAL;

	TRY
	{
		// create frame window
		{
			if (CFrameWnd* pFrameWnd = CreateFrame(CWnd::FromHandle(hWndParent)))
			{
				if (_pParentWnd && _bTopLevelParentPlacement)
				{
					WINDOWPLACEMENT wp = {0};
					wp.length = sizeof(wp);
					if (CWnd* pParentWnd = _pParentWnd->GetTopLevelParent())
					{
						pParentWnd->GetWindowPlacement(&wp);
						pFrameWnd->SetWindowPlacement(&wp);
					}
				}
				if (_pContext && _pContext->m_pCurrentDoc)
				{
					CDocument* pDocument = _pContext->m_pCurrentDoc;
					CDocTemplate* pTemplate = pDocument->GetDocTemplate();
					pTemplate->InitialUpdateFrame(pFrameWnd, pDocument);
				}
				else
				{
					pFrameWnd->InitialUpdateFrame(NULL, true);
				}
			}
		}
		if (!m_hWnd || !::IsWindow(m_hWnd))
		{
			TRACE0("Warning: failed to create new frame.\n");
			EndModal(-2);
		}
		if (m_nFlags & WF_CONTINUEMODAL)
		{
			// enter modal loop
			DWORD dwFlags = MLF_SHOWONIDLE;
			if (GetStyle() & DS_NOIDLEMSG)
				dwFlags |= MLF_NOIDLEMSG;
			VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
		}
		// hide the window before enabling the parent, etc.
		if (m_hWnd != NULL)
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
		CATCH_ALL(e)
	{
		e->Delete();
		m_nModalResult = -2;
	}
	END_CATCH_ALL

		if (bEnableParent)
			::EnableWindow(hWndParent, true);

	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	if (m_hWnd)
	{
		DestroyWindow();
		ASSERT(!m_hWnd);
	}

	// re-enable windows
	if (::IsWindow(hWndTop))
		::EnableWindow(hWndTop, true);

	if (CWinApp* pApp = AfxGetApp())
		pApp->EnableModeless(true);

	return m_nModalResult;
}	// CModalFrameLock::DoModal

/////////////////////////////////////////////////////////////////////////////
