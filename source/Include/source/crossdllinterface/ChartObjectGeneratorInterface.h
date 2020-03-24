#pragma once

#include "SurfaceInterfaces.h"
#include "common/file_storage.h"
#include "common/communicator.h"
#include "common/converter_structs.h"
#include "colreg/ChartSafetyStructs.h"
#include "common/central_class.h"

namespace chart_object
{
   struct iGenerator : colreg::iReleasable
   {
      virtual bool Init(std::shared_ptr<central_pack>) = 0;
      virtual bool GenerateStatic(const converter::raw_data_ref&) = 0;
      virtual const colreg::chart_objects_ref& GetChartObjects() const = 0;
   };
}

#ifdef CHARTOBJECTGEN_DLL
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT chart_object::iGenerator * CreateGenerator();