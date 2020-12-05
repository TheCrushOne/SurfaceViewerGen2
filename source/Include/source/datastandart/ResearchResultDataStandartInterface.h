#pragma once
#include "DataStandart.h"
#include "common/pathfinder_structs.h"
#include "navdisp/ComService.h"

namespace SV::data_standart
{
   struct iResearchResultDataStandart : public iDataStandart
   {
      //virtual void SetData(const pathfinder::GeoMatrix&) = 0;
      //virtual pathfinder::GeoMatrix& GetData() = 0;
      //virtual settings::unit_source_data& GetUnitData() = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define RESEARCHRESULTEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define RESEARCHRESULTEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" RESEARCHRESULTEXPRTIMPRT SV::data_standart::iDataStandart * CreateResearchResultDataStandart(SV::central_pack * pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService * pService);