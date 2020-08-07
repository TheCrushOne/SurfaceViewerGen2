#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <atldef.h>

// HACK: переопределяем мин и макс для ухода к стл
#undef min
#undef max

#include "common/header_collector.h"