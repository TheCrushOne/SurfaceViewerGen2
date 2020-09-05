
// SurfaceViewerGen2.cpp: определяет поведение классов для приложения.
//

#include <future>
#include <string>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <filesystem>

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

//CSurfaceViewerGen2App* gApp = nullptr;

void user_interface::RaiseError()
{
   theApp.RaiseCheckEngine();
}

// CSurfaceViewerGen2App

BEGIN_MESSAGE_MAP(CSurfaceViewerGen2App, CWinAppEx)
   ON_COMMAND(ID_APP_ABOUT, &CSurfaceViewerGen2App::OnAppAbout)
   // Стандартные команды по работе с файлами документов
   ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
   ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
   // Стандартная команда настройки печати
   ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
   ON_COMMAND(ID_TEST_PACKET, OnTestPacket)

   ON_COMMAND(ID_NEW, &CSurfaceViewerGen2App::OnNew)
   ON_COMMAND(ID_OPEN, &CSurfaceViewerGen2App::OnFileOpen)
   ON_COMMAND(ID_SAVE, &CSurfaceViewerGen2App::OnFileSave)
   ON_COMMAND(ID_SAVE_AS, &CSurfaceViewerGen2App::OnFileSaveAs)
   ON_COMMAND(ID_SAVE_FOCUSED_AS, &CSurfaceViewerGen2App::OnFileSaveFocusedAs)
   ON_COMMAND(ID_SCENARIO_DELETE, &CSurfaceViewerGen2App::OnDeleteScenario)
   ON_COMMAND(ID_SCENARIO_CREATE_FOLDER, &CSurfaceViewerGen2App::OnCreateFolder)
   ON_COMMAND(ID_SCENARIO_OPEN, &CSurfaceViewerGen2App::OnSimulate)
   ON_COMMAND(ID_SCENARIO_EDIT, &CSurfaceViewerGen2App::OnRename)

   // TODO: Оживить!!!
   //ON_COMMAND_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnRecentFile)

   ON_COMMAND(ID_EDIT_RUN, &CSurfaceViewerGen2App::OnRun)
   ON_COMMAND(ID_SIM_STEP, &CSurfaceViewerGen2App::OnRunStep)
   ON_COMMAND(ID_EDIT_PAUSE, &CSurfaceViewerGen2App::OnPause)
   ON_COMMAND(ID_EDIT_STOP, &CSurfaceViewerGen2App::OnStop)

   ON_COMMAND(ID_SIM_TIME_SCALE_1X, &CSurfaceViewerGen2App::OnTimeScale1X)
   ON_COMMAND(ID_SIM_TIME_SCALE_10X, &CSurfaceViewerGen2App::OnTimeScale10X)
   ON_COMMAND(ID_SIM_TIME_SCALE_100X, &CSurfaceViewerGen2App::OnTimeScale100X)

   ON_COMMAND(ID_SC_CHOOSESCENARIO, &CSurfaceViewerGen2App::OnChooseScenario)
   ON_COMMAND(ID_SC_PROCESSMAP, &CSurfaceViewerGen2App::OnProcessMap)
   ON_COMMAND(ID_SC_PROCESSSIMPLEPATHS, &CSurfaceViewerGen2App::OnProcessSimplePaths)
   ON_COMMAND(ID_SC_PROCESSOPTIMIZEDPATHS, &CSurfaceViewerGen2App::OnProcessOptimizedPaths)

   //ON_COMMAND(ID_AUTOPAUSE, &CSurfaceViewerGen2App::OnAutoPause)
   //ON_COMMAND(ID_DEBUG, &CSurfaceViewerGen2App::OnDebug)
   ON_COMMAND(ID_DEPTH_AREA + 100, &CSurfaceViewerGen2App::OnFalse)

   ON_UPDATE_COMMAND_UI(ID_EDIT_RUN, &CSurfaceViewerGen2App::OnRunUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_SIM_STEP, &CSurfaceViewerGen2App::OnStepUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_EDIT_PAUSE, &CSurfaceViewerGen2App::OnPauseUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_EDIT_STOP, &CSurfaceViewerGen2App::OnStopUpdateCommandUI)

   ON_UPDATE_COMMAND_UI(ID_SIM_TIME_SCALE_1X, &CSurfaceViewerGen2App::On1XUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_SIM_TIME_SCALE_10X, &CSurfaceViewerGen2App::On10XUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_SIM_TIME_SCALE_100X, &CSurfaceViewerGen2App::On100XUpdateCommandUI)

   ON_UPDATE_COMMAND_UI(ID_SC_CHOOSESCENARIO, &CSurfaceViewerGen2App::OnChooseScenarioUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_SC_PROCESSMAP, &CSurfaceViewerGen2App::OnProcessMapUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_SC_PROCESSSIMPLEPATHS, &CSurfaceViewerGen2App::OnProcessSimplePathsUpdateCommandUI)
   ON_UPDATE_COMMAND_UI(ID_SC_PROCESSOPTIMIZEDPATHS, &CSurfaceViewerGen2App::OnProcessOptimizedPathsUpdateCommandUI)

   ON_UPDATE_COMMAND_UI(ID_CHECK_ENGINE, &CSurfaceViewerGen2App::OnCheckEngineUpdateCommandUI)
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
   ScenarioManager::GetInstance(simulator::GetPack());

   return TRUE;
}

