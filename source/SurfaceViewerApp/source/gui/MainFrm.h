
// MainFrm.h: интерфейс класса CMainFrame
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "properties/PropertiesWnd.h"
#include "user_interface.h"

class CMainFrame : public CFrameWndEx
{
   
protected: // создать только из сериализации
   CMainFrame() noexcept;
   DECLARE_DYNCREATE(CMainFrame)

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// Реализация
public:
   virtual ~CMainFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   void SetOutputText(user_interface::OUTPUT_TYPE mode, const char* text, COLORREF color = 0, bool bold = false, bool italic = false);
   void CMainFrame::SetPanelText(const wchar_t* text);

protected:  // встроенные члены панели элементов управления
   CMFCMenuBar       m_wndMenuBar;
   CMFCToolBar       m_wndScenarioToolBar;
   CMFCToolBar       m_wndToolBar;
   CMFCStatusBar     m_wndStatusBar;
   CMFCToolBarImages m_UserImages;
   CFileView         m_wndFileView;
   CClassView        m_wndClassView;
   COutputWnd        m_wndOutput;
   CPropertiesWnd    m_wndProperties;

// Созданные функции схемы сообщений
protected:
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnViewCustomize();
   afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
   afx_msg void OnApplicationLook(UINT id);
   afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
   afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
   afx_msg void OnUpdateToolBar(CCmdUI* pCmdUI);
   DECLARE_MESSAGE_MAP()

   BOOL CreateDockingWindows();
   void SetDockingWindowIcons(BOOL bHiColorIcons);
};


