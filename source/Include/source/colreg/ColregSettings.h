#pragma once
#include "CommonStructs.h"
#include "GoodMarinePracticeSettings.h"
#include "RouteModelSettings.h"
namespace colreg
{
   struct uninitialized_tag {};
   // Стратегии поиска безопасного маршрута расхождения
   enum class SEARCH_STRATEGY : char
   {
      SS_FULL_ITERATION = 0,  ///< Полный перебор всех возможных маршрутов, затем поиск оптимального
      SS_ORDER_ITERATION,     ///< Перебор маршрутов в определенном порядке. Анализируем группу маршрутов на оптимальность.
      SS_ORDER_FAST,          ///< Перебор маршрутов в определенном порядке. Первый найденный маршрутов считается оптимальным.   
      SS_SEARCH_TREE,
   };

#pragma pack(push, 1)

   /*
   Структура для задания настроек коэффициентов штрафов colreg.
   */
   struct penalty_ratio_settings
   {
      double turnPenaltyRatio;            //< коэффициент для настройки штрафа за маневр поворотом в нежелательную сторону 
      double tssPenaltyRatio;             //< коэффициент для настройки штрафа за маневр с пересечение TSS
      double speedIncreasePenaltyRatio;   //< коэффициент для настройки штрафа за маневр повышением скорости 
      double speedDecreasePenaltyRatio;   //< коэффициент для настройки штрафа за маневр понижением скорости
      double uturnPenaltyRatio;           //< коэффициент для настройки штрафа за маневр разворотом
      double interceptionPenaltyRatio;    //< коэффициент для настройки штрафа за маневр с пересечением целей по курсу
      double startTimePenaltyRatio;       //< коэффициент для настройки штрафа за время старта маневра
      double cirRadPenaltyRatio;          //< коэффициент для настройки штрафа за радиус циркуляции
      double leftTurnPenaltyRatio;        //< коэффициент для настройки штрафа за поворот налево
      double rightTurnPenaltyRatio;       //< коэффициент для настройки штрафа за поворот направо
      double passingDistRatio;            //< коэффициент для настройки штрафа за дистанцию расхождения с другими кораблями
      double lastMomenManeuverChangeSpeedManScale; //< коэффициент для настройки штрафа при маневре скоростью при маневре Последнего Момента
      
      double lastMomenManeuverStartTimeScale; //< коэффициент для настройки штрафа при маневре Последнего Момента для времени старта. 
                                          //< чем он больше, тем больше штраф болучит маневр при удалении (по времени) от МПМ

      size_t minPenaltyTurnAngle;         //< угол поворота, с которого начинаем штрафовать, в градусах
      size_t normPenaltyTurnAngle;        //< угол для нормирования штрафа за острый угол при повороте, в градусах (dK-minPenaltyTurnAngle)/normPenaltyTurnAngle)

      penalty_ratio_settings()
         : turnPenaltyRatio{ 3. }
         , tssPenaltyRatio{ 3. }
         , speedIncreasePenaltyRatio{ 25. }
         , speedDecreasePenaltyRatio{ 9. }
         , uturnPenaltyRatio{ 15. }
         , interceptionPenaltyRatio{ 6. }
         , startTimePenaltyRatio{ 15. }
         , cirRadPenaltyRatio{ 5. }
         , leftTurnPenaltyRatio{ 10. }
         , rightTurnPenaltyRatio{ 0. }
         , minPenaltyTurnAngle{ 30 }
         , normPenaltyTurnAngle{ 70 }
         , passingDistRatio{ 0. }
         , lastMomenManeuverChangeSpeedManScale( 10.0 )
         , lastMomenManeuverStartTimeScale( 25.0)
      {}

