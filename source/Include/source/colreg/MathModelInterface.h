/*!
\file
\brief ������������ ���� MathModelInterface.h
����������� ����������� ��� ������ � �������������� ������� �������� �������
*/

#pragma once
#include "BaseRef.h"
#include "CommonStructs.h"
#include "WeatherInterface.h"

struct ICommunicator;

namespace colreg
{
}

#ifdef COLREGDLL_EXPORTS
#define MATH_MODEL_EXPORT_IMPORT __declspec(dllexport) // export DLL information
#else
#define MATH_MODEL_EXPORT_IMPORT __declspec(dllimport) // import DLL information
#endif