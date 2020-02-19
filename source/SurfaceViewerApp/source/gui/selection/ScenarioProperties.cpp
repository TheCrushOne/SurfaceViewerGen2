#include "stdafx.h"
#include "ScenarioProperties.h"
#include ""

#define CRSCENSETT(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(_prop_settings[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioSettingsMeta, sifi, &ScenarioProperties::OnSettingChanged)
#define CRSCENENV(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(_prop_environment[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioEnvironmentMeta, sifi, &ScenarioProperties::OnEnvironmentChanged)
#define CRSCENRUN(prStruct, obj, field, sifi) PROPHELPER_CREATEHOLDER_S(_prop_run[static_cast<size_t>(sifi)], prStruct, obj, field, ScenarioProperties::scenarioRunMeta, sifi, &ScenarioProperties::OnRunChanged)

ScenarioProperties::ScenarioProperties()
{
   const auto* disp = dispatcher::getDispatcher();
   const auto* sim = simulator::getSimulator();
   if (sim)
   {
      _description = sim->GetDescription();
      _aisLogFilePath = sim->GetAisLogFilePath();
      _mapFolderPath = sim->GetMapFolderPath();
      const auto& simulationState = sim->GetState();

      _settings = simulationState.GetSettings();
      _environment = simulationState.GetEnvironment();
      _run = simulationState.GetSimulationSettings();
      prepareScenarioSettingsFolder();
      prepareScenarioEnvironmentFolder();
      prepareScenarioRunFolder();
   }
}

