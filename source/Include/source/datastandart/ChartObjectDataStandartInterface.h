#pragma once
#include "common/chart_object.h"
#include "DataStandart.h"
#include "DataStandartInterfaceBase.h"
#include "navdisp/ComService.h"

namespace data_standart
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
      virtual void SetData(const chart_object::chart_object_unit_vct_ref, const chart_object::chart_object_unit_vct_ref) = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreateChartObjectDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);