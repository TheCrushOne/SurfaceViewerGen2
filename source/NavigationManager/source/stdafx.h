// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.



// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _AFX_ALL_WARNINGS
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <afxwin.h>         // MFC core and standard components

#include <atldef.h>
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdisp.h>        // MFC Automation classes

// HACK: переопределяем мин и макс для ухода к стл
#undef min
#undef max

#include "common/header_collector.h"

#include <functional>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <xutility>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <math.h>
#include <iostream>
#include <fstream> 
#include <limits>
#include <comdef.h>