int CSurfaceViewerGen2App::ExitInstance()
{
   //TODO: обработайте дополнительные ресурсы, которые могли быть добавлены
   AfxOleTerm(FALSE);

   return CWinAppEx::ExitInstance();
}

// Обработчики сообщений CSurfaceViewerGen2App

void CSurfaceViewerGen2App::OnNew()
{
   void CreateScenario();
   CreateScenario();
}

void CSurfaceViewerGen2App::OnFileOpen()
{
   // NOTE: отломано
   /*CFileDialog fileDialog(TRUE, NULL, "*.txt;*.crl;*.log;*.xml");
   if (fileDialog.DoModal() == IDOK)
   {
      AddToRecentFileList(fileDialog.GetPathName());
      ScenarioManager::GetInstance().Open(fileDialog.GetPathName());
   }*/
}

void CSurfaceViewerGen2App::OnFileSave()
{
   //ScenarioManager::GetInstance().Save();
}

void CSurfaceViewerGen2App::OnFileSaveAs()
{
   //CFileDialog fileDialog(FALSE, "xml", "state_");
   //if (fileDialog.DoModal() == IDOK)
   //   ScenarioManager::GetInstance().Save(fileDialog.GetPathName());
}

void CSurfaceViewerGen2App::OnFileSaveFocusedAs()
{
   //CFileDialog fileDialog(FALSE, "xml", "screen_state_");
   //if (fileDialog.DoModal() == IDOK)
   //   ScenarioManager::GetInstance().SaveFocused(fileDialog.GetPathName());
}

void CSurfaceViewerGen2App::OnDeleteScenario()
{
   void DeleteScenario();
   DeleteScenario();
}

void CSurfaceViewerGen2App::OnCreateFolder()
{
   void CreateFolder();
   CreateFolder();
}

void CSurfaceViewerGen2App::OnSimulate()
{
   void Simulate();
   Simulate();
}
void CSurfaceViewerGen2App::OnRename()
{
   void Rename();
   Rename();
}

void CSurfaceViewerGen2App::OnRun()
{
   ScenarioManager::GetInstance(simulator::GetPack()).GetState() == ColregSimulation::SCENARIO_STATUS::SS_RUN ? ScenarioManager::GetInstance(simulator::GetPack()).Pause()
      : ScenarioManager::GetInstance(simulator::GetPack()).Run();
}

void CSurfaceViewerGen2App::OnRunStep()
{
   OnPause();
   ScenarioManager::GetInstance(simulator::GetPack()).Step();
}

void CSurfaceViewerGen2App::OnPause()
{
   ScenarioManager::GetInstance(simulator::GetPack()).Pause();
}

void CSurfaceViewerGen2App::OnStop()
{
   ScenarioManager::GetInstance(simulator::GetPack()).Restart();
   user_interface::SetOutputText(user_interface::OT_OUTPUT, "Restart scenario");
}

void CSurfaceViewerGen2App::OnTimeScale1X()
{
   ScenarioManager::GetInstance(simulator::GetPack()).SetTimeScale(1);
   user_interface::SetOutputText(user_interface::OT_OUTPUT, "Set simulation time scale 1x1");
}

void CSurfaceViewerGen2App::OnTimeScale10X()
{
   ScenarioManager::GetInstance(simulator::GetPack()).SetTimeScale(10);
   user_interface::SetOutputText(user_interface::OT_OUTPUT, "Set simulation time scale 1x10");
}

