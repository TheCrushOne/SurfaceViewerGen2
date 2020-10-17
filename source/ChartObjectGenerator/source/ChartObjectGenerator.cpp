#include "stdafx.h"
#include "ChartObjectGenerator.h"
#include "SVCG/positioning.h"
#include "math/math_utils.h"
#include "navdisp\ComService.h"
#include "navdisp\OrderCreation.h"

#include <algorithm>
#include <limits>

using namespace chart_object;

ChartObjectGenerator::ChartObjectGenerator(central_pack *pack, navigation_dispatcher::iComService* service)
   : OrderBase(pack, service)
   , m_isolineGenerator(pack, service)
   , m_coverageGenerator(pack, service)
   , m_zoneGenerator(pack, service)
   , m_lock(false)
{}

bool ChartObjectGenerator::processCommand()
{
   if (m_lock)
      return false;

   auto* src = GetService()->GetDataStandartFactory()->GetDataStandart(m_commandData.source.AsString());
   auto* dst = GetService()->GetDataStandartFactory()->GetDataStandart(m_commandData.destination.AsString());

   if (!needToProcess())
      return true;

   if (!readFromSource(reinterpret_cast<data_standart::iSurfaceVieverGenMapDataStandart*>(src)))
      return false;

   if (!generateStatic())
      return false;

   if (!writeToDestination(reinterpret_cast<data_standart::iChartObjectDataStandart*>(dst)))
      return false;

   if (!recordOrderHashResult())
      return false;

   return true;
}

bool ChartObjectGenerator::readFromSource(data_standart::iSurfaceVieverGenMapDataStandart* src)
{
   m_rawDataPtr = src->GetData();
   return true;
}

bool ChartObjectGenerator::writeToDestination(data_standart::iChartObjectDataStandart* dst)
{
   dst->SetData(m_staticObjectStorage, m_dynamicObjectStorage);
   return true;
}

bool ChartObjectGenerator::generateStatic()
{
   if (m_lock)
      return false;
   m_staticObjectStorage.clear();
   m_staticObjectStorage.emplace_back();
   m_coverageGenerator.GenerateChartBorder(m_rawDataPtr, m_staticObjectStorage.back());
   m_isolineGenerator.GenerateIsolines(m_rawDataPtr, m_staticObjectStorage);
   return true;
}

void ChartObjectGenerator::prepareLocalStorage()
{
   m_chartStorage.clear();
}

navigation_dispatcher::iOrderPtr CreateObjectListGenerator(central_pack_ptr pack, navigation_dispatcher::iComServicePtr pService)
{
   return new ChartObjectGenerator(pack, pService);
}