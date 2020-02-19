#if !defined(AFX_TESTFRAME_H__644939A3_DF54_4F68_A69C_BADB6FB435CB__INCLUDED_)
#define AFX_TESTFRAME_H__644939A3_DF54_4F68_A69C_BADB6FB435CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestFrame frame

class CMyCreateContext : public CCreateContext
{
public:
	LPCTSTR pszMessage;
	virtual void Test();
};

class CTestFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CTestFrame)
protected:
	CTestFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTestFrame();

	CToolBar	_tbar;
	CStatusBar	_sbar;

	// Generated message map functions
	//{{AFX_MSG(CTestFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnNewModalFrame();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTFRAME_H__644939A3_DF54_4F68_A69C_BADB6FB435CB__INCLUDED_)