      penalty_ratio_settings(uninitialized_tag)
         : turnPenaltyRatio{ NO_VALUE }
         , tssPenaltyRatio{ NO_VALUE }
         , speedIncreasePenaltyRatio{ NO_VALUE }
         , speedDecreasePenaltyRatio{ NO_VALUE }
         , uturnPenaltyRatio{ NO_VALUE }
         , interceptionPenaltyRatio{ NO_VALUE }
         , startTimePenaltyRatio{ NO_VALUE }
         , cirRadPenaltyRatio{ NO_VALUE }
         , leftTurnPenaltyRatio{ NO_VALUE }
         , rightTurnPenaltyRatio{ NO_VALUE }
         , minPenaltyTurnAngle{ size_t(-1) }
         , normPenaltyTurnAngle{ size_t(-1) }
         , passingDistRatio{ NO_VALUE }
         , lastMomenManeuverChangeSpeedManScale{ NO_VALUE }
         , lastMomenManeuverStartTimeScale(NO_VALUE)
      {}
   };


   //! Структура для задания настроек алгоритма поиска вариантов расхождения.
   struct passing_algo_settings
   {
      size_t minStartTime;                ///< Минимальное время до начала маневра, в секундах
      size_t prolongationTime;            ///< На сколько продлеваем траекторию прогноза/маршрута при поиске маневра расхождения, в секундах. Продлеваем траекторию как движение по скорости/курсу в конце прогноза.
      double minTurnAngle;                ///< Минимальный угол поворота при маневре курсом, градусы
      double maxTurnAngle;                ///< Максимальный угол поворота при маневре курсом, градусы
      double minTurnAngleFog;             ///< Минимальный угол поворота при маневре курсом в тумане, градусы
      double angleSearchStep;             ///< Шаг перебора по углу поворота на новый курс, градусы
      size_t minRudderAngle;              ///< Минимальный угол перегладки руля при повороте на маневре
      size_t maxRudderAngle;              ///< Максимальный угол перегладки руля при повороте на маневре
      size_t rudderAngleStep;             ///< Шаг поиска угола перегладки руля при повороте на маневре
      double distSearchStep;              ///< Шаг перебора по дистанции для движения на новом курсе, мили
      bool useStartTimeReducing;          ///< Использовать послабление времени старта при поиске вариантов расхождения
      bool useDomainReducing;             ///< Использовать послабление сжатия домена при поиске вариантов расхождения
      double passingDomainScale;          ///< Коэффициент увеличения домена при поиске ваиантов расхождения 
      bool maneuversA1A2Enable;           ///< Использовать маневры 1A/2A (выход в следующую точку маршрута)
      bool optimizeVariantsSearch;        ///< Оптимизированный поиск вариантов расхождения (маршрут возможно не самый оптимальный но быстро работает)
      double draughtThreshold;            ///< Корабли с водоизмещением больше draughtThreshold получают повышенный штраф за маневр торможением, м3
      size_t brakingTimeThreshold;        ///< Корабли с водоизмещением больше draughtThreshold и временем торможения больше brakingTimeThreshold получают повышенный штраф за маневр торможением, секунды

      passing_algo_settings()
         : minStartTime{ 60 }
         , prolongationTime{300}
         , minTurnAngle{ 10.}
         , maxTurnAngle{ 130 }
         , minTurnAngleFog{ 30. }
         , angleSearchStep{ 10. }
         , distSearchStep{ 1. }
         , useStartTimeReducing{ false }
         , useDomainReducing{ false }
         , passingDomainScale{ 1.1 }
         , maneuversA1A2Enable{ true }
         , optimizeVariantsSearch{ true }
         , draughtThreshold{ 50000. }
         , brakingTimeThreshold{20 * 60}
         , minRudderAngle{ 10 }
         , maxRudderAngle{ 20 }
         , rudderAngleStep{ 10 }
      {}
   };

