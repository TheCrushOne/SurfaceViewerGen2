#include "stdafx.h"
#include "ColregSimulationImpl.h"

using namespace ColregSimulation;

const ship_path_ref* SimulationUnit::GetRoute(ROUTE_TYPE type) const
{
   switch (type)
   {
   //case ROUTE_TYPE::RT_COLREG:   return GetModelPath();
   //case ROUTE_TYPE::RT_SIMULAION:return GetSimulationPath();
   case ROUTE_TYPE::RT_CONTROL:   return GetSrcControlPoints();
   case ROUTE_TYPE::RT_SOURSE:   return GetSrcPath();
   }

   return nullptr;
}

void SimulationUnit::createDomain(double radius)
{
   m_domainPts.resize(1);
   size_t pointCount = 10;
   for (double angle = 0.; angle <= 360.; angle += 360. / (double)pointCount)
      m_domainPts[0].emplace_back(colreg::polar{angle, radius});
   m_domain.emplace_back(colreg::domain_geometry{ colreg::DOMAIN_GEOMETRY_TYPE::DOGT_SECTOR, colreg::polar_ref(&m_domainPts[0][0], m_domainPts[0].size()) });

   m_domainRef = colreg::domain_geometry_ref(&m_domain[0], m_domain.size());
}
