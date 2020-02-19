#pragma once

#include "ColregInterfaces.h"

namespace colreg
{
#pragma pack (push, 1)

   namespace statefull
   {
      //! ��������� ��������� ������� � ����� ������ STATE-FULL ������ COLREG
      enum class STATE_FULL_STATE : char
      {
         SFS_GOING_MAIN_ROUTE = 0,                 //< ���� �� �������� ��������
         SFS_GOING_PREDICTION,                     //< ���� �� ������������
         SFS_GOING_COURSE_SPEED,                   //< ���� �� ������ ����� ��������
         SFS_SELECTING_MANEUVER,                   //< ��������� � ��������� ������ ������� ����������� � ����������
         SFS_WAITING_MANEUVER,                     //< ��������� � ��������� �������� ������ �� �������� ������ �����������
         SFS_ON_MANEUVER,                          //< ��������� � ��������� ���������� ������� �����������
         SFS_SIZE
      };

    
      //! ��������� ��������� �������
      enum class MANEUVER_STATE : char
      {
         MS_SAFE                 = _BIT_(0),       //< ������ ���������
         MS_DANGER               = _BIT_(1),       //< ������ ������
         MS_SELECTING            = _BIT_(2),       //< ������ � ��������� ������
         MS_ACCEPTED             = _BIT_(3),       //< ������ ������ �� ����������, �� �� �����
         MS_LOST                 = _BIT_(4),       //< ������ �������� 
         MS_ON_PROGRESS          = _BIT_(5),       //< ������ �����������
         MS_COMPLETED            = _BIT_(6),       //< ������ ��������
      };
  
      //! ���������� � ��������� ������� �����������
      struct statefull_maneuver_info
      {
         char state              = 0;              //< ��������� ������� (����������� ������ MANEUVER_STATE)
         solution_description sol_descr;           //< ���� � solution, ��� �������� ��� ������� ������
      
         double time_create      = INVALID_TIME;   //< ����� �������� �������, UTC        
         double time_from_start  = INVALID_TIME;   //< ����� �� ������ �������, ������� (��� ��������� SFS_ON_MANEUVER)

         union
         {
            double time_to_start = INVALID_TIME;   //< ����� �� ������ �������, ������� (��� ��������� SFS_SELECTING_MANEUVER � SFS_WAITING_MANEUVER)
            double time_to_finish;                 //< ����� �� ���������� �������, ������� (��� ��������� SFS_ON_MANEUVER)
         };
         union
         {
            double dist_to_start = 0.;             //< ��������� �� ������ �������, ���� (��� ��������� SFS_SELECTING_MANEUVER � SFS_WAITING_MANEUVER)
            double dist_to_finish;                 //< ��������� �� ���������� �������, ���� (��� ��������� SFS_ON_MANEUVER)
         };
         inline bool isStatePresent(MANEUVER_STATE checkState)const { return state & (char)checkState; }
      };
   
      //! ��������� ������ ����������� � ����������
      struct iStateFullManeuver : iSolutionManeuver
      {
         //! �������� ���������� �� ���������� ������� �����������
         virtual statefull_maneuver_info GetInfo() const = 0;
      };

      using statefull_maneuver_ref = base_ref<iStateFullManeuver*>;


      //! ��������� ��� ��������� ���������� � ���������� ������� STATE-FULL ������ COLREG
      struct iStateFullShip
      {
         //! �������� ��������� ������� � �������� ID
         virtual STATE_FULL_STATE GetState()const = 0;

         //! �������� ID �������
         virtual id_type GetID()const = 0;

         //! �������� ���������� ��������� �������� ����������� � ����������
         virtual const statefull_maneuver_ref* GetPosibleManeuvers()const = 0;
      };

      //! ��������� ������������
      enum class SUGGESTION_TYPE : char
      {
         ST_CANCEL_MANEUVER,                       //< ������������ �������� ��������, �� �� ������� ������ �����������
         ST_RETURN_TO_MAIN_ROUTE,                  //< ������������ �������� ��������� ������� ������ � ��������� �� ������� �������
         ST_ACCEPT_MANEUVER,                       //< ������������ ������� �� ���������� ���� �� ������������ ��������
      };