void CSurfaceViewerGen2App::OnTimeScale100X()
{
   ScenarioManager::GetInstance(simulator::GetPack()).SetTimeScale(100);
   user_interface::SetOutputText(user_interface::OT_OUTPUT, "Set simulation time scale 1x100");
}

/*void CSurfaceViewerGen2App::OnAutoPause()
{
   ScenarioManager::GetInstance(simulator::GetPack()).SetAutoPause(!ScenarioManager::GetInstance(simulator::GetPack()).GetAutoPause());
}*/

/*void CSurfaceViewerGen2App::OnShowRelations()
{
   // NOTE: временно отсюда запускается расчет путей
   ScenarioManager::GetInstance(simulator::GetPack()).ReEstimate();
   //ScenarioManager::GetInstance().SetShowRelations(!ScenarioManager::GetInstance().GetShowRelations());
   user_interface::InvalidateView();
}*/

/*void CSurfaceViewerGen2App::OnUploadDangerStatistic()
{
   // NOTE: временно отсюда запускаются исследования
   ScenarioManager::GetInstance(simulator::GetPack()).ReSearch();
   user_interface::InvalidateView();
}*/

void CSurfaceViewerGen2App::OnChooseScenario()
{
   CFileDialog fileDialog(
      TRUE,
      L"meta",
      NULL,
      OFN_HIDEREADONLY,
      L"Scenario Metadata Files (*.meta)|*.meta|",
      AfxGetMainWnd()
   );
   std::wstring cPath = std::filesystem::current_path().c_str();
   cPath += L"\\..\\..\\..\\scenarios";
   // NOTE: CFileDialog почему-то не любит точки в путях...
   auto fsPath = std::filesystem::absolute(cPath);
   fileDialog.m_ofn.lpstrInitialDir = fsPath.c_str();
   if (fileDialog.DoModal() == IDOK)
   {
      AddToRecentFileList(fileDialog.GetPathName());
      ScenarioManager::GetInstance(simulator::GetPack()).CheckOpen(fileDialog.GetPathName(), [this]()
         {
            m_processMap = true;
            activateSimulationControl(false);
            activatePathComputeControl(false);
            refresh();
         }
      );
   }
}

void CSurfaceViewerGen2App::OnProcessMap()
{
   ScenarioManager::GetInstance(simulator::GetPack()).ProcessMap([this]()
      {
         activateSimulationControl(false);
         activatePathComputeControl(true);
         refresh();
      });
}

void CSurfaceViewerGen2App::OnProcessSimplePaths()
{
   ScenarioManager::GetInstance(simulator::GetPack()).ProcessPaths([this]()
      {
         activateSimulationControl(true);
         refresh();
      });
}

void CSurfaceViewerGen2App::OnProcessOptimizedPaths()
{
   ScenarioManager::GetInstance(simulator::GetPack()).ProcessOptPaths([this]()
      {
         activateSimulationControl(true);
         refresh();
      });
}

void CSurfaceViewerGen2App::OnDebug()
{
   // NOTE: резерв
   // TODO: потом подключить
   ScenarioManager::GetInstance(simulator::GetPack()).SetDebugMode(!ScenarioManager::GetInstance(simulator::GetPack()).IsDebugMode());
}

void CSurfaceViewerGen2App::OnFalse()
{}

void CSurfaceViewerGen2App::OnRunUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_runStatus);
}

void CSurfaceViewerGen2App::OnStepUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_runStepStatus);
}

void CSurfaceViewerGen2App::OnPauseUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_pauseStatus);
}

void CSurfaceViewerGen2App::OnStopUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_stopStatus);
}

void CSurfaceViewerGen2App::On1XUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_timeScale1XStatus);
}

void CSurfaceViewerGen2App::On10XUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_timeScale10XStatus);
}

void CSurfaceViewerGen2App::On100XUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_timeScale100XStatus);
}

void CSurfaceViewerGen2App::OnChooseScenarioUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_chooseScenario);
}

void CSurfaceViewerGen2App::OnProcessMapUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_processMap);
}

void CSurfaceViewerGen2App::OnProcessSimplePathsUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_simplePaths);
}

void CSurfaceViewerGen2App::OnProcessOptimizedPathsUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_optPaths);
}

void CSurfaceViewerGen2App::OnCheckEngineUpdateCommandUI(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_checkEngineStatus);
}

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