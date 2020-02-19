#pragma once

#include "ColregInterfaces.h"

namespace colreg
{
#pragma pack (push, 1)

   namespace statefull
   {
      //! Возможные состояния корабля с точки зрения STATE-FULL модели COLREG
      enum class STATE_FULL_STATE : char
      {
         SFS_GOING_MAIN_ROUTE = 0,                 //< Идем по главному маршруту
         SFS_GOING_PREDICTION,                     //< Идем по предсказанию
         SFS_GOING_COURSE_SPEED,                   //< Идем по данным курса скорости
         SFS_SELECTING_MANEUVER,                   //< Находимся в состоянии выбора маневра расхождения с опасностью
         SFS_WAITING_MANEUVER,                     //< Находимся в состоянии ожидания захода на принятый маневр расхождения
         SFS_ON_MANEUVER,                          //< Находимся в состоянии выполнения маневра расхождения
         SFS_SIZE
      };

    
      //! Возможные состояния маневра
      enum class MANEUVER_STATE : char
      {
         MS_SAFE                 = _BIT_(0),       //< Маневр безопасен
         MS_DANGER               = _BIT_(1),       //< Маневр опасен
         MS_SELECTING            = _BIT_(2),       //< Маневр в состояние выбора
         MS_ACCEPTED             = _BIT_(3),       //< Маневр принят на выполнение, но не начат
         MS_LOST                 = _BIT_(4),       //< Маневр пропущен 
         MS_ON_PROGRESS          = _BIT_(5),       //< Маневр выполняется
         MS_COMPLETED            = _BIT_(6),       //< Маневр завершен
      };
  
      //! Информация о возможном маневре расхождения
      struct statefull_maneuver_info
      {
         char state              = 0;              //< Состояния маневра (объединение флагов MANEUVER_STATE)
         solution_description sol_descr;           //< Инфа о solution, для которого был получен маневр
      
         double time_create      = INVALID_TIME;   //< Время создания маневра, UTC        
         double time_from_start  = INVALID_TIME;   //< Время от старта маневра, секунды (для состояния SFS_ON_MANEUVER)

         union
         {
            double time_to_start = INVALID_TIME;   //< Время до старта маневра, секунды (для состояний SFS_SELECTING_MANEUVER и SFS_WAITING_MANEUVER)
            double time_to_finish;                 //< Время да завершения маневра, секунды (для состояния SFS_ON_MANEUVER)
         };
         union
         {
            double dist_to_start = 0.;             //< Дистанция до старта маневра, мили (для состояний SFS_SELECTING_MANEUVER и SFS_WAITING_MANEUVER)
            double dist_to_finish;                 //< Дистанция до завершения маневра, мили (для состояния SFS_ON_MANEUVER)
         };
         inline bool isStatePresent(MANEUVER_STATE checkState)const { return state & (char)checkState; }
      };
   
      //! Возможный маневр расхождения с опасностью
      struct iStateFullManeuver : iSolutionManeuver
      {
         //! Получить информацию по возможному маневру расхождения
         virtual statefull_maneuver_info GetInfo() const = 0;
      };

      using statefull_maneuver_ref = base_ref<iStateFullManeuver*>;


      //! Интерфейс для получения информации о состоянием корабля STATE-FULL модели COLREG
      struct iStateFullShip
      {
         //! Получить состояние корабля с заданным ID
         virtual STATE_FULL_STATE GetState()const = 0;

         //! Получить ID корябля
         virtual id_type GetID()const = 0;

         //! Получить актуальные возможные варианты расхождения с опасностью
         virtual const statefull_maneuver_ref* GetPosibleManeuvers()const = 0;
      };

      //! Возможные рекомендации
      enum class SUGGESTION_TYPE : char
      {
         ST_CANCEL_MANEUVER,                       //< Рекомендация отменить принятый, но не начатый маневр расхождения
         ST_RETURN_TO_MAIN_ROUTE,                  //< Рекомендация досрочно завершить начатый маневр и вернуться на главный маршрут
         ST_ACCEPT_MANEUVER,                       //< Рекомендация принять на выполнения один из предложенных маневров
      };

