#include "stdafx.h"
#include "ChartObjectGenerator.h"
#include "SVCG/positioning.h"
#include "math/math_utils.h"
#include <algorithm>
#include <limits>

using namespace chart_object;

ChartObjectGenerator::ChartObjectGenerator()
{}

bool ChartObjectGenerator::Init(central_pack* pack)
{
   Central::Init(pack);
   m_isolineGenerator.Init(pack);
   m_coverageGenerator.Init(pack);
   m_zoneGenerator.Init(pack);
   auto creator = [this]()->chart_storage& { return generateNew(); };
   auto adder = [this](chart_storage& storage) { addChartObject(storage); };
   m_isolineGenerator.SetAdder(creator, adder);
   m_coverageGenerator.SetAdder(creator, adder);
   m_zoneGenerator.SetAdder(creator, adder);
   return true;
}

bool ChartObjectGenerator::GenerateStatic(const converter::raw_data_ref& rawdata)
{
   if (m_lock)
      return false;
   prepareLocalStorage();
   m_coverageGenerator.GenerateChartBorder(rawdata);
   m_isolineGenerator.GenerateIsolines(rawdata);
   return true;
}

const colreg::chart_objects_ref& ChartObjectGenerator::GetChartObjects() const
{
   prepareRef();
   return m_chartObjectRef;
}

void ChartObjectGenerator::prepareRef() const
{
   // HACK: ...но без него никак
   const_cast<ChartObjectGenerator*>(this)->m_chartObjectRef = colreg::chart_objects_ref(m_chartObjVct.data(), m_chartObjVct.size());
}

void ChartObjectGenerator::prepareLocalStorage()
{
   m_chartObjVct.clear();
   m_chartStorage.clear();
}

void ChartObjectGenerator::addChartObject(chart_storage& storage)
{
   static colreg::chart_object_id id = 0;
   colreg::object_props_ref propRef = colreg::object_props_ref(storage.prop_vct.data(), storage.prop_vct.size());
   colreg::object_geometry_ref geoRef = colreg::object_geometry_ref(storage.geom_contour_ref_vct.data(), storage.geom_contour_ref_vct.size());
   m_chartObjVct.emplace_back(colreg::chart_object(id++, storage.type, geoRef, propRef));
}

chart_object::iGenerator * CreateGenerator()
{
   return new chart_object::ChartObjectGenerator();
}