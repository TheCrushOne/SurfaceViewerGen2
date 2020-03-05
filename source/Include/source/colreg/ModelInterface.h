/*!
\file
\brief Заголовочный файл ModelInterface.h
Определяет интерфейс для работы с моделью
*/
#pragma once
#include "CommonStructs.h"
#include "ModelStructs.h"
#include "CommonRefs.h"
#include <functional>
#include <memory>
#include "WeatherInterface.h"
#include "DebugStructs.h"

namespace colreg
{
}

#ifdef COLREGDLL_EXPORTS
#define ROUTE_MODEL_EXPORT_IMPORT __declspec(dllexport) // export DLL information
#else
#define ROUTE_MODEL_EXPORT_IMPORT __declspec(dllimport) // import DLL information
#endif