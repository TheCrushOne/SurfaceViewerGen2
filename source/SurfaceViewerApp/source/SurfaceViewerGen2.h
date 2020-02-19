
// SurfaceViewerGen2.h: основной файл заголовка для приложения SurfaceViewerGen2
//
#pragma once

#ifndef __AFXWIN_H__
   #error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы
#include "colreg\ModuleGuard.h"
#include "crossdllinterface\TransceiverInterface.h"

// CSurfaceViewerGen2App:
// Сведения о реализации этого класса: SurfaceViewerGen2.cpp
//

class CSurfaceViewerGen2App : public CWinAppEx
{
public:
   CSurfaceViewerGen2App() noexcept;
protected:
   void createDirectXApp();
   void createTransceiver();
   void initTransceiver();
   void callback(const char*);
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
   DECLARE_MESSAGE_MAP()

private:
   colreg::ModuleGuard<iTransceiver> m_transceiver;
};

extern CSurfaceViewerGen2App theApp;