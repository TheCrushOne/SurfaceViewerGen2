// pch.h: ��� �������������� ���������������� ������������ ����.
// ������������� ���� ����� ������������� ������ ���� ���, ��� �������� ����������� ������.
// ��� ����� ������ �� ������ IntelliSense, ������� ������ ������� ��������� � ���������� ����.
// ������ ��������� ������ �� ����������� ����� ������ ����� ���������� ������ �������� � ��������� ���������� ����(!) ���� ������.
// �� ���������� ���� �����, ������� ���������� ����� ��������, ��� ��� � ���� ������ �������� � ������������������ �� �����.



// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atldef.h>
#include <atlbase.h>
#include <atlstr.h>

#define _USE_MATH_DEFINES // for C++
#include  <cmath>
// HACK: �������������� ��� � ���� ��� ����� � ���
#undef min
#undef max

#include "common/header_collector.h"

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <xutility>
#include <comdef.h>