   //! Параметры для Stateful модели
   struct stateful_settings
   {
      unsigned int cleanUpInterval;       ///< интервал в секундах процедуры выполнения чистки устаревших данных
      unsigned int shipDataMaxCount;        ///< храним данные корабля последние shipDataMaxCount точек
      unsigned int maxOutOufRouteTime;    ///< если корабль сошел с маршрута, в течение maxOutOufRouteTime секунд модель еще идет по маршруту и нет варнинга
      unsigned int maxBadDirectionTime;   ///< время, когда у корабля на маршруте "плохой" маршрут (направленный на сход с маршрута) и не происходит 
                                          ///< схожденис с маршрута
      double trackAnalyzeInterval;        ///< анализ трека по скорости на интервале, секунд
      double alwaysReportDangerTime;      ///< коеффициент к minStartTime.  если время до опасности меньше этого значения *minStartTime + turnTime(90 Градусов), опасность репортится всегда
      double stableMovementInterval;      ///< секунды, интервал движения корабля с неизменными параметрами, при котором курс считается стабильным
      double stableSpeedEps;              ///< узлы, допустимая погрешность для скорости корабля, считающаяся стабильно
      double stableAngleEps;              ///< градусы, допустимая погрешность для курса корабля, чтобы считать его стабильным
      stateful_settings()
         : cleanUpInterval{ 60 }
         , shipDataMaxCount{ 5 }
         , maxOutOufRouteTime( 60 )
         , maxBadDirectionTime( 180 )
         , trackAnalyzeInterval( 60.0 )
         , alwaysReportDangerTime( 1.0 )
         , stableMovementInterval( 30.0 )
         , stableSpeedEps(0.1)
         , stableAngleEps(1.0)
      {
      }

   };


   /*
   Структура для задания настроек оценки опасного сближения кораблей
   */
   struct colision_estimation_settings
   {
      bool considerRelativeMovement;       //<при оценки опасного сближения кораблей учитывать их относительное движение
      double sameDirectionDomainScale;     //<коэффициент сжатия домена по носу для кораблей идущих в ~одном направлении (кильватерная  колонна)
      double forwardDomainLengthScale;     //<коэффициент длины корабля по носу
      double backwardDomainLengthScale;    //<коэффициент длины корабля по корме
      double leftDomainLengthScale;        //<коэффициент длины корабля по левому борту
      double rightDomainLengthScale;       //<коэффициент длины корабля по правому борту
      colision_estimation_settings()
         : considerRelativeMovement{ true }
         , forwardDomainLengthScale{ 4.}
         , backwardDomainLengthScale{ 1. }
         , leftDomainLengthScale{ 2. }
         , rightDomainLengthScale{ 3. }
         , sameDirectionDomainScale{ 0.5 }
      {}
   };

   struct danger_analyzer_settings
   {
      double stableDangerTimeSeconds = 90.0;  //<интервал в течение которого цель стабильно детектится как опасность
      double maxDangerTimeDeltaSeconds = 60.0; //<макс. разброс между предполагаемым временем опасности
      double stableDangerAreaDomainKoef = 0.5; //Коэффициент к радиусу домена безопасности для расчета "зоны стабильности опасности"
      double maxHistoryTime = 90 * 2; //stableDangerTimeSeconds * 2, интервал хранения оценок опасности
      size_t maxCountOfSyntheticDangers = 1; //макс. количество раз, когда опасность пропала, но на случай "дрожания" таргета или датчиков, репортим цель 
   };
   


   /*
   Структура для задания настроек colreg
   */
   struct settings
   {
      size_t timeStep;                                      //< шаг прогноза, в сек.
      size_t timePrediction;                                //< время прогноза, в сек.
      double alarmTimePercent;                              //< процент от времени прогноза, при котором срабатывает сигнализация опасного сближения (0 - 1)     
      SEARCH_STRATEGY searchStrategy;                       //< алгоритм поиска
      colreg::COLREG_MODE  colregMode;                      //< режима работы колрега. нужен только при создании
      int checkRouteMode;                                   //< содержит флаги при проверке CheckOnRoute. может содержать комбинацию из ROUTE_CHECK_FLAG
      double circulationRadiusFactor;                       //< коэффициент радиуса поворота в зависимости от длины корабля
      bool emergencyManeuverEnable;                         //< использовать экстренный маневр, при отсутствии решений   
      bool getOutOfDangerEnable;                            //< попытаться найти маршрут, который выведет из изначально опасного положения

