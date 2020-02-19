#pragma once
#include "SelectedObjectManager.h"
#include "property_helper.h"


struct debug_filter_info
{
   std::string name;
   bool show = false;
};

class ScenarioProperties : public iSelected
{
   enum CustomSettingsFieldIndex
   {
      CSFI_DESCRIPTION = 0,
      CSFI_AISLOGPATH,
      CSFI_MAPFOLDERPATH,
   };

   enum ScenarioSettingsFieldIndex
   {
      SSFI_TIMESTEP = 0,
      SSFI_TIMEPREDICTION,
      SSFI_ALARMTIMEPRCNT,
      SSFI_SEARCHSTRAT,
      SSFI_STATEFULMODE,
      SSFI_CHECKROUTEMODE,
      SSFI_CIRRADFACTOR,
      SSFI_EMERGMANEUVER,
      SSFI_GETOUTOFDNGR,

      SSFI_AVOIDTURNPEN,
      SSFI_UTURNPEN,
      SSFI_SHARPANGLEPEN,
      SSFI_SPEEDINCPEN,
      SSFI_SPEEDDECPEN,
      SSFI_CROSSCRSPEN,
      SSFI_CIRRADPEN,
      SSFI_TURNLEFTPEN,
      SSFI_TURNRIGHTPEN,
      SSFI_RETTIMEPEN,
      SSFI_DOMAINTYPE,
      SSFI_COOPMODE,

      SSFI_USELASTMOMENT,
      SSFI_DEFSHIPWDTH,
      SSFI_DEFSHIPLENGTH,
      SSFI_DEFSHIPDRAFT,
      SSFI_DEFLEFTXTE,
      SSFI_DEFRIGHTXTE,
      SSFI_OFFSETLEFTXTE,
      SSFI_OFFSETRIGHTXTE,
      SSFI_USELOGGING,
      SSFI_ZEROSPDTHRESHOLD,
      SSFI_MLESSSHIPUSECOLREG,
      SSFI_USEGMP,
      SSFI_ALLOWSHIPXTD,
      SSFI_USEROUTEXTEINMAN,

      SSFI_ROUTEUPDATESPEEDEPS,
      SSFI_ROUTEUPDATESPEEDSTRATEGY,

      // penalties
      SSFI_TURNPENRATIO,
      SSFI_TSSPENRATIO,
      SSFI_SPDINCPENRATIO,
      SSFI_SPDDECPENRATIO,
      SSFI_UTURNPENRATIO,
      SSFI_INTPENRATIO,
      SSFI_STTIMEPENRATIO,
      SSFI_CIRRADPENRATIO,
      SSFI_LEFTTURNPENRATIO,
      SSFI_RIGHTTURNPENRATIO,
      SSFI_PASSINGDISTRATIO,
      SSFI_LMMCHANGESPDMANSCALE,
      SSFI_LMMSTARTTIMESCALE,
      SSFI_MINPENTURNANGLE,
      SSFI_NORMPENTURNANGLE,

      // gmp
      SSFI_CHECKTRGALARM,
      SSFI_CHECKCRSINT,
      SSFI_SFCRSINT,
      SSFI_MAXPENALTY,
      // gmp hcm
      SSFI_BADHEADINGMAN,
      SSFI_SECONDTURNPEN,
      SSFI_THIRDTURNPEN,
      SSFI_MAXPARANGLEDELTA,
      SSFI_OFFSETPENALTY,
      // gmp cr
      SSFI_MINPENALTY,
      SSFI_TURNTOSTERNPEN,
      SSFI_LEAVEBEHINDPEN,

      // algo
      SSFI_MINSTARTTIME,
      SSFI_PROLONGTIME,
      SSFI_MINTURNANGLE,
      SSFI_MAXTURNANGLE,
      SSFI_MINTURNANGLEFOG,
      SSFI_ANGLESRCHSTEP,
      SSFI_MINRUDDERANGLE,
      SSFI_MAXRUDDERANGLE,
      SSFI_RUDDERANGLESTEP,
      SSFI_DSTSRCHSTEP,
      SSFI_USESTTMREDUCE,
      SSFI_USEDOMREDUCE,
      SSFI_PASSDOMSCALE,

      SSFI_MANA1A2ENABLE,
      SSFI_OPTVARSRCH,

      SSFI_DRAUGHTTHRESHOLD,
      SSFI_BRKTIMETHRESHOLD,

