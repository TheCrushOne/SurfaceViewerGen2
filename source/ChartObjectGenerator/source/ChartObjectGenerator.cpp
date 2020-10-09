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
{
   init();
}

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

void ChartObjectGenerator::init()
{
   /*auto creator = [this]()->geometry_chart_object& { return generateNew(); };
   auto adder = [this](geometry_chart_object& storage) { addChartObject(storage); };
   m_isolineGenerator.SetAdder(creator, adder);
   m_coverageGenerator.SetAdder(creator, adder);
   m_zoneGenerator.SetAdder(creator, adder);*/
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

//const colreg::chart_objects_ref& ChartObjectGenerator::getChartObjects() const
//{
//   prepareRef();
//   return m_chartObjectRef;
//}

//void ChartObjectGenerator::prepareRef() const
//{
//   // HACK: ...�� ��� ���� �����
//   const_cast<ChartObjectGenerator*>(this)->m_chartObjectRef = colreg::chart_objects_ref(m_chartObjVct.data(), m_chartObjVct.size());
//}

void ChartObjectGenerator::prepareLocalStorage()
{
   //m_chartObjVct.clear();
   m_chartStorage.clear();
}

void ChartObjectGenerator::addChartObject(chart_object::chart_object_unit& storage)
{
   // NOTE: ����� ��� ��������
   ATLASSERT(false);
   //static colreg::chart_object_id id = 0;
   //colreg::object_props_ref propRef = colreg::object_props_ref(storage.prop_vct.data(), storage.prop_vct.size());
   //colreg::object_geometry_ref geoRef = colreg::object_geometry_ref(storage.geom_contour_ref_vct.data(), storage.geom_contour_ref_vct.size());
   //m_chartObjVct.emplace_back(colreg::chart_object(id++, storage.type, geoRef, propRef));
}

navigation_dispatcher::iOrderPtr CreateObjectListGenerator(central_pack_ptr pack, navigation_dispatcher::iComServicePtr pService)
{
   return new ChartObjectGenerator(pack, pService);
}