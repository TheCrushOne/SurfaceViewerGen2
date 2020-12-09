#include "stdafx.h"
#include "EngineBase.h"

using namespace SV;
using namespace SV::engine;

EngineBase::EngineBase(central_pack* pack)
   : Central(pack)
   , m_rawdata(std::make_shared<pathfinder::RoutePointMatrix>(CG::route_point{}))
   , m_pathfinder(std::make_unique<pathfinder::PathFinderPipeline>(pack))
{}

void EngineBase::processPathFindInternal(const pathfinder::path_finder_indata& scenarioData, std::function<void(void)> completeCallback)
{
   m_indata = std::make_shared<pathfinder::path_finder_indata>(scenarioData);
   // NOTE: радиус пока что тут настраивается
   m_indata->strategy_settings = pathfinder::strategy_settings{ pathfinder::StrategyType::ST_RHOMBOID, 5. };
   m_indata->settings = pathfinder::path_finder_settings{};
   m_indata->settings.multithread = true;
   m_pathfinder->FindPath([completeCallback]() { completeCallback(); }, m_settings, m_rawdata, m_indata);
   //using namespace std::chrono_literals;
   //std::this_thread::sleep_for(3ms);
   //completeCallback();
}