void ScenarioProperties::prepareScenarioSettingsFolder()
{
   _scenario_settings_folder = std::make_unique<FolderProperty>("COLREG settings");
   _prop_settings.resize(ScenarioSettingsFieldIndex::SSFI_END);

   CRSCENSETT(colreg::settings, _settings, timeStep, ScenarioSettingsFieldIndex::SSFI_TIMESTEP);
   PROPHELPER_CREATEHOLDER_S(_prop_description, ScenarioProperties, (*this), _description, ScenarioProperties::customSettingsMeta, CustomSettingsFieldIndex::CSFI_DESCRIPTION, &ScenarioProperties::OnDescriptionChanged);
   PROPHELPER_CREATEHOLDER_S(_prop_aisLogFilePath, ScenarioProperties, (*this), _aisLogFilePath, ScenarioProperties::customSettingsMeta, CustomSettingsFieldIndex::CSFI_AISLOGPATH, &ScenarioProperties::OnAisLogPathChanged);
   PROPHELPER_CREATEHOLDER_S(_prop_mapFolderPath, ScenarioProperties, (*this), _mapFolderPath, ScenarioProperties::customSettingsMeta, CustomSettingsFieldIndex::CSFI_MAPFOLDERPATH, &ScenarioProperties::OnMapFolderPathChanged);


   CRSCENSETT(colreg::settings, _settings, timePrediction, ScenarioSettingsFieldIndex::SSFI_TIMEPREDICTION);
   CRSCENSETT(colreg::settings, _settings, alarmTimePercent, ScenarioSettingsFieldIndex::SSFI_ALARMTIMEPRCNT);
   CRSCENSETT(colreg::settings, _settings, searchStrategy, ScenarioSettingsFieldIndex::SSFI_SEARCHSTRAT);
   _prop_settings[ScenarioSettingsFieldIndex::SSFI_SEARCHSTRAT]->set_list(searchStats);
   CRSCENSETT(colreg::settings, _settings, colregMode, ScenarioSettingsFieldIndex::SSFI_STATEFULMODE);
   _prop_settings[ScenarioSettingsFieldIndex::SSFI_STATEFULMODE]->set_list(colregModes);
   CRSCENSETT(colreg::settings, _settings, checkRouteMode, ScenarioSettingsFieldIndex::SSFI_CHECKROUTEMODE);
   CRSCENSETT(colreg::settings, _settings, circulationRadiusFactor, ScenarioSettingsFieldIndex::SSFI_CIRRADFACTOR);
   CRSCENSETT(colreg::settings, _settings, emergencyManeuverEnable, ScenarioSettingsFieldIndex::SSFI_EMERGMANEUVER);
   CRSCENSETT(colreg::settings, _settings, getOutOfDangerEnable, ScenarioSettingsFieldIndex::SSFI_GETOUTOFDNGR);

   CRSCENSETT(colreg::settings, _settings, avoidTurnPenalty, ScenarioSettingsFieldIndex::SSFI_AVOIDTURNPEN);
   CRSCENSETT(colreg::settings, _settings, uturnPenalty, ScenarioSettingsFieldIndex::SSFI_UTURNPEN);
   CRSCENSETT(colreg::settings, _settings, sharpAnglePenalty, ScenarioSettingsFieldIndex::SSFI_SHARPANGLEPEN);
   CRSCENSETT(colreg::settings, _settings, speedIncreasePenalty, ScenarioSettingsFieldIndex::SSFI_SPEEDINCPEN);
   CRSCENSETT(colreg::settings, _settings, speedDecreasePenalty, ScenarioSettingsFieldIndex::SSFI_SPEEDDECPEN);
   CRSCENSETT(colreg::settings, _settings, crossCoursePenalty, ScenarioSettingsFieldIndex::SSFI_CROSSCRSPEN);
   CRSCENSETT(colreg::settings, _settings, cirRadPenalty, ScenarioSettingsFieldIndex::SSFI_CIRRADPEN);
   CRSCENSETT(colreg::settings, _settings, turnLeftPenalty, ScenarioSettingsFieldIndex::SSFI_TURNLEFTPEN);
   CRSCENSETT(colreg::settings, _settings, turnRightPenalty, ScenarioSettingsFieldIndex::SSFI_TURNRIGHTPEN);
   CRSCENSETT(colreg::settings, _settings, returnTimePenalty, ScenarioSettingsFieldIndex::SSFI_RETTIMEPEN);
   CRSCENSETT(colreg::settings, _settings, domainType, ScenarioSettingsFieldIndex::SSFI_DOMAINTYPE);
   _prop_settings[ScenarioSettingsFieldIndex::SSFI_DOMAINTYPE]->set_list(domainTypes);
   CRSCENSETT(colreg::settings, _settings, cooperativeMode, ScenarioSettingsFieldIndex::SSFI_COOPMODE);

   CRSCENSETT(colreg::settings, _settings, useLastMomentManeuver, ScenarioSettingsFieldIndex::SSFI_USELASTMOMENT);
   CRSCENSETT(colreg::settings, _settings, defaultShipWidth, ScenarioSettingsFieldIndex::SSFI_DEFSHIPWDTH);
   CRSCENSETT(colreg::settings, _settings, defaultShipLength, ScenarioSettingsFieldIndex::SSFI_DEFSHIPLENGTH);
   CRSCENSETT(colreg::settings, _settings, defaultShipDraft, ScenarioSettingsFieldIndex::SSFI_DEFSHIPDRAFT);
   CRSCENSETT(colreg::settings, _settings, defaultLeftXTE, ScenarioSettingsFieldIndex::SSFI_DEFLEFTXTE);
   CRSCENSETT(colreg::settings, _settings, defaultRightXTE, ScenarioSettingsFieldIndex::SSFI_DEFRIGHTXTE);
   CRSCENSETT(colreg::settings, _settings, offsetLeftXTE, ScenarioSettingsFieldIndex::SSFI_OFFSETLEFTXTE);
   CRSCENSETT(colreg::settings, _settings, offsetRightXTE, ScenarioSettingsFieldIndex::SSFI_OFFSETRIGHTXTE);
   CRSCENSETT(colreg::settings, _settings, useLogging, ScenarioSettingsFieldIndex::SSFI_USELOGGING);
   CRSCENSETT(colreg::settings, _settings, zeroSpeedThreshold, ScenarioSettingsFieldIndex::SSFI_ZEROSPDTHRESHOLD);
   CRSCENSETT(colreg::settings, _settings, motionlessShipUseColreg, ScenarioSettingsFieldIndex::SSFI_MLESSSHIPUSECOLREG);
   CRSCENSETT(colreg::settings, _settings, useGoodMarinePractice, ScenarioSettingsFieldIndex::SSFI_USEGMP);
   CRSCENSETT(colreg::settings, _settings, allowShipXTD, ScenarioSettingsFieldIndex::SSFI_ALLOWSHIPXTD);
   CRSCENSETT(colreg::settings, _settings, useRouteXTEInManeuver, ScenarioSettingsFieldIndex::SSFI_USEROUTEXTEINMAN);

   CRSCENSETT(colreg::settings, _settings, routeUpdateSpeedEps, ScenarioSettingsFieldIndex::SSFI_ROUTEUPDATESPEEDEPS);
   CRSCENSETT(colreg::settings, _settings, routeSpeedUpdateStrategy, ScenarioSettingsFieldIndex::SSFI_ROUTEUPDATESPEEDSTRATEGY);

   CRSCENSETT(colreg::settings, _settings, smallXTEdetectKoef, ScenarioSettingsFieldIndex::SSFI_SMODE_SMALLXTEDETECTKOEF);
   CRSCENSETT(colreg::settings, _settings, smallXTEincreaseKoef, ScenarioSettingsFieldIndex::SSFI_SMODE_SMALLXTEINCREASEXTE);
   CRSCENSETT(colreg::settings, _settings, ignoreCurrentChartDangers, ScenarioSettingsFieldIndex::SSFI_SMODE_IGNORECHARTDANGERSALREADYON);
   CRSCENSETT(colreg::settings, _settings, shipRelRedLevelPercentStartsFrom, ScenarioSettingsFieldIndex::SSFI_SMODE_SHIPRELREDLEVELSTARTS);
   CRSCENSETT(colreg::settings, _settings, offsetTrajectoryByXTD, ScenarioSettingsFieldIndex::SSFI_SMODE_OFFSET_TRAJECTORY);
   CRSCENSETT(colreg::settings, _settings, loggingTime, ScenarioSettingsFieldIndex::SRFI_LOGGING_TIME);
   CRSCENSETT(colreg::settings, _settings, useWeather, ScenarioSettingsFieldIndex::SRFI_USE_WEATHER);
   CRSCENSETT(colreg::settings, _settings, useWeatherDanger, ScenarioSettingsFieldIndex::SRFI_USE_WEATHER_DANGERS);

   _prop_settings[ScenarioSettingsFieldIndex::SSFI_ROUTEUPDATESPEEDSTRATEGY]->set_list(routeSpeedUpdateStrategies);

   _scenario_settings_folder->AddChild(_prop_description.get());
   _scenario_settings_folder->AddChild(_prop_aisLogFilePath.get());
   _scenario_settings_folder->AddChild(_prop_mapFolderPath.get());

   for (size_t idx = 0; idx < _prop_settings.size(); idx++)
   {
      if (_prop_settings[idx].get())
      {
         _scenario_settings_folder->AddChild(_prop_settings[idx].get());
      }
   }

   _scenario_settings_penalties_folder = std::make_unique<FolderProperty>("COLREG penalties settings");
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, turnPenaltyRatio, ScenarioSettingsFieldIndex::SSFI_TURNPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, tssPenaltyRatio, ScenarioSettingsFieldIndex::SSFI_TSSPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, speedIncreasePenaltyRatio, ScenarioSettingsFieldIndex::SSFI_SPDINCPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, speedDecreasePenaltyRatio, ScenarioSettingsFieldIndex::SSFI_SPDDECPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, uturnPenaltyRatio, ScenarioSettingsFieldIndex::SSFI_UTURNPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, interceptionPenaltyRatio, ScenarioSettingsFieldIndex::SSFI_INTPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, startTimePenaltyRatio, ScenarioSettingsFieldIndex::SSFI_STTIMEPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, cirRadPenaltyRatio, ScenarioSettingsFieldIndex::SSFI_CIRRADPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, leftTurnPenaltyRatio, ScenarioSettingsFieldIndex::SSFI_LEFTTURNPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, rightTurnPenaltyRatio, ScenarioSettingsFieldIndex::SSFI_RIGHTTURNPENRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, passingDistRatio, ScenarioSettingsFieldIndex::SSFI_PASSINGDISTRATIO);
   CRSCENSETT(colreg::penalty_ratio_settings, _settings.penalties, lastMomenManeuverChangeSpeedManScale, ScenarioSettingsFieldIndex::SSFI_LMMCHANGESPDMANSCALE);

   for (size_t idx = SSFI_TURNPENRATIO; idx <= SSFI_LMMCHANGESPDMANSCALE; idx++)
      _scenario_settings_penalties_folder->AddChild(_prop_settings[idx].get());
   _scenario_settings_folder->AddChild(_scenario_settings_penalties_folder.get());

   _scenario_settings_gmp_folder = std::make_unique<FolderProperty>("COLREG gmp settings");
   CRSCENSETT(colreg::good_marine_practice_settings, _settings.gmp_settings, checkTargetAlarm, ScenarioSettingsFieldIndex::SSFI_CHECKTRGALARM);
   CRSCENSETT(colreg::good_marine_practice_settings, _settings.gmp_settings, checkCourseIntersection, ScenarioSettingsFieldIndex::SSFI_CHECKCRSINT);
   CRSCENSETT(colreg::good_marine_practice_settings, _settings.gmp_settings, safeCouseIntersection, ScenarioSettingsFieldIndex::SSFI_SFCRSINT);
   CRSCENSETT(colreg::good_marine_practice_settings, _settings.gmp_settings, maxPenalty, ScenarioSettingsFieldIndex::SSFI_MAXPENALTY);

   for (size_t idx = SSFI_CHECKTRGALARM; idx <= SSFI_MAXPENALTY; idx++)
      _scenario_settings_gmp_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_gmp_hcm_folder = std::make_unique<FolderProperty>("COLREG heading course maneuver settings");
   CRSCENSETT(colreg::gmp_heading_courses_maneuver_cfg, _settings.gmp_settings.headingCoursesPenalties, badHeadingMeneuver, ScenarioSettingsFieldIndex::SSFI_BADHEADINGMAN);
   CRSCENSETT(colreg::gmp_heading_courses_maneuver_cfg, _settings.gmp_settings.headingCoursesPenalties, secondTurnPenalty, ScenarioSettingsFieldIndex::SSFI_SECONDTURNPEN);
   CRSCENSETT(colreg::gmp_heading_courses_maneuver_cfg, _settings.gmp_settings.headingCoursesPenalties, thirdTurnPenalty, ScenarioSettingsFieldIndex::SSFI_THIRDTURNPEN);
   CRSCENSETT(colreg::gmp_heading_courses_maneuver_cfg, _settings.gmp_settings.headingCoursesPenalties, maxParallelAngleDelta, ScenarioSettingsFieldIndex::SSFI_MAXPARANGLEDELTA);
   CRSCENSETT(colreg::gmp_heading_courses_maneuver_cfg, _settings.gmp_settings.headingCoursesPenalties, offsetPenalty, ScenarioSettingsFieldIndex::SSFI_OFFSETPENALTY);

   for (size_t idx = SSFI_BADHEADINGMAN; idx <= SSFI_OFFSETPENALTY; idx++)
      _scenario_settings_gmp_hcm_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_gmp_folder->AddChild(_scenario_settings_gmp_hcm_folder.get());

   _scenario_settings_gmp_cr_folder = std::make_unique<FolderProperty>("COLREG crossing right settings");
   CRSCENSETT(colreg::gmp_crossing_right_cfg, _settings.gmp_settings.crossingRightPenalties, minPenalty, ScenarioSettingsFieldIndex::SSFI_MINPENALTY);
   CRSCENSETT(colreg::gmp_crossing_right_cfg, _settings.gmp_settings.crossingRightPenalties, turnToSternPenalty, ScenarioSettingsFieldIndex::SSFI_TURNTOSTERNPEN);
   CRSCENSETT(colreg::gmp_crossing_right_cfg, _settings.gmp_settings.crossingRightPenalties, leaveBehindPenalty, ScenarioSettingsFieldIndex::SSFI_LEAVEBEHINDPEN);

   for (size_t idx = SSFI_MINPENALTY; idx <= SSFI_LEAVEBEHINDPEN; idx++)
      _scenario_settings_gmp_cr_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_gmp_folder->AddChild(_scenario_settings_gmp_cr_folder.get());

   _scenario_settings_folder->AddChild(_scenario_settings_gmp_folder.get());

   _scenario_settings_algo_folder = std::make_unique<FolderProperty>("COLREG algo settings");
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, minStartTime, ScenarioSettingsFieldIndex::SSFI_MINSTARTTIME);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, prolongationTime, ScenarioSettingsFieldIndex::SSFI_PROLONGTIME);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, minTurnAngle, ScenarioSettingsFieldIndex::SSFI_MINTURNANGLE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, maxTurnAngle, ScenarioSettingsFieldIndex::SSFI_MAXTURNANGLE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, minTurnAngleFog, ScenarioSettingsFieldIndex::SSFI_MINTURNANGLEFOG);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, angleSearchStep, ScenarioSettingsFieldIndex::SSFI_ANGLESRCHSTEP);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, minRudderAngle, ScenarioSettingsFieldIndex::SSFI_MINRUDDERANGLE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, maxRudderAngle, ScenarioSettingsFieldIndex::SSFI_MAXRUDDERANGLE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, rudderAngleStep, ScenarioSettingsFieldIndex::SSFI_RUDDERANGLESTEP);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, distSearchStep, ScenarioSettingsFieldIndex::SSFI_DSTSRCHSTEP);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, useStartTimeReducing, ScenarioSettingsFieldIndex::SSFI_USESTTMREDUCE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, useDomainReducing, ScenarioSettingsFieldIndex::SSFI_USEDOMREDUCE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, passingDomainScale, ScenarioSettingsFieldIndex::SSFI_PASSDOMSCALE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, maneuversA1A2Enable, ScenarioSettingsFieldIndex::SSFI_MANA1A2ENABLE);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, optimizeVariantsSearch, ScenarioSettingsFieldIndex::SSFI_OPTVARSRCH);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, draughtThreshold, ScenarioSettingsFieldIndex::SSFI_DRAUGHTTHRESHOLD);
   CRSCENSETT(colreg::passing_algo_settings, _settings.algo_settings, brakingTimeThreshold, ScenarioSettingsFieldIndex::SSFI_BRKTIMETHRESHOLD);

   for (size_t idx = SSFI_MINSTARTTIME; idx <= SSFI_BRKTIMETHRESHOLD; idx++)
      _scenario_settings_algo_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_folder->AddChild(_scenario_settings_algo_folder.get());

   _scenario_settings_collision_folder = std::make_unique<FolderProperty>("COLREG collision settings");
   CRSCENSETT(colreg::colision_estimation_settings, _settings.collision_settings, considerRelativeMovement, ScenarioSettingsFieldIndex::SSFI_CONSIDERRELMOV);
   CRSCENSETT(colreg::colision_estimation_settings, _settings.collision_settings, sameDirectionDomainScale, ScenarioSettingsFieldIndex::SSFI_SMDIRDOMSCALE);
   CRSCENSETT(colreg::colision_estimation_settings, _settings.collision_settings, forwardDomainLengthScale, ScenarioSettingsFieldIndex::SSFI_FWDDOMLENGTHSCALE);
   CRSCENSETT(colreg::colision_estimation_settings, _settings.collision_settings, backwardDomainLengthScale, ScenarioSettingsFieldIndex::SSFI_BACKDOMLENGTHSCALE);
   CRSCENSETT(colreg::colision_estimation_settings, _settings.collision_settings, leftDomainLengthScale, ScenarioSettingsFieldIndex::SSFI_LEFTDOMLENGTHSCALE);
   CRSCENSETT(colreg::colision_estimation_settings, _settings.collision_settings, rightDomainLengthScale, ScenarioSettingsFieldIndex::SSFI_RIGHTDOMLENGTHSCALE);

   for (size_t idx = SSFI_CONSIDERRELMOV; idx <= SSFI_RIGHTDOMLENGTHSCALE; idx++)
      _scenario_settings_collision_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_folder->AddChild(_scenario_settings_collision_folder.get());

   _scenario_settings_stateful_folder = std::make_unique<FolderProperty>("COLREG statefull mode settings");
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, cleanUpInterval, ScenarioSettingsFieldIndex::SSFI_SMODE_CLEANUPINTERVAL);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, shipDataMaxCount, ScenarioSettingsFieldIndex::SSFI_SMODE_SHIPDATAMAXCOUNT);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, maxOutOufRouteTime, ScenarioSettingsFieldIndex::SSFI_SMODE_MAXOUTOFROUTEINTERVAL);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, maxBadDirectionTime, ScenarioSettingsFieldIndex::SSFI_SMODE_MAXBADDIRECTION);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, trackAnalyzeInterval, ScenarioSettingsFieldIndex::SSFI_SMODE_TRACKANALYZEINTERVAL);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, alwaysReportDangerTime, ScenarioSettingsFieldIndex::SSFI_SMODE_ALWAYSREPORTDANGERTIME);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, stableMovementInterval, ScenarioSettingsFieldIndex::SSFI_SMODE_STABLEMOVEMENTINTERVAL);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, stableSpeedEps, ScenarioSettingsFieldIndex::SSFI_SMODE_STABLESPEEDEPS);
   CRSCENSETT(colreg::stateful_settings, _settings.stateful_mode_settings, stableAngleEps, ScenarioSettingsFieldIndex::SSFI_SMODE_STABLEANGLEEPS);

   for (size_t idx = SSFI_SMODE_CLEANUPINTERVAL; idx <= SSFI_SMODE_STABLEANGLEEPS; idx++)
      _scenario_settings_stateful_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_folder->AddChild(_scenario_settings_stateful_folder.get());


   /////////// dange analyzer settings
   _scenario_danger_analyzer_folder = std::make_unique<FolderProperty>("Stateful Danger Analyzer Settings");

   CRSCENSETT(colreg::danger_analyzer_settings, _settings.dngr_analyzer_cfg, stableDangerTimeSeconds, ScenarioSettingsFieldIndex::SSFI_DAS_STABLEDANGERSECONDS);
   CRSCENSETT(colreg::danger_analyzer_settings, _settings.dngr_analyzer_cfg, maxDangerTimeDeltaSeconds, ScenarioSettingsFieldIndex::SSFI_DAS_DANGERTIMEDELTA);
   CRSCENSETT(colreg::danger_analyzer_settings, _settings.dngr_analyzer_cfg, stableDangerAreaDomainKoef, ScenarioSettingsFieldIndex::SSFI_DAS_STABLEDANGERAREADOMAINKOEF);
   CRSCENSETT(colreg::danger_analyzer_settings, _settings.dngr_analyzer_cfg, maxHistoryTime, ScenarioSettingsFieldIndex::SSFI_DAS_MAXHISTORYTIME);
   CRSCENSETT(colreg::danger_analyzer_settings, _settings.dngr_analyzer_cfg, maxCountOfSyntheticDangers, ScenarioSettingsFieldIndex::SSFI_DAS_SYNTHCOUNT);

   for (size_t idx = SSFI_DAS_STABLEDANGERSECONDS; idx <= SSFI_DAS_SYNTHCOUNT; idx++)
      _scenario_danger_analyzer_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_folder->AddChild(_scenario_danger_analyzer_folder.get());
   ////////////////////////////


   /////////////// Route Model Config
   _route_model_settings_folder = std::make_unique<FolderProperty>("Route Model Settings");

   CRSCENSETT(colreg::route_model_config, _settings.route_config, in_route_max_course_diff, ScenarioSettingsFieldIndex::SSFI_IN_ROUTEMAXCOURSEDIFF);
   CRSCENSETT(colreg::route_model_config, _settings.route_config, close_to_route_zone_koef, ScenarioSettingsFieldIndex::SSFI_CLOSETOROUTEZONEKOEF);
   CRSCENSETT(colreg::route_model_config, _settings.route_config, close_to_route_max_course_diff, ScenarioSettingsFieldIndex::SSFI_CLOSETOROUTEMAXCOURSEDIFF);
   CRSCENSETT(colreg::route_model_config, _settings.route_config, near_to_route_zone_koef, ScenarioSettingsFieldIndex::SSFI_NEARTOROUTEZONEKOEF);
   CRSCENSETT(colreg::route_model_config, _settings.route_config, min_zone_width_koef, ScenarioSettingsFieldIndex::SSFI_MINZONEWIDTHKOEF);
   CRSCENSETT(colreg::route_model_config, _settings.route_config, use_extended_route_zones, ScenarioSettingsFieldIndex::SSFI_USE_EXT_ZONES);

   for (size_t idx = SSFI_IN_ROUTEMAXCOURSEDIFF; idx <= SSFI_USE_EXT_ZONES; idx++)
      _route_model_settings_folder->AddChild(_prop_settings[idx].get());

   _scenario_settings_folder->AddChild(_route_model_settings_folder.get());
   /////////////////////////////////



   AddChild(_scenario_settings_folder.get());




}