      //! Возможные события
      enum class EVENT_TYPE
      {
         ET_SELECTING_CANCELED = 0,                //< Состояние выбора маневра расхождения сборошено
         ET_MANEUVER_CANCELED,                     //< Выбранный, но не начатый маневр отменен
         ET_MANEUVER_STARTED,                      //< Начат маневр расхождения
         ET_MANEUVER_FINISHED,                     //< Закончен маневр расхождения
         ET_LOST_MANEUVER,                         //< Сошли с незавершенного маневра
         ET_RETURN_TO_MANEUVER,                    //< Вернулись на незавершенный маневр
      };

      //! Причина уведомления
      enum class NOTIFICATION_REASON
      {
         NR_NONE = 0,                              //< Опасность исчезла
         NR_DANGER_LOST,                           //< Опасность исчезла
         NR_DANGER_CHANGED,                        //< Опасность изменилась
         NR_DANGER_ARISEN,                         //< Опасность появилась
         NR_PRIORITY_CHANGED,                      //< Изменился приоритет расхождения с опасностью
         NR_MANEUVERS_MISSED,                      //< Все возможные маневра расхождения пропущены
         NR_MANEUVERS_DANGERS,                     //< Все возможные маневра расхождения опасны
      };

        //!  Информация о рекомендации
      struct suggestion
      {
         id_type              ship_id;             //< ID корабля
         SUGGESTION_TYPE      type;                //< Рекомендация
         NOTIFICATION_REASON  reason;              //< Причина нотификации
      };
      
      //!  Информация о событие
      struct event
      {
         id_type              ship_id;             //< ID корабля
         EVENT_TYPE           type;                //< Событие
         NOTIFICATION_REASON  reason;              //< Причина нотификации
      };

      //!  Информация о активном маршруте, после отмены вызовом CancelManeuver
      struct iActiveRoute : iReleasable
      {
         //! Тип активного маршрута
         virtual ROUTE_TYPE GetType()const = 0;
         
         /*!
         Получить текущий активный маршрут
         \return nullptr если маршрута нет
         */
         virtual const route_ref* GetRoute() const = 0;
      };

      //! Расширение интерфейса iColreg для взаимодействия пользователя с STATE-FULL моделью
      struct iStateFullExtention 
      {
         /*! 
         Отменить маневр для корабля с заданным ID
         \return iActiveRoute
         */
         virtual iActiveRoute* CancelManeuver(id_type id) = 0;

         /*!
         Перевести корабль с заданным ID в состояние выбора маневра расхождения с опасным объектом 
         \param[in] shipId Идентификатор корабля
         \param[in] type solution_description из iCooperativeSolution)/iSingleSolution->GetDescription()
         \param[in] posible_maneuvers Маневры полученные из iCooperativeSolution->GetManeuver()/iSingleSolution->GetVariants()
         */
         virtual bool StartSelecting(const solution_description& descr, const solution_maneuver_ref* posible_maneuvers) = 0;

         /*!
         Принять выбранный маневр на исполнение для корабля с заданным ID
         \param[in] shipId Идентификатор корабля
         \param[in] accepted_maneuver один из маневров iStateFullShip->GetPosibleManeuvers(), находящегося в состоянии SFS_SELECTING_MANEUVER 
         */
         virtual bool AcceptManeuver(const solution_description& descr, const iSolutionManeuver* accepted_maneuver) = 0;
      };
   
      using statfull_ships_ref = base_ref<const iStateFullShip*>;
      using suggestions_ref = base_ref<suggestion>;
      using events_ref = base_ref<event>;

