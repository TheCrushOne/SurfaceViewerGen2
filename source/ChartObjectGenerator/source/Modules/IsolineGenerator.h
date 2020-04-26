#pragma once

#include "common/central_class.h"
#include "chart_storage.h"
#include "ModuleBase.h"
#include "math/math_utils.h"
#include "IsolineAlgorithms\LabirinthTraverse.h"
#include "IsolineAlgorithms\SegmentCollector.h"
#include "IsolineAlgorithms\WaveFrontline.h"

namespace chart_object
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
      IsolineGenerator();
      void Init(central_pack* pack);
      void GenerateIsolines(const converter::raw_data_ref& rawdata);
   private:
      void generateIsolineLevel(AlgorithmType type, const converter::raw_data_ref& rawdata, double height, int H);

      void addChartObjectSet(const std::vector<math::geo_points>& data, double height, int H);
   private:
      std::unique_ptr<LabirinthTraverse> m_labirinthTraverseAlgorithm;
      std::unique_ptr<SegmentCollector> m_segmentCollectorAlgorithm;
      std::unique_ptr<WaveFrontline> m_waveFrontlineAlgortihm;
   };
}