void ScenarioProperties::prepareScenarioEnvironmentFolder()
{
   _scenario_environment_folder = std::make_unique<FolderProperty>("COLREG environment");
   _prop_environment.resize(ScenarioEnvironmentFieldIndex::SEFI_END);

   CRSCENENV(colreg::environment, _environment, fog, ScenarioEnvironmentFieldIndex::SEFI_FOG);
   CRSCENENV(colreg::environment, _environment, maxSpeed, ScenarioEnvironmentFieldIndex::SEFI_MAXSPEED);
   CRSCENENV(colreg::environment, _environment, distDanger, ScenarioEnvironmentFieldIndex::SEFI_DISTDANGER);
   CRSCENENV(colreg::environment, _environment, distSafe, ScenarioEnvironmentFieldIndex::SEFI_DISTSAFE);
   CRSCENENV(colreg::environment, _environment, engineMode, ScenarioEnvironmentFieldIndex::SEFI_ENGINEMODE);
   _prop_environment[ScenarioEnvironmentFieldIndex::SEFI_ENGINEMODE]->set_list(engineModes);

   for (size_t idx = SEFI_FOG; idx <= SEFI_ENGINEMODE; idx++)
      _scenario_environment_folder->AddChild(_prop_environment[idx].get());

   _scenario_environment_cs_folder = std::make_unique<FolderProperty>("COLREG environment chart settings");

   CRSCENENV(colreg::chart_safety_settings, _environment.chart_settings, obstacleDistSafe, ScenarioEnvironmentFieldIndex::SEFI_OBSTDISTSAFE);
   CRSCENENV(colreg::chart_safety_settings, _environment.chart_settings, debugNarrow, ScenarioEnvironmentFieldIndex::SEFI_DEBUGNARROW);
   CRSCENENV(colreg::chart_safety_settings, _environment.chart_settings, debugNarrowWidth, ScenarioEnvironmentFieldIndex::SEFI_DEBUGNARROWWIDTH);
   CRSCENENV(colreg::chart_safety_settings, _environment.chart_settings, analysisObjectsMask, ScenarioEnvironmentFieldIndex::SEFI_ANALYSISOBJMASK);

   for (size_t idx = SEFI_OBSTDISTSAFE; idx <= SEFI_ANALYSISOBJMASK; idx++)
      _scenario_environment_cs_folder->AddChild(_prop_environment[idx].get());

   _scenario_environment_folder->AddChild(_scenario_environment_cs_folder.get());

   AddChild(_scenario_environment_folder.get());
}