      //! Интерфейс для получения информации о результате обработки навигационной ситуации от STATE-FULL модели COLREG
      struct iStateFullNavEstimation
      {
         //! Получить StateFull корабли, находящиеся в состоянии state
         virtual const statfull_ships_ref* GetShipsByState(STATE_FULL_STATE state)const = 0;

         //! Получить расширение для корабля STATE-FULL модели Colreg по ID
         virtual const iStateFullShip* GetShipById(id_type id)const = 0;

         //! Получить список рекомендаций
         virtual const suggestions_ref* GetSuggestions() const = 0;

         //! Получить список событий
         virtual const events_ref* GetEvents() const = 0;
      };

      inline const char* state_to_string(STATE_FULL_STATE state)
      {
         switch (state)
         {
         case colreg::statefull::STATE_FULL_STATE::SFS_GOING_MAIN_ROUTE:         return "GOING_MAIN_ROUTE";
         case colreg::statefull::STATE_FULL_STATE::SFS_GOING_PREDICTION:         return "GOING_PREDICTION";;
         case colreg::statefull::STATE_FULL_STATE::SFS_GOING_COURSE_SPEED:       return "GOING_COURSE_SPEED";
         case colreg::statefull::STATE_FULL_STATE::SFS_SELECTING_MANEUVER:       return "SELECTING_MANEUVER";
         case colreg::statefull::STATE_FULL_STATE::SFS_WAITING_MANEUVER:         return "WAITING_MANEUVER";
         case colreg::statefull::STATE_FULL_STATE::SFS_ON_MANEUVER:              return "ON_MANEUVER";
         }
         return "";
      }
    
      inline const char* suggestion_type_to_string(SUGGESTION_TYPE s)
      {
         switch (s)
         {
         case SUGGESTION_TYPE::ST_CANCEL_MANEUVER:          return "SHOULD CANCEL MANEUVER";
         case SUGGESTION_TYPE::ST_RETURN_TO_MAIN_ROUTE:     return "CAN RETURN TO MAIN ROUTE";
         case SUGGESTION_TYPE::ST_ACCEPT_MANEUVER:          return "SHOULD ACCEPT_MANEUVER";
         }
         ATLASSERT(false);
         return "unknown suggestion type";
      }

      inline const char* event_type_to_string(EVENT_TYPE e)
      {
         switch (e)
         {
         case EVENT_TYPE::ET_SELECTING_CANCELED:            return "SELECTING WAS CANLELED";
         case EVENT_TYPE::ET_MANEUVER_CANCELED:             return "MANEUVER_CANLELED";   
         case EVENT_TYPE::ET_MANEUVER_STARTED:              return "MANEUVER_STARTED";    
         case EVENT_TYPE::ET_MANEUVER_FINISHED:             return "MANEUVER_FINISHED";   
         case EVENT_TYPE::ET_LOST_MANEUVER:                 return "LOST_MANEUVER";
         case EVENT_TYPE::ET_RETURN_TO_MANEUVER:            return "RETURN_TO_MANEUVER";

         }
         ATLASSERT(false);
         return "unknown event type";
      }

      inline const char* reason_to_string(NOTIFICATION_REASON reason)
      {
         switch (reason)
         {
         case NOTIFICATION_REASON::NR_DANGER_LOST:          return "DANGER WAS LOST";
         case NOTIFICATION_REASON::NR_DANGER_CHANGED:       return "DANGER WAS CHANGED";
         case NOTIFICATION_REASON::NR_DANGER_ARISEN:        return "DANGER WAS ARISEN";
         case NOTIFICATION_REASON::NR_PRIORITY_CHANGED:     return "PRIORITY WAS CHANGED";
         case NOTIFICATION_REASON::NR_MANEUVERS_MISSED:     return "ALL MANEUVERS ARE MISSED";
         case NOTIFICATION_REASON::NR_MANEUVERS_DANGERS:    return "ALL MANEUVERS ARE DANGER";
         }
         return  "";
      }

   }

#pragma pack (pop)
}