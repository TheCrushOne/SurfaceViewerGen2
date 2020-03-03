#pragma once

#include "ReleaseGuard.h"
#include "BaseRefHolder.h"
#include "ColregInterfaces.h"
#include "ChartSafetyInterface.h"

using NavEstimationGuard = colreg::ReleaseGuard<colreg::iNavEstimation>;

using ModelTrackGuard   = colreg::ReleaseGuard<colreg::iModelTrack>;
using ChartContextGuard = colreg::ReleaseGuard<colreg::iAnalyzeResult>;

using RouteRefHolder     = BaseRefHolder<colreg::route_point>;
using RouteInfoRefHolder = BaseRefHolder<colreg::maneuver_route_info>;
using RelationsHolder    = BaseRefHolder<colreg::ship_relation_info>;

using SolutionManeuverRefHolder = BaseRefHolder<colreg::iSolutionManeuver*>;

using ColregRoutePoints  = std::vector<colreg::route_point>;
using ColregTrackPoints  = std::vector<colreg::track_point_info>;
using ColregModelInfo    = std::vector<colreg::model_point_info>;
using ColregChartContext = std::vector<colreg::chart_context>;
using ColregDomainBorder = std::vector<colreg::domain_border_info>;

class TrackFullInfoRefHolder : public colreg::track_full_info_ref
{
public:
   TrackFullInfoRefHolder() = default;

   TrackFullInfoRefHolder(ColregTrackPoints&& track, ColregModelInfo&& modelInfo,
                          ColregChartContext&& chartContext, ColregDomainBorder&& domainBorder)
      : colreg::track_full_info_ref(track.data(),
                                    modelInfo.empty()    ? nullptr : modelInfo.data(),
                                    chartContext.empty() ? nullptr : chartContext.data(),
                                    domainBorder.empty() ? nullptr : domainBorder.data(),
                                    track.size())
      , _track(std::move(track))
      , _modelInfo(std::move(modelInfo))
      , _chartContext(std::move(chartContext))
      , _domainBorder(std::move(domainBorder))
   {}

private:
   ColregTrackPoints  _track;
   ColregModelInfo    _modelInfo;
   ColregChartContext _chartContext;
   ColregDomainBorder _domainBorder;
};