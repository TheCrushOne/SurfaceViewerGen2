#pragma once

/**
@file     ordermanager.h
@brief    Engine support draw order manager
@date     Created on 01/12/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class Engine;

// Специализированный класс, устанавливающий порядок отрисовки 3д объектов 
class OrderManager
{
    struct sb_object
    {
        void (*sbo_function)(void*);
        void *sbo_instance;
        D3DXVECTOR3 *sbo_coordinates;
        PRIORTYPE *sbo_priority;

        sb_object *sbo_next;
        
        sb_object(void(*frender)(void*), void *instance, D3DXVECTOR3 *coord, PRIORTYPE *priority) { sbo_function = frender; sbo_instance = instance;  sbo_coordinates = coord; sbo_next = nullptr; sbo_priority = priority; }
    };

public:
   OrderManager(std::shared_ptr<Engine> eng);
   ~OrderManager();

   void InvokeRegistration();
   void RegisterObject(void(*frender)(void*), void *instance, D3DXVECTOR3 *coord, PRIORTYPE *priority);
   void WithdrawRegistration();

   void Update();
   void Render() { m_currentRType == SBM_ORDERED ? orderedRender() : disorederedRender(); }

   void ToggleRenderType();
   inline RTYPE GetRenderType() { return m_currentRType; }
private:
   void orderedRender();
   void disorederedRender();

   void defaultSort();
   void recountDistance();
private:
   sb_object** m_orderedLine;
   sb_object** m_disorderedLine;
   sb_object** m_chaoticLine;        //проектируется

   float* m_dist;        //исп. только для orderedLine
   PRIORTYPE* m_prior;       //исп. только для orderedLine
   sb_object* m_first;
   sb_object* m_last;

   DWORD m_totalObjectCount;

   std::shared_ptr<Engine> m_engine;

   RTYPE m_currentRType;

   static const RTYPE DEFAULT_RENDER_TYPE;
};