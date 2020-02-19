﻿
// DXExternalDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "framework.h"
#include "DXExternal.h"
#include "DXExternalDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CDXExternalDlg


IMPLEMENT_DYNAMIC(CDXExternalDlg, CDialogEx);

CDXExternalDlg::CDXExternalDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DXEXTERNAL_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;

	m_pD3DWnd = new CD3DWnd;
}

CDXExternalDlg::~CDXExternalDlg()
{
	// Если для данного диалогового окна используется прокси-сервер автоматизации, установите
	//  для его обратного указателя на это диалоговое окно значение NULL, чтобы он узнал об
	//  удалении диалогового окна.
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
	delete m_pD3DWnd;
}

void CDXExternalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDXExternalDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Обработчики сообщений CDXExternalDlg

BOOL CDXExternalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CDXExternalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CDXExternalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}


	// Перенести в windowproc
	//if (m_pD3DWnd && m_pD3DWnd->IsActive())
	//	if (m_pD3DWnd->HandleMouseMessages(message, wParam, lParam))
	//		return 0L;
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CDXExternalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Серверы автоматизации не должны завершать работу при закрытии интерфейса пользователя,
//  если контроллеры по-прежнему удерживают один из его объектов.  Эти
//  обработчики сообщений гарантируют, что если прокси-сервер еще используется,
//  то пользовательский интерфейс скрывается, но диалоговое окно остается (если оно
//  закрыто).

void CDXExternalDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CDXExternalDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CDXExternalDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CDXExternalDlg::CanExit()
{
	// Если объект прокси-сервера по-прежнему запущен, то контроллер
	//  автоматизации удерживает это приложение.  Оставьте
	//  диалоговое окно запущенным, но скройте его пользовательский интерфейс.
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

