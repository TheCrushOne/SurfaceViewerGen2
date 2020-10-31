#include "stdafx.h"
#include "math/math_utils.h"
#include "SVCG/positioning.h"
#include "LayerUnitImpl.h"

using namespace SV;
using namespace SV::surface_simulation;

const CG::layer_provider::trajectory_point_vct& LayerUnitImpl::GetRoute(ROUTE_TYPE type) const
{
   switch (type)
   {
      //case ROUTE_TYPE::RT_COLREG:   return GetModelPath();
      //case ROUTE_TYPE::RT_SIMULAION:return GetSimulationPath();
   case ROUTE_TYPE::RT_CONTROL:   return GetSrcControlPoints();
   case ROUTE_TYPE::RT_SOURSE:   return GetSrcPath();
   }
}

void LayerUnitImpl::createDomain(double radius)
{
   /*m_domainPts.resize(1);
   size_t pointCount = 10;
   for (double angle = 0.; angle <= 360.; angle += 360. / (double)pointCount)
      m_domainPts[0].emplace_back(colreg::polar{angle, radius});
   m_domain.emplace_back(colreg::domain_geometry{ colreg::DOMAIN_GEOMETRY_TYPE::DOGT_SECTOR, colreg::polar_ref(&m_domainPts[0][0], m_domainPts[0].size()) });

   m_domainRef = colreg::domain_geometry_ref(&m_domain[0], m_domain.size());*/
}

void LayerUnitImpl::SetSrcRoute(const settings::route& src)
{
   const auto& env_stt = m_settings.env_stt;
   for (const auto& cpt : src.control_point_list)
      m_controlPoints.emplace_back(transfercase::RoutePointToPositionPoint(cpt, env_stt));
   for (const auto& rpt : src.route_list)
      m_srcRoute.emplace_back(transfercase::RoutePointToPositionPoint(rpt, env_stt));
   for (size_t idx = 0; idx < m_srcRoute.size(); idx++)
   {
      auto& cur = m_srcRoute[idx];
      auto& next = m_srcRoute[idx < m_srcRoute.size() - 1 ? idx + 1 : idx];
      cur.course = math::direction(cur.pos, next.pos);
      cur.speed = DefaultUnitSpeed;   // NOTE: пока что дефолт
   }
}