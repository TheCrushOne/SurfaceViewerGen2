#pragma once
//#include "thirdparty\TextProgressCtrl.h"

class CAppStatusViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CAppStatusView : public CDockablePane
{
	// Создание
public:
	CAppStatusView() noexcept;

	void AdjustLayout();
	void OnChangeVisualStyle();
	// Реализация
	void SetProgress(unsigned int progress);
public:
	virtual ~CAppStatusView();
protected:
	CProgressCtrl m_overallProgress;
	CAppStatusViewToolBar m_wndToolBar;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};