#if !defined(AFX_TESTVIEW_H__4830FA55_9CB7_4DBD_8962_EA8402746B1F__INCLUDED_)
#define AFX_TESTVIEW_H__4830FA55_9CB7_4DBD_8962_EA8402746B1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestView view

class CTestView : public CView
{
protected:
	CTestView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTestView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	bool _bYellow;

	//{{AFX_MSG(CTestView)
	afx_msg void OnFilePrintPreview();
	afx_msg void OnViewGreen();
	afx_msg void OnUpdateViewGreen(CCmdUI* pCmdUI);
	afx_msg void OnViewYellow();
	afx_msg void OnUpdateViewYellow(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVIEW_H__4830FA55_9CB7_4DBD_8962_EA8402746B1F__INCLUDED_)
