#pragma once

#include "user_interface.h"

/////////////////////////////////////////////////////////////////////////////
// окно COutputList

class COutputList : public CRichEditCtrl
{
// Создание
public:
   COutputList() noexcept;

// Реализация
public:
   virtual ~COutputList();

protected:
   afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
   afx_msg void OnEditCopy();
   afx_msg void OnEditClear();
   afx_msg void OnViewOutput();

   DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
   // Construction
public:
   COutputWnd() noexcept;
   void UpdateFonts();
   void SetOutputText(user_interface::OUTPUT_TYPE mode, const char* text, COLORREF color = 0, bool bold = false, bool italic = false);

   // Attributes
protected:
   CMFCTabCtrl	m_wndTabs;
   COutputList m_wndOutput[user_interface::OUTPUT_TYPE::OT_SIZE];
protected:
   void AdjustHorzScroll(CListBox& wndListBox);
   // Implementation
public:
   virtual ~COutputWnd();
protected:
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnSize(UINT nType, int cx, int cy);

   DECLARE_MESSAGE_MAP()
};