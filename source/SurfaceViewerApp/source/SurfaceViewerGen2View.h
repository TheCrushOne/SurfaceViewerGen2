
// SurfaceViewerGen2View.h: интерфейс класса CSurfaceViewerGen2View
//

#if !defined(AFX_MFCD3DVIEW_H__F98F15F6_2AC2_41DC_B694_78FA2B25E99E__INCLUDED_)
#define AFX_MFCD3DVIEW_H__F98F15F6_2AC2_41DC_B694_78FA2B25E99E__INCLUDED_

#pragma once

class CModalDirectxDlg;

class CSurfaceViewerGen2View : public CFormView
{
protected: // создать только из сериализации
   CSurfaceViewerGen2View() noexcept;
   DECLARE_DYNCREATE(CSurfaceViewerGen2View)

// Атрибуты
public:
   CSurfaceViewerGen2Doc* GetDocument() const;

public:
   //{{AFX_DATA(CMFCD3DView)
   enum { IDD = IDD_SVG2V_FORM };
   // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA

// Операции
public:

// Переопределение
public:
   //virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   virtual void OnInitialUpdate(); // called first time after construct
   virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
   /*virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
   virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);*/

// Реализация
public:
   virtual ~CSurfaceViewerGen2View();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
   /*afx_msg void OnFilePrintPreview();
   afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);*/
   afx_msg void OnRender();
   DECLARE_MESSAGE_MAP()
private:
   BOOL	_bPlacement;
   CModalDirectxDlg* m_dlg;
};

#ifndef _DEBUG  // версия отладки в SurfaceViewerGen2View.cpp
inline CSurfaceViewerGen2Doc* CSurfaceViewerGen2View::GetDocument() const
   { return reinterpret_cast<CSurfaceViewerGen2Doc*>(m_pDocument); }
#endif

#endif // !defined(AFX_MFCD3DVIEW_H__F98F15F6_2AC2_41DC_B694_78FA2B25E99E__INCLUDED_)