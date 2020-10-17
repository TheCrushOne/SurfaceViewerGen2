#pragma once
#include "CommonStructs.h"
#include "CommonRefs.h"
#include "ColregSettings.h"
#include "ModelInterface.h"
#include "DomainInterface.h"
#include "DebugStructs.h"
#include "SVCG/base_geometry.h"

struct ICommunicator;

//namespace colreg
//{
//#pragma pack (push, 1)
//   using ids_ref                 = base_ref<SVCG::id_type>;
//   using maneuver_route_info_ref = base_ref<maneuver_route_info>;
//   using model_name_ref          = base_ref<const char*>;
//#pragma pack (pop)
//}

#ifdef COLREGDLL_EXPORTS
#define EXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define EXPRTIMPRT __declspec(dllimport) // import DLL information
#endif