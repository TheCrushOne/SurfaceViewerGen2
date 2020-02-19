/**
  @file     cglMain.cpp
  @brief    WinMain container
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include <windows.h>

#include "directxapp.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>


/**
  @brief    This function is WinMain for D3DBase project

  @return   Windows error code
  @param    hInstance        standard WinMain input
  @param    hPrevInsatnce    standard WinMain input
  @param    lpszCommandLine  standard WinMain input
  @param    nCmdShow         standard WinMain input
  @author   Bvs
  @see      MSDN =)
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInsatnce,
                   LPSTR lpszCommandLine, int nCmdShow)
{
    std::unique_ptr<DirectXApp> app;
#ifdef _DEBUG
    AllocConsole();
    FILE *stream;       //фиктивный
    freopen_s(&stream, "CONIN$", "r", stdin);
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    //_CrtSetBreakAlloc(245);       //для поиска
#endif
    //app = std::make_unique<DirectXApp>(800, 600, (void*)hInstance, nCmdShow);      //800 на 600 (4:3)
    app = std::make_unique<DirectXApp>(1280, 1024, (void*)hInstance, nCmdShow);      //800 на 600 (4:3)

    if (!app->isFailed())
        app->theLoop();

#ifdef _DEBUG

    _CrtDumpMemoryLeaks();
    _RPT0(_CRT_WARN, "/////////////////////////////\nend of dump message\n");

    system("pause");
#endif
} // end of WinMain