      // collision
      SSFI_CONSIDERRELMOV,
      SSFI_SMDIRDOMSCALE,
      SSFI_FWDDOMLENGTHSCALE,
      SSFI_BACKDOMLENGTHSCALE,
      SSFI_LEFTDOMLENGTHSCALE,
      SSFI_RIGHTDOMLENGTHSCALE,

      // stateful mode
      SSFI_SMODE_CLEANUPINTERVAL,
      SSFI_SMODE_SHIPDATAMAXCOUNT,
      SSFI_SMODE_MAXOUTOFROUTEINTERVAL,
      SSFI_SMODE_MAXBADDIRECTION,
      SSFI_SMODE_TRACKANALYZEINTERVAL,
      SSFI_SMODE_ALWAYSREPORTDANGERTIME,
      SSFI_SMODE_STABLEMOVEMENTINTERVAL,
      SSFI_SMODE_STABLESPEEDEPS,
      SSFI_SMODE_STABLEANGLEEPS,
      SSFI_SMODE_SMALLXTEDETECTKOEF,
      SSFI_SMODE_SMALLXTEINCREASEXTE,
      SSFI_SMODE_IGNORECHARTDANGERSALREADYON,
      SSFI_SMODE_SHIPRELREDLEVELSTARTS,
      SSFI_SMODE_OFFSET_TRAJECTORY,

      // danger analyzer settings
      SSFI_DAS_STABLEDANGERSECONDS,
      SSFI_DAS_DANGERTIMEDELTA,
      SSFI_DAS_STABLEDANGERAREADOMAINKOEF,
      SSFI_DAS_MAXHISTORYTIME,
      SSFI_DAS_SYNTHCOUNT,

      SRFI_LOGGING_TIME,


      // ROUTE SETTINGS
      SSFI_IN_ROUTEMAXCOURSEDIFF,
      SSFI_CLOSETOROUTEZONEKOEF,
      SSFI_CLOSETOROUTEMAXCOURSEDIFF,
      SSFI_NEARTOROUTEZONEKOEF,
      SSFI_MINZONEWIDTHKOEF,
      SSFI_USE_EXT_ZONES,

      SRFI_USE_WEATHER,
      SRFI_USE_WEATHER_DANGERS,
      SSFI_END,
   };

   enum ScenarioEnvironmentFieldIndex
   {
      SEFI_FOG = 0,
      SEFI_MAXSPEED,
      SEFI_DISTDANGER,
      SEFI_DISTSAFE,
      SEFI_ENGINEMODE,

      // chart safety settings
      SEFI_OBSTDISTSAFE,
      SEFI_DEBUGNARROW,
      SEFI_DEBUGNARROWWIDTH,
      SEFI_ANALYSISOBJMASK,

      SEFI_END,
   };

   enum ScenarioRunFieldIndex
   {
      SRFI_OWNSHIP_MODE = 0,
      SRFI_TIME_STEP,
      SRFI_ESTIMATION_INTERVAL,
      SRFI_AUTO_APLY,
      SRFI_EXTEND_ROUTE_LAST_POINT,
      SRFI_IS_SCRIPTED,
      SRFI_END,
   };

