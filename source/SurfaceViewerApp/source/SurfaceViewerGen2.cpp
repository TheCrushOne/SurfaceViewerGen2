
// SurfaceViewerGen2.cpp: определяет поведение классов для приложения.
//

#include <future>
#include <string>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "stdafx.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SurfaceViewerGen2.h"
#include "gui/ScenarioView.h"
#include "gui/MainFrm.h"

#include "SurfaceViewerGen2Doc.h"
#include "SurfaceViewerGen2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSurfaceViewerGen2App

BEGIN_MESSAGE_MAP(CSurfaceViewerGen2App, CWinAppEx)
   ON_COMMAND(ID_APP_ABOUT, &CSurfaceViewerGen2App::OnAppAbout)
   // Стандартные команды по работе с файлами документов
   ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
   ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
   // Стандартная команда настройки печати
   ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
   ON_COMMAND(ID_TEST_PACKET, OnTestPacket)
END_MESSAGE_MAP()


// Создание CSurfaceViewerGen2App

CSurfaceViewerGen2App::CSurfaceViewerGen2App() noexcept
{
   m_bHiColorIcons = TRUE;

   // поддержка диспетчера перезагрузки
   m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
   // Если приложение построено с поддержкой среды Common Language Runtime (/clr):
   //     1) Этот дополнительный параметр требуется для правильной поддержки работы диспетчера перезагрузки.
   //   2) В своем проекте для сборки необходимо добавить ссылку на System.Windows.Forms.
   System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

   // TODO: замените ниже строку идентификатора приложения строкой уникального идентификатора; рекомендуемый
   // формат для строки: ИмяКомпании.ИмяПродукта.СубПродукт.СведенияОВерсии
   SetAppID(_T("SurfaceViewerGen2.AppID.NoVersion"));

   // TODO: добавьте код создания,
   // Размещает весь важный код инициализации в InitInstance
}

// Единственный объект CSurfaceViewerGen2App

CSurfaceViewerGen2App theApp;


// Инициализация CSurfaceViewerGen2App

BOOL CSurfaceViewerGen2App::InitInstance()
{
   // InitCommonControlsEx() требуются для Windows XP, если манифест
   // приложения использует ComCtl32.dll версии 6 или более поздней версии для включения
   // стилей отображения.  В противном случае будет возникать сбой при создании любого окна.
   INITCOMMONCONTROLSEX InitCtrls;
   InitCtrls.dwSize = sizeof(InitCtrls);
   // Выберите этот параметр для включения всех общих классов управления, которые необходимо использовать
   // в вашем приложении.
   InitCtrls.dwICC = ICC_WIN95_CLASSES;
   InitCommonControlsEx(&InitCtrls);

   CWinAppEx::InitInstance();


   // Инициализация библиотек OLE
   if (!AfxOleInit())
   {
      AfxMessageBox(IDP_OLE_INIT_FAILED);
      return FALSE;
   }

   AfxEnableControlContainer();

   EnableTaskbarInteraction(FALSE);

   // Для использования элемента управления RichEdit требуется метод AfxInitRichEdit2()
   // AfxInitRichEdit2();

   // Стандартная инициализация
   // Если эти возможности не используются и необходимо уменьшить размер
   // конечного исполняемого файла, необходимо удалить из следующего
   // конкретные процедуры инициализации, которые не требуются
   // Измените раздел реестра, в котором хранятся параметры
   // TODO: следует изменить эту строку на что-нибудь подходящее,
   // например на название организации
   SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));
   LoadStdProfileSettings(4);  // Загрузите стандартные параметры INI-файла (включая MRU)


   InitContextMenuManager();

   InitKeyboardManager();

   InitTooltipManager();
   CMFCToolTipInfo ttParams;
   ttParams.m_bVislManagerTheme = TRUE;
   theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
      RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

   // Зарегистрируйте шаблоны документов приложения.  Шаблоны документов
   //  выступают в роли посредника между документами, окнами рамок и представлениями
   CSingleDocTemplate* pDocTemplate;
   pDocTemplate = new CSingleDocTemplate(
      IDR_MAINFRAME,
      RUNTIME_CLASS(CSurfaceViewerGen2Doc),
      RUNTIME_CLASS(CMainFrame),       // основное окно рамки SDI
      RUNTIME_CLASS(ScenarioView));
      //RUNTIME_CLASS(CSurfaceViewerGen2View));
   if (!pDocTemplate)
      return FALSE;
   AddDocTemplate(pDocTemplate);


   // Разрешить использование расширенных символов в горячих клавишах меню
   CMFCToolBar::m_bExtCharTranslation = TRUE;

   // Синтаксический разбор командной строки на стандартные команды оболочки, DDE, открытие файлов
   CCommandLineInfo cmdInfo;
   ParseCommandLine(cmdInfo);


   // Команды диспетчеризации, указанные в командной строке.  Значение FALSE будет возвращено, если
   // приложение было запущено с параметром /RegServer, /Register, /Unregserver или /Unregister.
   if (!ProcessShellCommand(cmdInfo))
      return FALSE;

   // Одно и только одно окно было инициализировано, поэтому отобразите и обновите его
   m_pMainWnd->ShowWindow(SW_MAXIMIZE);
   //m_pMainWnd->SetWindowPos();
   m_pMainWnd->UpdateWindow();

   // Запуск всего и вся
   //createDirectXApp();
   ScenarioManager::GetInstance();

   return TRUE;
}

int CSurfaceViewerGen2App::ExitInstance()
{
   //TODO: обработайте дополнительные ресурсы, которые могли быть добавлены
   AfxOleTerm(FALSE);

   return CWinAppEx::ExitInstance();
}

// Обработчики сообщений CSurfaceViewerGen2App


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
   CAboutDlg() noexcept;

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

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Команда приложения для запуска диалога
void CSurfaceViewerGen2App::OnAppAbout()
{
   CAboutDlg aboutDlg;
   aboutDlg.DoModal();
}

// Методы загрузки или сохранения настроек CSurfaceViewerGen2App

void CSurfaceViewerGen2App::PreLoadState()
{
   BOOL bNameValid;
   CString strName;
   bNameValid = strName.LoadString(IDS_EDIT_MENU);
   ASSERT(bNameValid);
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
   bNameValid = strName.LoadString(IDS_EXPLORER);
   ASSERT(bNameValid);
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CSurfaceViewerGen2App::LoadCustomState()
{
}

void CSurfaceViewerGen2App::SaveCustomState()
{
}

// Обработчики сообщений CSurfaceViewerGen2App


BOOL CSurfaceViewerGen2App::OnIdle(LONG lCount)
{
   if (CWinApp::OnIdle(lCount))
      return TRUE;

   m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_RENDER);

   return FALSE;
}

void CSurfaceViewerGen2App::OnTestPacket()
{
   
}

void CSurfaceViewerGen2App::createDirectXApp()
{
   // additional information
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   // set the size of the structures
   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));

   // start the program up
   CreateProcess(L"lab.exe",   // the path
      L"",            // Command line
      NULL,           // Process handle not inheritable
      NULL,           // Thread handle not inheritable
      FALSE,          // Set handle inheritance to FALSE
      0,              // No creation flags
      NULL,           // Use parent's environment block
      NULL,           // Use parent's starting directory 
      &si,            // Pointer to STARTUPINFO structure
      &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
   );
   // Close process and thread handles. 
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);
}