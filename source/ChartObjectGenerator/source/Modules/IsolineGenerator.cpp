#include "stdafx.h"
#include "IsolineGenerator.h"
#include "SVCG/positioning.h"
#include "common/pathfinder_structs.h"
#include <future>
#include "IsolineAlgorithms\AlgorithmHelpers.h"

#define LOCALMULTITHREAD

using namespace chart_object;

IsolineGenerator::IsolineGenerator()
{
   auto adder = [this](const std::vector<math::geo_points>& obj, double H, int height) { addChartObjectSet(obj, H, height); };
   m_labirinthTraverseAlgorithm = std::make_unique<LabirinthTraverse>(adder);
   m_segmentCollectorAlgorithm = std::make_unique<SegmentCollector>(adder);
   m_waveFrontlineAlgortihm = std::make_unique<WaveFrontline>(adder);
}

void IsolineGenerator::Init(central_pack* pack)
{
   Central::Init(pack);
   m_labirinthTraverseAlgorithm->Init(pack);
   m_segmentCollectorAlgorithm->Init(pack);
   m_waveFrontlineAlgortihm->Init(pack);
}

void IsolineGenerator::GenerateIsolines(const converter::raw_data_ref& rawdata)
{
   size_t levelCount = 10;
   double min = rawdata.arr[0].arr[0], max = rawdata.arr[0].arr[0];
   for (auto& row : rawdata)
   {
      for (auto& col : row)
      {
         if (col < min)
            min = col;
         if (col > max)
            max = col;
      }
   }
   double step = (max - min) / static_cast<double>(levelCount);
   //double height = 27.;
   //double height = 50.;
   AlgorithmType type = AlgorithmType::AT_WAVEFL;
#ifdef LOCALMULTITHREAD
   std::vector<std::future<void>> futures;
   // NOTE: ������ ��� ��������� ���
   for (size_t levelIdx = 0; levelIdx < levelCount; levelIdx++)
      futures.push_back(std::async(&IsolineGenerator::generateIsolineLevel, this, type, rawdata, min + step * static_cast<double>(levelIdx), 360. / static_cast<double>(levelCount)* static_cast<double>(levelIdx)));

   for (auto& future : futures)
      future.get();
#else
   //size_t levelIdx = 3;
   for (size_t levelIdx = 0; levelIdx < levelCount; levelIdx++)
      IsolineGenerator::generateIsolineLevel(type, rawdata, min + step * static_cast<double>(levelIdx), 360. / static_cast<double>(levelCount)* static_cast<double>(levelIdx));
#endif
}

void IsolineGenerator::generateIsolineLevel(AlgorithmType type, const converter::raw_data_ref& rawdata, double height, int H)
{
   switch(type)
   {
      case AlgorithmType::AT_LABTRV:
         m_labirinthTraverseAlgorithm->GenerateIsolineLevel(rawdata, height, H);
         break;
      case AlgorithmType::AT_SEGCOL:
         m_segmentCollectorAlgorithm->GenerateIsolineLevel(rawdata, height, H);
         break;
      case AlgorithmType::AT_WAVEFL:
         m_waveFrontlineAlgortihm->GenerateIsolineLevel(rawdata, height, H);
         break;
      default:
         ATLASSERT(false);
   }
}

void IsolineGenerator::addChartObjectSet(const std::vector<math::geo_points>& data, double height, int H)
{
   for (auto& isoLine : data)
   {
      chart_storage& isoLineStorageCell = m_genNewObjectRef();
      isoLineStorageCell.type = colreg::OBJECT_TYPE::OT_ISOLINE;
      isoLineStorageCell.geom_contour_vct.emplace_back();
      // TODO: ����������������� ����
      int rgb[3];
      HSVtoRGB(H, 1., 1., rgb);
      char color[64];
      sprintf(color, "%i %i %i", rgb[0], rgb[1], rgb[2]);
      isoLineStorageCell.prop_holder_vct.emplace_back("Color", color);
      isoLineStorageCell.prop_holder_vct.emplace_back("Depth", std::to_string(height).c_str());
      for (auto& elem : isoLineStorageCell.prop_holder_vct)
         isoLineStorageCell.prop_vct.emplace_back(elem.key.c_str(), elem.val.c_str());
      auto& isoLineGeom = isoLineStorageCell.geom_contour_vct.back();
      for (auto& point : isoLine)
         isoLineGeom.emplace_back(point);
      isoLineStorageCell.Commit();

      m_addObject(isoLineStorageCell);
   }
}