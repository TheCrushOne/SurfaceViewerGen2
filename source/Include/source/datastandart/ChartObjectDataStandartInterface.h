#pragma once
#include "common/chart_object_unit.h"
#include "DataStandart.h"
#include "DataStandartInterfaceBase.h"
#include "navdisp/ComService.h"

namespace SV::data_standart
{
   struct geom_object_vec_junc
   {
      chart_object::chart_object_unit_vct static_objects;
      chart_object::chart_object_unit_vct dynamic_objects;
   };

   struct iChartObjectDataStandart : public iDataStandart
   {
      // Common
      // Read
      virtual const geom_object_vec_junc& GetData() = 0;
      // Write
      virtual void SetData(const chart_object::chart_object_unit_vct&, const chart_object::chart_object_unit_vct&) = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT SV::data_standart::iDataStandart * CreateChartObjectDataStandart(SV::central_pack * pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService * pService);