      double avoidTurnPenalty;                              //< штраф за маневр поворотом в нежелательную сторону(0 - 1)
      double uturnPenalty;                                  //< штраф за маневр разворотом (0 - 1)
      double sharpAnglePenalty;                             //< штраф за резкий поворот (0 - 1)
      double speedIncreasePenalty;                          //< штраф за маневр увеличением скорости (0 - 1)
      double speedDecreasePenalty;                          //< штраф за маневр уменьшением скорости (0 - 1)
      double crossCoursePenalty;                            //< штраф за пересение курса цели (0 - 1)
      double cirRadPenalty;                                 //< штраф за маленький радиус циркуляции (0 - 1)
      double turnLeftPenalty;                               //< штраф за поворот нелево (0 - 1)
      double turnRightPenalty;                              //< штраф за поворот направо (0 - 1)
      double returnTimePenalty;                             //< штраф за время возврата на исходный маршрут(чем раньше вернемся на маршрут - тем лучше)
      DOMAIN_TYPE domainType;                               //< используемый домен безопасности
      bool   cooperativeMode;                               //< вкл/выкл кооперативный режим расхождения
      penalty_ratio_settings penalties;

      bool useLastMomentManeuver;                           //< использовать маневр последнего момента в случае вознокновения опасности с кораблем, не принимающим маршрут
      double defaultShipWidth;                              //< ширина корабля, если не задано, метры
      double defaultShipLength;                             //< длина корабля, если не задано, метры
      double defaultShipDraft;                              //< Осадка корабля, если не задано, метры
      double defaultLeftXTE;                                //< максимальное боковое смещение слева от маршрута, мили
      double defaultRightXTE;                               //< максимальное боковое смещение справа от маршрута, мили     
      double offsetLeftXTE;                                 //< максимальное боковое смещение слева при следовании без маршрута, мили
      double offsetRightXTE;                                //< максимальное боковое смещение справа при следовании без маршрута, мили     
      bool useLogging;                                      //< вкл/выкл логирование
      double zeroSpeedThreshold;                            //< корабли с скоростью ниже zeroSpeedThreshold считается неподвижными, узлы
      bool motionlessShipUseColreg;                         //< неподвижные корабли расходятся согласно МППСС, иначе остальные их объезжают
      bool useGoodMarinePractice;                           //< исполтьзовать хорошую морскую практику
      bool allowShipXTD;                                    //< возвращать маршрут расхождения сдвинутым от исходного взависимости от значения текушего отклонения 
      bool useRouteXTEInManeuver;                           //< На маневре расхождения используем XTE исходного маршрута

      good_marine_practice_settings gmp_settings;           //< настроеки для использования хорошей морской практики
      passing_algo_settings algo_settings;                  //< настройки алгоритма поиска вариантов расхождения
      colision_estimation_settings collision_settings;      //< настройки для оценки опасного сближения кораблей
      stateful_settings  stateful_mode_settings;            //< настройки для stateful режима

      double routeUpdateSpeedEps;                           //< Минимально расхождение между скоростью корабля и скоростью маршрута, при котором происходи update маршрутной скорости
      ROUTE_SPEED_UPDATE_STRATEGY   routeSpeedUpdateStrategy; //<стратегия корректировки скорости в маршруте