void ScenarioProperties::prepareScenarioRunFolder()
{
   _scenario_run_folder = std::make_unique<FolderProperty>("COLREG simulation settings");
   _prop_run.resize(ScenarioRunFieldIndex::SRFI_END);

   CRSCENRUN(ColregSimulation::simulation_settings, _run, ownship_id, ScenarioRunFieldIndex::SRFI_OWNSHIP_MODE);
   CRSCENRUN(ColregSimulation::simulation_settings, _run, simulationTimeStep, ScenarioRunFieldIndex::SRFI_TIME_STEP);
   CRSCENRUN(ColregSimulation::simulation_settings, _run, estimationInterval, ScenarioRunFieldIndex::SRFI_ESTIMATION_INTERVAL);
   CRSCENRUN(ColregSimulation::simulation_settings, _run, auto_apply, ScenarioRunFieldIndex::SRFI_AUTO_APLY);
   CRSCENRUN(ColregSimulation::simulation_settings, _run, extend_route_last_point, ScenarioRunFieldIndex::SRFI_EXTEND_ROUTE_LAST_POINT);
   CRSCENRUN(ColregSimulation::simulation_settings, _run, scripted_scenario, ScenarioRunFieldIndex::SRFI_IS_SCRIPTED);

   for (size_t idx = SRFI_OWNSHIP_MODE; idx < SRFI_END; idx++)
      _scenario_run_folder->AddChild(_prop_run[idx].get());

   AddChild(_scenario_run_folder.get());
}

void ScenarioProperties::OnDescriptionChanged()
{
   //auto* sim = simulator::getSimulator();
   //if (sim)
   //{
   //   sim->SetDescription(_description.c_str());
   //}
}

void ScenarioProperties::OnSettingChanged()
{
   //simulator::getSimulator()->SetSettings(_settings);
   //simulator::getSimulator()->ReEstimate();
   //user_interface::InvalidateView();
}

void ScenarioProperties::OnEnvironmentChanged()
{
   //simulator::getSimulator()->SetEnvironment(_environment);
   //simulator::getSimulator()->ReEstimate();
   //user_interface::InvalidateView();
}

void ScenarioProperties::OnRunChanged()
{
   //simulator::getSimulator()->SetSimulationSettings(_run);
}



#undef CRSCENSETT
#undef CRSCENENV
#undef CRSCENRUN