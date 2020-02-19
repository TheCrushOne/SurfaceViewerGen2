
// DXExternal.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CDXExternalApp:
// Сведения о реализации этого класса: DXExternal.cpp
//

class CDXExternalApp : public CWinApp
{
public:
	CDXExternalApp();

// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CDXExternalApp theApp;