      //! ��������� �������
      enum class EVENT_TYPE
      {
         ET_SELECTING_CANCELED = 0,                //< ��������� ������ ������� ����������� ���������
         ET_MANEUVER_CANCELED,                     //< ���������, �� �� ������� ������ �������
         ET_MANEUVER_STARTED,                      //< ����� ������ �����������
         ET_MANEUVER_FINISHED,                     //< �������� ������ �����������
         ET_LOST_MANEUVER,                         //< ����� � �������������� �������
         ET_RETURN_TO_MANEUVER,                    //< ��������� �� ������������� ������
      };

      //! ������� �����������
      enum class NOTIFICATION_REASON
      {
         NR_NONE = 0,                              //< ��������� �������
         NR_DANGER_LOST,                           //< ��������� �������
         NR_DANGER_CHANGED,                        //< ��������� ����������
         NR_DANGER_ARISEN,                         //< ��������� ���������
         NR_PRIORITY_CHANGED,                      //< ��������� ��������� ����������� � ����������
         NR_MANEUVERS_MISSED,                      //< ��� ��������� ������� ����������� ���������
         NR_MANEUVERS_DANGERS,                     //< ��� ��������� ������� ����������� ������
      };

        //!  ���������� � ������������
      struct suggestion
      {
         id_type              ship_id;             //< ID �������
         SUGGESTION_TYPE      type;                //< ������������
         NOTIFICATION_REASON  reason;              //< ������� �����������
      };
      
      //!  ���������� � �������
      struct event
      {
         id_type              ship_id;             //< ID �������
         EVENT_TYPE           type;                //< �������
         NOTIFICATION_REASON  reason;              //< ������� �����������
      };

      //!  ���������� � �������� ��������, ����� ������ ������� CancelManeuver
      struct iActiveRoute : iReleasable
      {
         //! ��� ��������� ��������
         virtual ROUTE_TYPE GetType()const = 0;
         
         /*!
         �������� ������� �������� �������
         \return nullptr ���� �������� ���
         */
         virtual const route_ref* GetRoute() const = 0;
      };

      //! ���������� ���������� iColreg ��� �������������� ������������ � STATE-FULL �������
      struct iStateFullExtention 
      {
         /*! 
         �������� ������ ��� ������� � �������� ID
         \return iActiveRoute
         */
         virtual iActiveRoute* CancelManeuver(id_type id) = 0;

         /*!
         ��������� ������� � �������� ID � ��������� ������ ������� ����������� � ������� �������� 
         \param[in] shipId ������������� �������
         \param[in] type solution_description �� iCooperativeSolution)/iSingleSolution->GetDescription()
         \param[in] posible_maneuvers ������� ���������� �� iCooperativeSolution->GetManeuver()/iSingleSolution->GetVariants()
         */
         virtual bool StartSelecting(const solution_description& descr, const solution_maneuver_ref* posible_maneuvers) = 0;

         /*!
         ������� ��������� ������ �� ���������� ��� ������� � �������� ID
         \param[in] shipId ������������� �������
         \param[in] accepted_maneuver ���� �� �������� iStateFullShip->GetPosibleManeuvers(), ������������ � ��������� SFS_SELECTING_MANEUVER 
         */
         virtual bool AcceptManeuver(const solution_description& descr, const iSolutionManeuver* accepted_maneuver) = 0;
      };
   
      using statfull_ships_ref = base_ref<const iStateFullShip*>;
      using suggestions_ref = base_ref<suggestion>;
      using events_ref = base_ref<event>;

      //! ��������� ��� ��������� ���������� � ���������� ��������� ������������� �������� �� STATE-FULL ������ COLREG
      struct iStateFullNavEstimation
      {
         //! �������� StateFull �������, ����������� � ��������� state
         virtual const statfull_ships_ref* GetShipsByState(STATE_FULL_STATE state)const = 0;

         //! �������� ���������� ��� ������� STATE-FULL ������ Colreg �� ID
         virtual const iStateFullShip* GetShipById(id_type id)const = 0;

         //! �������� ������ ������������
         virtual const suggestions_ref* GetSuggestions() const = 0;

         //! �������� ������ �������
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