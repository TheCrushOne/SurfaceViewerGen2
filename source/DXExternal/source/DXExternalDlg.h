
// DXExternalDlg.h: файл заголовка
//

#pragma once

class CDXExternalDlgAutoProxy;

#include <d3d11.h>

#include "gui/directx/d3dwnd.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

// Диалоговое окно CDXExternalDlg
class CDXExternalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDXExternalDlg);
	friend class CDXExternalDlgAutoProxy;

// Создание
public:
	CDXExternalDlg(CWnd* pParent = nullptr);	// стандартный конструктор
	virtual ~CDXExternalDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DXEXTERNAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	CDXExternalDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;
	CD3DWnd* m_pD3DWnd;

	BOOL CanExit();

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
};
