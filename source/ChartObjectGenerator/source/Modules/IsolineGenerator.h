#pragma once

#include "common/central_class.h"
#include "common/servicable.h"
#include "common/chart_object_unit.h"
#include "ModuleBase.h"
#include "math/math_utils.h"
#include "IsolineAlgorithms\LabirinthTraverse.h"
#include "IsolineAlgorithms\SegmentCollector.h"
#include "IsolineAlgorithms\WaveFrontline.h"

namespace SV::chart_object
{
   enum class AlgorithmType : unsigned short
   {
      AT_LABTRV = 0,
      AT_SEGCOL,
      AT_WAVEFL,
   };

   class IsolineGenerator : public ModuleBase
   {
   public:
      IsolineGenerator(central_pack* pack, navigation_dispatcher::iComService* service);
      void GenerateIsolines(const pathfinder::GeoMatrix&, chart_object_unit_vct&);
   private:
      chart_object::chart_object_unit_vct generateIsolineLevel(AlgorithmType type, const pathfinder::GeoMatrix& rawdata, double height, int H);

      void addChartObjectSet(const CG::geo_contour_vct& data, double height, int H);
   private:
      std::unique_ptr<LabirinthTraverse> m_labirinthTraverseAlgorithm;
      std::unique_ptr<SegmentCollector> m_segmentCollectorAlgorithm;
      std::unique_ptr<WaveFrontline> m_waveFrontlineAlgortihm;
   };
}