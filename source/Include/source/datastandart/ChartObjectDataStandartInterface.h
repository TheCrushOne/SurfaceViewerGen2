#pragma once
#include "common/chart_object.h"
#include "DataStandart.h"
#include "DataStandartInterfaceBase.h"
#include "navdisp/ComService.h"

namespace data_standart
{
   typedef std::vector<chart_object::geometry_chart_object> geom_object_vec;

   struct geom_object_vec_junc
   {
      geom_object_vec static_objects;
      geom_object_vec dynamic_objects;
   };

   struct iChartObjectDataStandart : public iDataStandart
   {
      // Common
      // Read
      virtual const geom_object_vec_junc& GetData() = 0;
      // Write
      virtual void SetData(const geom_object_vec&, const geom_object_vec&) = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT data_standart::iDataStandart * CreateChartObjectDataStandart(central_pack * pack, LPCWSTR base_folder, navigation_dispatcher::iComService * pService);