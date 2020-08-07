#include "stdafx.h"
#include "IsolineGenerator.h"
#include "SVCG/positioning.h"
#include "common/pathfinder_structs.h"
#include <future>
#include "IsolineAlgorithms\AlgorithmHelpers.h"
#include <iostream>
#include <chrono>
#include <ctime>

#define LOCALMULTITHREAD
#define CURTIME_MS(time_ms) time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

using namespace chart_object;

IsolineGenerator::IsolineGenerator(central_pack* pack)
   : ModuleBase(pack)
{
   auto adder = [this](const std::vector<math::geo_points>& obj, double H, int height) { addChartObjectSet(obj, H, height); };
   m_labirinthTraverseAlgorithm = std::make_unique<LabirinthTraverse>(pack, adder);
   m_segmentCollectorAlgorithm = std::make_unique<SegmentCollector>(pack, adder);
   m_waveFrontlineAlgortihm = std::make_unique<WaveFrontline>(pack, adder);
}

void IsolineGenerator::GenerateIsolines(const pathfinder::GeoMatrix* rawdata)
{
   size_t levelCount = 10;
   double min = rawdata->Min(), max = rawdata->Max();

   double step = (max - min) / static_cast<double>(levelCount);
   //double height = 27.;
   //double height = 50.;
   AlgorithmType type = AlgorithmType::AT_WAVEFL;
   __int64 start;
   CURTIME_MS(start);
#ifdef LOCALMULTITHREAD
   std::vector<std::future<void>> futures;
   // NOTE: менять тип алгоритма тут
   for (size_t levelIdx = 0; levelIdx < levelCount; levelIdx++)
      futures.push_back(std::async(&IsolineGenerator::generateIsolineLevel, this, type, rawdata, min + step * static_cast<double>(levelIdx), 360. / static_cast<double>(levelCount)* static_cast<double>(levelIdx)));

   for (auto& future : futures)
      future.get();
#else
   //size_t levelIdx = 3;
   for (size_t levelIdx = 0; levelIdx < levelCount; levelIdx++)
      IsolineGenerator::generateIsolineLevel(type, rawdata, min + step * static_cast<double>(levelIdx), 360. / static_cast<double>(levelCount)* static_cast<double>(levelIdx));
#endif
   __int64 finish;
   CURTIME_MS(finish);
   GetPack()->comm->Message(ICommunicator::MessageType::MT_PERFORMANCE, (std::string("Isoline build time: ") + std::to_string(finish - start) + " ms.").c_str());
}

void IsolineGenerator::generateIsolineLevel(AlgorithmType type, const pathfinder::GeoMatrix* rawdata, double height, int H)
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
      // TODO: подкорректировать цвет
      int rgb[3];
      HSVtoRGB(H, 1., 1., rgb);
      char color[64];
      sprintf(color, "%i %i %i", rgb[0], rgb[1], rgb[2]);
      isoLineStorageCell.prop_vct.emplace_back("Color", color);
      isoLineStorageCell.prop_vct.emplace_back("Depth", std::to_string(height).c_str());
      /*for (auto& elem : isoLineStorageCell.prop_holder_vct)
         isoLineStorageCell.prop_vct.emplace_back(elem.key.c_str(), elem.val.c_str());*/
      auto& isoLineGeom = isoLineStorageCell.geom_contour_vct.back();
      for (auto& point : isoLine)
         isoLineGeom.emplace_back(point);
      //isoLineStorageCell.Commit();

      m_addObject(isoLineStorageCell);
   }
}