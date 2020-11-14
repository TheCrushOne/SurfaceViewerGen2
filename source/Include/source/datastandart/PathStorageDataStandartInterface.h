#pragma once
#include "DataStandart.h"
#include "navdisp/ComService.h"
#include "common/pathfinder_structs.h"

namespace SV::data_standart
{
   struct iPathStorageDataStandart : public iDataStandart
   {
      virtual const pathfinder::route_data& GetData() const = 0;
      virtual const pathfinder::UnsignedMatrix& GetLandUnitExplication() const = 0;
      virtual const pathfinder::UnsignedMatrix& GetAirUnitExplication() const = 0;
      virtual const std::vector<pathfinder::UnsignedMatrix>& GetCoverageHistory() const = 0;

      virtual void SetData(const pathfinder::route_data& paths, const pathfinder::UnsignedMatrix& land, const pathfinder::UnsignedMatrix& air, const std::vector<pathfinder::SharedUnsignedMatrix>& coverages) = 0;
   };
}

#ifdef DATASTANDART_EXPORTS
#define CHARTOBJEXPRTIMPRT __declspec(dllexport) // export DLL information
#else
#define CHARTOBJEXPRTIMPRT __declspec(dllimport) // import DLL information
#endif

extern "C" CHARTOBJEXPRTIMPRT SV::data_standart::iDataStandart * CreatePathStorageDataStandart(SV::central_pack * pack, LPCSTR base_folder, SV::navigation_dispatcher::iComService * pService);