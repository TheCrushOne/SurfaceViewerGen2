
// SurfaceViewerGen2.h: основной файл заголовка для приложения SurfaceViewerGen2
//
#pragma once

#ifndef __AFXWIN_H__
   #error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы
#include "colreg\ModuleGuard.h"

// CSurfaceViewerGen2App:
// Сведения о реализации этого класса: SurfaceViewerGen2.cpp
//

class CSurfaceViewerGen2App : public CWinAppEx
{
public:
   CSurfaceViewerGen2App() noexcept;
protected:
   void createDirectXApp();
// Переопределение
public:
   virtual BOOL InitInstance();
   virtual BOOL OnIdle(LONG lCount);
   virtual int ExitInstance();

// Реализация
   UINT  m_nAppLook;
   BOOL  m_bHiColorIcons;

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
   afx_msg void OnRuler();
   afx_msg void OnSelect();
   afx_msg void OnCreate();
   afx_msg void OnEdit();
   afx_msg void OnTimeScale1X();
   afx_msg void OnTimeScale10X();
   afx_msg void OnTimeScale100X();
   afx_msg void OnEnableTrafficStatistic();
   afx_msg void OnAutoPause();
   afx_msg void OnDelete();
   afx_msg void OnRecord();
   afx_msg void OnShowRelations();
   afx_msg void OnUploadDangerStatistic();
   afx_msg void OnDebug();
   afx_msg void OnFalse();

   DECLARE_MESSAGE_MAP()

private:
};

extern CSurfaceViewerGen2App theApp;