      double smallXTEdetectKoef;                            //< Коэффициент к Width/2  для определения узкого XTE и его динамического увеличения, для предотвращения схода за маршрута 
      double smallXTEincreaseKoef;                          //< Коэффициент к Width/2  при узком XTE для увеличения значения XTE при расчете схода с маршрута
      bool   ignoreCurrentChartDangers;                     //< true - Игнорить опасности-объекты карты, на которых уже находимся в данный момент. В противном случае давать NO SOLUTION
      double shipRelRedLevelPercentStartsFrom;              //< проценты вероятности возникновения опасной ситуации, в момент когда цель становится DL_RED
                                                            //< для DL_YELLOW вероятность от 0 до (shipRelRedLevelPercentStartsFrom), для DL_RED от (shipRelRedLevelPercentStartsFrom) до 100%
                                                            //< в зависимости от времени (чем ближе к опасности тем больше процент, до 100% в момент опасной ситцации)
      bool offsetTrajectoryByXTD;                           //< генерировать точки траектории со смещением, зависящим от текущего отклонения от маршрута
      danger_analyzer_settings   dngr_analyzer_cfg;
      size_t loggingTime;                                   //< время логирования в секундах
      route_model_config   route_config;                    //< Настройки RouteModel
      bool useWeather = false;                              //< Вкл/выкл погоду
      bool useWeatherDanger = false;                        //< Вкл/выкл опасности по погоде
      settings()
         : searchStrategy{ SEARCH_STRATEGY::SS_SEARCH_TREE }
         , circulationRadiusFactor{ 4. }
         , timeStep{ 60 }
         , timePrediction{ 3600 }
         , alarmTimePercent{ 0.5 }
         , avoidTurnPenalty{ 0.5 }
         , uturnPenalty{ 0.25 }
         , speedIncreasePenalty{ 1. }
         , speedDecreasePenalty{ 1. }
         , sharpAnglePenalty{ 0.8 }
         , crossCoursePenalty{ 1.0 }
         , cirRadPenalty{ 0.5 }
         , turnLeftPenalty{ 0.5 }
         , turnRightPenalty{ 0.1 }
         , returnTimePenalty{ 0.05 }
         , emergencyManeuverEnable{ true }         
         , getOutOfDangerEnable{ true }
         , domainType{ DOMAIN_TYPE::DOM_ELLIPTIC }
         , cooperativeMode{ true }         
         , useLastMomentManeuver{ false }
         , defaultShipWidth{ 25 }
         , defaultShipLength{ 200 }
         , defaultShipDraft{ 3 }
         , defaultLeftXTE{ 0.5 }
         , defaultRightXTE{ 0.5 }
         , offsetLeftXTE{ 1 * ( 1.0 / 1852.0 ) }
         , offsetRightXTE{ 1 * ( 1.0 / 1852.0 ) }
         , useLogging{ true }
         , loggingTime{ 24 * 60 * 60 } //сутки
         , zeroSpeedThreshold{ 3.}
         , motionlessShipUseColreg{ false }        
         , useGoodMarinePractice{ true }
         , allowShipXTD{ false }
         , useRouteXTEInManeuver{ false }
         , colregMode( colreg::COLREG_MODE::CM_STATE_LESS )
         , routeUpdateSpeedEps( 1 )
         , checkRouteMode( 1 )
         , routeSpeedUpdateStrategy( ROUTE_SPEED_UPDATE_STRATEGY::RSUS_PROGRESSIVE_GE )
         , smallXTEdetectKoef(1.5)
         , smallXTEincreaseKoef(2.0)
         , ignoreCurrentChartDangers( true )
         , shipRelRedLevelPercentStartsFrom(80.0)
         , offsetTrajectoryByXTD{ true }         
      {}
   };

   // Режим двигателя
   enum class ENGINE_MODE : int
   {
      EM_MARINE = 0,    //<морской
      EM_MANEUVERABLE   //<маневренный
   };

   struct chart_safety_settings
   {
      chart_safety_settings()
         : obstacleDistSafe{ .1 }
         , debugNarrow{ false }
         , debugNarrowWidth{ 3. }
         , analysisObjectsMask{ OT_SURFACE_OBJS | OT_UNDERWATER_OBJS | OT_TSS | OT_DYNAMIC_AREAS }
      {}

      double  obstacleDistSafe;    ///< Безопасное расстояние до препятствия, мили
      bool    debugNarrow;         ///< Плавания в узкостях (для проверки работы colreg в узкостях)
      double  debugNarrowWidth;    ///< Цирина узкости, мили (для проверки работы colreg в узкостях)
      mask_type analysisObjectsMask; ///< Маска объектов карты (colreg::OBJECT_TYPE), участвующих в анализе
   };

   //! Структура для задания окружающей среды
   struct environment
   {
      bool        fog;        ///< Туман
      double      maxSpeed;   ///< Максимальная скорость (ограничение в районе плавания), узлы
      double      distDanger; ///< Опасная дистанция между кораблями (общее для района плавания), мили
      double      distSafe;   ///< Безопасная дистанция между кораблями (общее для района плавания), мили
      ENGINE_MODE engineMode; ///< Режима двигателя в районе

      chart_safety_settings chart_settings;

      environment()
         : fog{ false }
         , maxSpeed{ 30. }
         , distDanger{ 1. }
         , distSafe{ 2. }
         , engineMode{ ENGINE_MODE::EM_MARINE }
      {}
   };

#pragma pack(pop)
};