   const std::unordered_map<CustomSettingsFieldIndex, FieldMeta> customSettingsMeta = {
      { CSFI_DESCRIPTION, { "description", "Scenario description", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { CSFI_AISLOGPATH, { "AIS Log Path", "AIS File Log Full Path", VALUE_FORMAT_TYPE::VFT_LOGFILE, false } },
      { CSFI_MAPFOLDERPATH, { "MAP Folder Path", "MAP folder path (containing files quad_tree_descr.bin and 0.bin)", VALUE_FORMAT_TYPE::VFT_FOLDERPATH, false } },
   };

   const std::unordered_map<ScenarioSettingsFieldIndex, FieldMeta> scenarioSettingsMeta = {
      { SSFI_TIMESTEP, { "Time Step", "prediction step, in seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_TIMEPREDICTION, { "Time Prediction", "prediction time, in seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_ALARMTIMEPRCNT, { "Alarm Time Percent", "prediction time percent, at which dangerous proximity alarm triggers (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SEARCHSTRAT, { "Search Strategy", "search strategy(algorithm)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_STATEFULMODE, { "Colreg Mode", "colreg mode (FullState or StateLess)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CHECKROUTEMODE, { "Check Route Mode", "check Route Mode, bitmask ( 0 -default, 1 - checkDirection, 2 - skip check xte )", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CIRRADFACTOR, { "Circulation Radius Factor", "turn radius factor depending on vessel length", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_EMERGMANEUVER, { "Emergency Maneuver Enabled", "use emergency maneuver when solution was not found", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_GETOUTOFDNGR, { "Get Out Of Danger Enable", "try to find route to got out of danger", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SSFI_USELASTMOMENT, { "Use Last Moment", "use last moment maneuver in case of emerging danger with not accepting route vessel", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DEFSHIPWDTH, { "Default Ship Width", "ship width if not assigned", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DEFSHIPLENGTH, { "Default Ship Length", "ship length if not assigned", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DEFSHIPDRAFT, { "Default Ship Draft", "ship draft if not assigned", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DEFLEFTXTE, { "Default Left XTE", "default left XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DEFRIGHTXTE, { "Default Right XTE", "default right XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_OFFSETLEFTXTE, { "Offset Left XTE", "offset left XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_OFFSETRIGHTXTE, { "Offset Right XTE", "offset right XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_USELOGGING, { "Use Logging", "log of colreg`s calls", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_ZEROSPDTHRESHOLD, { "Zero Speed Threshold", "speed threshold for a stationary vessel, in knoots", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MLESSSHIPUSECOLREG, { "Motionless Ship Use Colreg", "stationary vessels passing according to the COLREG", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_USEGMP, { "Use Good Marine Practice", "use good marine practice", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_ALLOWSHIPXTD, { "Allow Ship XTD", "return the passing route shifted from the original depending on the value of the current XTD", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_USEROUTEXTEINMAN, { "Use Route XTE In Maneuver", "use route XTE whitin maneuver", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SSFI_AVOIDTURNPEN, { "Avoid Turn Penalty", "turn to undesirable side penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_UTURNPEN, { "U-turn Penalty", "u-turn penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SHARPANGLEPEN, { "Sharp Angle Penalty", "sharp turn penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SPEEDINCPEN, { "Speed Increase Penalty", "speed increase penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SPEEDDECPEN, { "Speed Decrease Penalty", "speed decrease penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CROSSCRSPEN, { "Cross Course Penalty", "crossing targets course penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CIRRADPEN, { "Cir Radius Penalty", "small turn radius penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_TURNLEFTPEN, { "Turn Left Penalty", "left turn penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_TURNRIGHTPEN, { "Turn Right Penalty", "right turn penalty (0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_RETTIMEPEN, { "Return Time Penalty", "return to route time penalty (the sooner the better)(0 - 100)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DOMAINTYPE, { "Domain Type", "used safety domain (0-radial, 1-elliptic)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_COOPMODE, { "Cooperative Mode", "on/off cooperative passing mode", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_IGNORECHARTDANGERSALREADYON, { "Ignore Chart Dangers AlreadyOn", "Ignore Chart Dangers ship has already located on", VALUE_FORMAT_TYPE::VFT_NONE, false } } ,
      { SSFI_SMODE_SHIPRELREDLEVELSTARTS, { "Ship Relation RedLevel Danger Percent Starts From", "Danger situation probability starts from in Red Level", VALUE_FORMAT_TYPE::VFT_NONE, false } } ,
      { SSFI_SMODE_OFFSET_TRAJECTORY, { "Offset trajectory by vessel XTD", "Offset trajectory by vessel XTD", VALUE_FORMAT_TYPE::VFT_NONE, false } } ,
      // pentalties

      { SSFI_TURNPENRATIO, { "turnPenaltyRatio", "turn to undesirable side penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_TSSPENRATIO, { "tssPenaltyRatio", "TSS crossing penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SPDINCPENRATIO, { "speedIncreasePenaltyRatio", "speed increase penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SPDDECPENRATIO, { "speedDecreasePenaltyRatio", "speed decrease penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_UTURNPENRATIO, { "uturnPenaltyRatio", "u-turn penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_INTPENRATIO, { "interceptionPenaltyRatio", "crossing targets course penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_STTIMEPENRATIO, { "startTimePenaltyRatio", "maneuver start time penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CIRRADPENRATIO, { "cirRadPenaltyRatio", "small turn radius penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_LEFTTURNPENRATIO, { "leftTurnPenaltyRatio", "left turn penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_RIGHTTURNPENRATIO, { "rightTurnPenaltyRatio", "right turn penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_PASSINGDISTRATIO, { "passingDistRatio", "passing distance ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_LMMCHANGESPDMANSCALE, { "lastMomenManeuverChangeSpeedManScale", "Last Moment Maneuver speed change maneuver ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_LMMSTARTTIMESCALE, { "lastMomenManeuverStartTimeScale", "Last Moment Maneuver Start Time penalty ratio", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SSFI_MINPENTURNANGLE, { "minPenaltyTurnAngle", "turn angle, from which penalties starts", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_NORMPENTURNANGLE, { "normPenaltyTurnAngle", "turn angle for rationing sharp turn penalty, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SSFI_ROUTEUPDATESPEEDEPS, { "routeUpdateSpeedEps", "min acceptable speed delta between track point and route point , until route speed updated", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_ROUTEUPDATESPEEDSTRATEGY, { "routeSpeedUpdateStrategy", "update route speed strategy", VALUE_FORMAT_TYPE::VFT_NONE, false } },


      // gmp
      { SSFI_CHECKTRGALARM, { "Check Target Alarm", "when searching for maneuver, check the alarm of targets", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CHECKCRSINT, { "Check Course Intersection", "when searching for maneuver, check the course intersection with targets", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SFCRSINT, { "Safe Course Intersection", "safe distance the course intersection with target", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MAXPENALTY, { "Max Penalty", "max penalty value for Good Marine Practice", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      // gmp hcm
      { SSFI_BADHEADINGMAN, { "Bad Heading Meneuver", "bad maneuver penalty value", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MAXPARANGLEDELTA, { "Max Parallel Angle Delta", "max acceptable angle delta on 1-st and 3-d maneuver segments", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SECONDTURNPEN, { "Second Turn Penalty", "bad turn in P2 point penalty weight", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_THIRDTURNPEN, { "Third Turn Penalty", "bad turn in P3 point penalty weight", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_OFFSETPENALTY, { "Offset Penatly", "penalty weight in case of great distance from current movement line", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      // gmp cr
      { SSFI_MINPENALTY, { "Min Penalty", "min penalty", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_TURNTOSTERNPEN, { "Turn To Stern Penalty", "turn to stern penalty weight", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_LEAVEBEHINDPEN, { "Leave Behind Penalty", "leave behind penalty weight", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      // algo
      { SSFI_MINSTARTTIME, { "Min Start Time", "minimal time before start maneuvering, in seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_PROLONGTIME, { "Prolongation Time", "predictions's extend time, in seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MINTURNANGLE, { "Min Turn Angle", "minimal turn angle for course maneuver, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MAXTURNANGLE, { "Max Turn Angle", "maximal turn angle for course maneuver, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MINTURNANGLEFOG, { "Min Turn Angle Fog", "minimal turn angle for course maneuver in the conditions of fog, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_ANGLESRCHSTEP, { "Angle Search Step", "search step by turn angle, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MINRUDDERANGLE, { "Min Rudder Angle", "minimal rudder angle for course maneuver, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MAXRUDDERANGLE, { "Max Rudder Angle", "maximal rudder angle for course maneuver, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_RUDDERANGLESTEP, { "Rudder Angle Step", "search step by rudder angle, in degrees", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DSTSRCHSTEP, { "Dist Search Step", "search step by distance, in miles", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_USESTTMREDUCE, { "Use Start Time Reducing", "start time easing when searching variants", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_USEDOMREDUCE, { "Use Domain Reducing", "domain compression easing when searching variants", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_PASSDOMSCALE, { "Passing Domain Scale", "passing domain scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MANA1A2ENABLE, { "Maneuvers A1A2 Enable", "use maneuvers 1A/2A (proceed to the next route point)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_OPTVARSRCH, { "Optimize Variants Search", "optimal passing variant search (route might be not optimal, but works fast)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DRAUGHTTHRESHOLD, { "Draught Threshold", "draught(threshold for increase braking  penalty)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_BRKTIMETHRESHOLD, { "Braking Time Threshold", "braking time threshold, in seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      // collision
      { SSFI_CONSIDERRELMOV, { "Ñonsider Relative Movement", "when estimate collisions considered relative movement", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMDIRDOMSCALE, { "Same Direction Domain Scale", "when go same direction reduce domain scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_FWDDOMLENGTHSCALE, { "Forward Domain Length Scale", "Forward domain's length scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_BACKDOMLENGTHSCALE, { "Backward Domain Length Scale", "Backward domain's length scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_LEFTDOMLENGTHSCALE, { "Left Domain Length Scale", "Left domain's length scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_RIGHTDOMLENGTHSCALE, { "Right Domain Length Scale", "Right domain's length scale", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      // stateful
      { SSFI_SMODE_CLEANUPINTERVAL, { "Clean Up Interval", "cleanup stateful info interval, seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_SHIPDATAMAXCOUNT, { "Ship Data Max Count", "ship data live frame, points count", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_MAXOUTOFROUTEINTERVAL, { "Max Out Of Route Time", "interval, seconds. if ship is out of route in this interval, no warning appears and model follow route", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_MAXBADDIRECTION, { "Max Bad Direction Time", "interval, seconds. if ship has bad direction in route, no warning appears and model follow route", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_TRACKANALYZEINTERVAL, { "Track Analyze Interval", "interval, seconds. time interval of track points to analyze", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_ALWAYSREPORTDANGERTIME, { "Always Report Danger Time", "interval, seconds. danger with time less this value always reports", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_STABLEMOVEMENTINTERVAL, { "Stable Movement Interval", "interval, seconds. stable ships movement in this interval became new cours", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_STABLESPEEDEPS, { "Stable Speed Eps", "knots, valid speed eps for ship with constant speed", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_STABLEANGLEEPS, { "stableAngleEps", "degrees, valid course eps for ship with constant course", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SSFI_SMODE_SMALLXTEDETECTKOEF, { "smallXTEdetectKoef", "Koef to Width/2 for detect extra narrow XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_SMODE_SMALLXTEINCREASEXTE, { "smallXTEincreaseKoef", "Koef to Width/2 to increase extra narrow XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      //damnger analyzer settings
      { SSFI_DAS_STABLEDANGERSECONDS, { "stableDangerTimeSeconds", "Stable danger interval to alarm", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DAS_DANGERTIMEDELTA, { "maxDangerTimeDeltaSeconds", "Danger time max delta interval", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DAS_STABLEDANGERAREADOMAINKOEF, { "stableDangerAreaDomainKoef", "Danger area Stability size as koef to ship length ", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DAS_MAXHISTORYTIME, { "maxHistoryTime", "analyzer history len in seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_DAS_SYNTHCOUNT, { "maxCountOfSyntheticDangers", "max synthetic dangers", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SRFI_LOGGING_TIME, { "Logging Time", "Logging time, in seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SSFI_IN_ROUTEMAXCOURSEDIFF, { "in_route_max_course_diff", "In Route Zone Max Course Diff, grad", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CLOSETOROUTEZONEKOEF, { "close_to_route_zone_koef", "Close To Route Zone Size, koef to ROUTE XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_CLOSETOROUTEMAXCOURSEDIFF, { "close_to_route_max_course_diff", "Close To Route Zone Max Course Diff, grad", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_NEARTOROUTEZONEKOEF, { "near_to_route_zone_koef", "Near To Route Zone Size, koef to ROUTE XTE", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_MINZONEWIDTHKOEF, { "min_zone_width_koef", "Min Zone Width, koef to ship width", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SSFI_USE_EXT_ZONES, { "use_extended_route_zones", "use extended routes zones to analyze location within route ", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      { SRFI_USE_WEATHER, { "Use weather", "Take into account of weather influence", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SRFI_USE_WEATHER_DANGERS, { "Use weather dangers", "Generate dangers by weather conditions to avoid dangerous maneuvers.", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      //{ SSFI_, { "", "", VALUE_FORMAT_TYPE::VFT_NONE, false } },
   };

   const std::vector<std::string> searchStats = {
      { "Full Iteration" },
      { "Order Iteration" },
      { "Order Fast" },
      { "Seach Tree" },
   };

   const std::vector<std::string> colregModes = {
      { "Stateless" },
      { "Statefull" },
   };

   const std::vector<std::string> domainTypes = {
      { "Radius" },
      { "Elliptic" },
   };

   const std::vector<std::string> routeSpeedUpdateStrategies = {
      { "Simple" },
      { "Progressive" },
      { "Progressive Incremental" },
      { "Force Equal" },
   };

   const std::vector<std::string> engineModes = {
      { "Marine" },
      { "Maneuverable" },
   };

   const std::unordered_map<ScenarioEnvironmentFieldIndex, FieldMeta> scenarioEnvironmentMeta = {
      { SEFI_FOG, { "fog", "fog", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SEFI_MAXSPEED, { "maxSpeed", "maximum speed limit, knots", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SEFI_DISTDANGER, { "distDanger", "danger distance between ships, miles", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SEFI_DISTSAFE, { "distSafe", "safe distance between ships, miles", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SEFI_ENGINEMODE, { "engineMode", "engine operating mode", VALUE_FORMAT_TYPE::VFT_NONE, false } },

      // chart safety settings
      { SEFI_OBSTDISTSAFE, { "obstacleDistSafe", "safe distance to obstacle", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SEFI_DEBUGNARROW, { "debugNarrow", "for swimming in narrows(colreg check)", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SEFI_DEBUGNARROWWIDTH, { "debugNarrowWidth", "narrow width, miles", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SEFI_ANALYSISOBJMASK, { "analysisObjectsMask", "defines which objects will be used while analysing", VALUE_FORMAT_TYPE::VFT_NONE, false } },
   };

   const std::unordered_map<ScenarioRunFieldIndex, FieldMeta> scenarioRunMeta = {
      { SRFI_OWNSHIP_MODE, { "ownship", "ownShip id", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SRFI_TIME_STEP, { "timeStep", "simulation time step", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SRFI_ESTIMATION_INTERVAL, { "estimateInterval", "estimation time interval, seconds", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SRFI_AUTO_APLY, { "autoAply", "auto aply COLREG solutions", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SRFI_EXTEND_ROUTE_LAST_POINT, { "autoExtendRoute", "Automatically extend each route in scenario with one point ", VALUE_FORMAT_TYPE::VFT_NONE, false } },
      { SRFI_IS_SCRIPTED, { "isScripted", "Colreg scenario is scripted", VALUE_FORMAT_TYPE::VFT_NONE, true } },
   };
public:
   ScenarioProperties();
   void Render(render::iRender* renderer) override {}
private:
   void OnSettingChanged();
   void OnEnvironmentChanged();
   void OnRunChanged();
   void OnDebugFilterChanged();
   void OnDescriptionChanged();
   void OnAisLogPathChanged();
   void OnMapFolderPathChanged();
   void prepareScenarioSettingsFolder();
   void prepareScenarioEnvironmentFolder();
   void prepareScenarioRunFolder();

   //void addDebugFilter(const dbg::debug_info_node& node, FolderProperty* folder);
private:
   std::unique_ptr<FolderProperty> _scenario_settings_folder;
   std::unique_ptr<FolderProperty> _scenario_settings_penalties_folder;
   std::unique_ptr<FolderProperty> _scenario_settings_gmp_folder;
   std::unique_ptr<FolderProperty> _scenario_settings_gmp_hcm_folder;
   std::unique_ptr<FolderProperty> _scenario_settings_gmp_cr_folder;
   std::unique_ptr<FolderProperty> _scenario_settings_algo_folder;
   std::unique_ptr<FolderProperty> _scenario_settings_collision_folder;
   std::unique_ptr<FolderProperty> _scenario_settings_stateful_folder;
   std::unique_ptr<FolderProperty> _scenario_danger_analyzer_folder;
   std::unique_ptr<FolderProperty> _route_model_settings_folder;

   std::unique_ptr<FolderProperty> _scenario_environment_folder;
   std::unique_ptr<FolderProperty> _scenario_environment_cs_folder;

   std::unique_ptr<FolderProperty> _scenario_run_folder;

   //settings properties
   std::vector<iPropertyPtr> _prop_settings;
   std::vector<iPropertyPtr> _prop_environment;
   std::vector<iPropertyPtr> _prop_run;
   iPropertyPtr _prop_description;
   iPropertyPtr _prop_aisLogFilePath;
   iPropertyPtr  _prop_mapFolderPath;

   //colreg::settings _settings;
   //colreg::environment _environment;
   //ColregSimulation::simulation_settings _run;

   std::string _description;
   std::string _aisLogFilePath;
   std::string _mapFolderPathPrev;
   std::string _mapFolderPath;
};