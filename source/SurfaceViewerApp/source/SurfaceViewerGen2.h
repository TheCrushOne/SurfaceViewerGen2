
// SurfaceViewerGen2.h: основной файл заголовка для приложения SurfaceViewerGen2
//
#pragma once

#ifndef __AFXWIN_H__
   #error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы
#include "colreg\ModuleGuard.h"
#include "colreg/ColregSimulation.h"
#include "scenario/ScenarioManager.h"

// CSurfaceViewerGen2App:
// Сведения о реализации этого класса: SurfaceViewerGen2.cpp
//

class CSurfaceViewerGen2App : public CWinAppEx
{
public:
   CSurfaceViewerGen2App() noexcept;
protected:
   void createDirectXApp();
   void refresh() { /*this->OnIdle(-1);*/ }
   void recountSimulationControlStatus(SV::surface_simulation::SCENARIO_STATUS status)
   {
      SV::ScenarioDispather::GetInstance().OnScenarioScenarioStatusChanged(status);
      m_runStatus = status >= SV::surface_simulation::SCENARIO_STATUS::SS_PATHS_COUNTED;
      m_runStepStatus = status >= SV::surface_simulation::SCENARIO_STATUS::SS_PATHS_COUNTED;
      m_pauseStatus = status >= SV::surface_simulation::SCENARIO_STATUS::SS_PATHS_COUNTED;
      m_stopStatus = status >= SV::surface_simulation::SCENARIO_STATUS::SS_PATHS_COUNTED;
      m_chooseScenario = true;
      m_processMap = status >= SV::surface_simulation::SCENARIO_STATUS::SS_MAP_CHECKOPENED;
      m_processMapObj = status >= SV::surface_simulation::SCENARIO_STATUS::SS_MAP_PROCESSED;
      m_simplePaths = status >= SV::surface_simulation::SCENARIO_STATUS::SS_MAPOBJ_PROCESSED;
      m_optPaths = status >= SV::surface_simulation::SCENARIO_STATUS::SS_MAPOBJ_PROCESSED;
   }
   // Переопределение
public:
   virtual BOOL InitInstance();
   virtual BOOL OnIdle(LONG lCount);
   virtual int ExitInstance();

   void RaiseCheckEngine() { m_checkEngineStatus = true; refresh(); }

   // Реализация
   UINT  m_nAppLook;
   BOOL  m_bHiColorIcons;
   bool m_checkEngineStatus = false;

   bool m_runStatus = false;
   bool m_runStepStatus = false;
   bool m_pauseStatus = false;
   bool m_stopStatus = false;

   bool m_timeScale1XStatus = false;
   bool m_timeScale10XStatus = false;
   bool m_timeScale100XStatus = false;

   bool m_chooseScenario = true;
   bool m_processMap = false;
   bool m_processMapObj = false;
   bool m_simplePaths = false;
   bool m_optPaths = false;

   virtual void PreLoadState();
   virtual void LoadCustomState();
   virtual void SaveCustomState();

   afx_msg void OnTestPacket();
   afx_msg void OnAppAbout();

   afx_msg void OnNew();
   afx_msg void OnFileOpen();
   afx_msg void OnFileSave();
   afx_msg void OnFileSaveAs();
   afx_msg void OnFileSaveFocusedAs();
   afx_msg void OnDeleteScenario();
   afx_msg void OnCreateFolder();
   afx_msg void OnSimulate();
   afx_msg void OnRename();

   afx_msg void OnRun();
   afx_msg void OnRunStep();
   afx_msg void OnPause();
   afx_msg void OnStop();

   afx_msg void OnTimeScale1X();
   afx_msg void OnTimeScale10X();
   afx_msg void OnTimeScale100X();

   //afx_msg void OnAutoPause();
   afx_msg void OnChooseScenario();
   afx_msg void OnProcessMap();
   afx_msg void OnProcessMapObj();
   afx_msg void OnProcessSimplePaths();
   afx_msg void OnProcessOptimizedPaths();
   afx_msg void OnDebug();
   afx_msg void OnFalse();

   afx_msg void OnRunUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void OnStepUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void OnPauseUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void OnStopUpdateCommandUI(CCmdUI* pCmdUI);

   afx_msg void On1XUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void On10XUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void On100XUpdateCommandUI(CCmdUI* pCmdUI);

   afx_msg void OnChooseScenarioUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void OnProcessMapUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void OnProcessMapObjUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void OnProcessSimplePathsUpdateCommandUI(CCmdUI* pCmdUI);
   afx_msg void OnProcessOptimizedPathsUpdateCommandUI(CCmdUI* pCmdUI);

   afx_msg void OnCheckEngineUpdateCommandUI(CCmdUI* pCmdUI);

   DECLARE_MESSAGE_MAP()

private:
};

extern CSurfaceViewerGen2App theApp;