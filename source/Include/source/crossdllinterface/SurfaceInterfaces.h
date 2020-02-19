#pragma once

#include "colreg/Baseref.h"
#include "common/settings.h"

#ifdef _USRDLL
#define EXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define EXPRTIMPRT __declspec(dllimport) // import DLL information
#endif