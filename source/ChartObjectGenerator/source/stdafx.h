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

#include <atldef.h>

// HACK: �������������� ��� � ���� ��� ����� � ���
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