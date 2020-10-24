#pragma once 

#include "Resource.h"
// MainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog
namespace SV
{
   class CModalDirectxDlg : public CDialog
   {
      // Construction
   public:
      CModalDirectxDlg(CWnd* pParent = NULL);	// standard constructor

   // Dialog Data
      //{{AFX_DATA(CMainDlg)
      enum { IDD = IDD_DXMODAL };
      BOOL	_bPlacement;
      //}}AFX_DATA

      // ClassWizard generated virtual function overrides
      //{{AFX_VIRTUAL(CMainDlg)
   protected:
      virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
      //}}AFX_VIRTUAL

   // Implementation
   protected:
      HICON m_hIcon;

      // Generated message map functions
      //{{AFX_MSG(CMainDlg)
      virtual BOOL OnInitDialog();
      afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
      afx_msg void OnPaint();
      afx_msg HCURSOR OnQueryDragIcon();
      afx_msg void OnButton1();
      afx_msg void OnCheck1();
      //}}AFX_MSG
      DECLARE_MESSAGE_MAP()
   };
}