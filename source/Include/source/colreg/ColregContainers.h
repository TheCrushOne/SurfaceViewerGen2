#pragma once

#include "ReleaseGuard.h"
#include "BaseRefHolder.h"
#include "ColregInterfaces.h"
#include "ChartSafetyInterface.h"


//using RouteRefHolder     = BaseRefHolder<colreg::route_point>;
//using RouteInfoRefHolder = BaseRefHolder<colreg::maneuver_route_info>;
//
//using ColregRoutePoints  = std::vector<colreg::route_point>;
//using ColregTrackPoints  = std::vector<colreg::track_point_info>;
//using ColregModelInfo    = std::vector<colreg::model_point_info>;
//using ColregDomainBorder = std::vector<colreg::domain_border_info>;
//
//class TrackFullInfoRefHolder : public colreg::track_full_info_ref
//{
//public:
//   TrackFullInfoRefHolder() = default;
//
//   TrackFullInfoRefHolder(ColregTrackPoints&& track, ColregModelInfo&& modelInfo,
//                          ColregDomainBorder&& domainBorder)
//      : colreg::track_full_info_ref(track.data(),
//                                    modelInfo.empty()    ? nullptr : modelInfo.data(),
//                                    domainBorder.empty() ? nullptr : domainBorder.data(),
//                                    track.size())
//      , _track(std::move(track))
//      , _modelInfo(std::move(modelInfo))
//      , _domainBorder(std::move(domainBorder))
//   {}
//
//private:
//   ColregTrackPoints  _track;
//   ColregModelInfo    _modelInfo;
//   ColregDomainBorder _domainBorder;
//};