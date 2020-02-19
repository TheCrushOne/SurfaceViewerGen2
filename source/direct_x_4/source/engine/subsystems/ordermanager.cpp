/**
@file     ordermanager.cpp
@brief    Engine support draw order manager
@date     Created on 01/12/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "ordermanager.h"
#include "engine/engine.h"
#include "utils\camera.h"

const RTYPE OrderManager::DEFAULT_RENDER_TYPE = SBM_DISORDERED;

OrderManager::OrderManager(std::shared_ptr<Engine> eng)
   : m_engine(eng)
   , m_totalObjectCount(0)
   , m_first(nullptr)
   , m_last(nullptr)
   , m_currentRType(DEFAULT_RENDER_TYPE)
{}

OrderManager::~OrderManager()
{
   sb_object* tmp;
   while (m_first != m_last)
   {
      tmp = m_first;
      m_first = m_first->sbo_next;
      delete tmp;
   }
   delete m_last;
   m_first = m_last = nullptr;

   delete m_prior;
   delete m_dist;
   delete m_orderedLine;
   delete m_disorderedLine;
   delete m_chaoticLine;
}

// ������������� ���������� ����� �������� ����������� 3� ��������
void OrderManager::InvokeRegistration()
{
   //���� ��� �� ������������ �.�. ������ ���� � ����������� ������, � ������ ����� ���������� ����������� ���������� ������� ����������
}

// ����������� ������������ ������� � ������ ��������, ���������� ����������
void OrderManager::RegisterObject(void(*frender)(void*), void *instance, D3DXVECTOR3 *coord, PRIORTYPE *priority)
{
   m_last = (m_last ? m_last->sbo_next : m_first) = new sb_object(frender, instance, coord, priority);
   m_totalObjectCount++;
}

// ������������� ���������� ����� ���������� ����������� 3� ��������
void OrderManager::WithdrawRegistration()
{
   m_orderedLine = new sb_object*[m_totalObjectCount];
   m_disorderedLine = new sb_object*[m_totalObjectCount];
   m_chaoticLine = new sb_object*[m_totalObjectCount];
   sb_object *tmp = m_first;
   DWORD i = 0;

   while (1)       //������� ����...
   {
      m_orderedLine[i] = m_disorderedLine[i] = m_chaoticLine[i] = tmp;
      i++;
      if (tmp == m_last)
         break;
      else
         tmp = tmp->sbo_next;
   }

   m_dist = new float[m_totalObjectCount];
   m_prior = new PRIORTYPE[m_totalObjectCount];
   for (i = 0; i < m_totalObjectCount; i++)
      m_dist[i] = 0.0f;
}

// �������������� ��������(����� ��������(�� �������������))
void OrderManager::Update()
{
   defaultSort();      //���� ���(��� ����� � ���������� ����������)
}

// ����� ������ ������� ������������������ �������� � ����������� �������(���. ����������)
void OrderManager::orderedRender()
{
   Update();
   for (DWORD i = 0; i < m_totalObjectCount; i++)
      m_orderedLine[i]->sbo_function(m_orderedLine[i]->sbo_instance);
}

// ����� ������ ������� ������������������ �������� � ������� �����������(���. ���������� �����������)
void OrderManager::disorederedRender()
{
   sb_object *cr = m_first;

   while (1)
   {
      cr->sbo_function(cr->sbo_instance);
      if (cr == m_last)
         break;
      else
         cr = cr->sbo_next;
	}
}

// ������������ �������� ������� ����������������(�.�. �� ����� ����� ����� ������ ������ �� ��������)
void OrderManager::ToggleRenderType()
{
    //��������� �� �������� ���������� �������
    //(sbm_currentRType == SBM_OREDERED) ? sbm_currentRType = SBM_DISOREDERED : (sbm_currentRType == SBM_DISOREDERED) ? sbm_currentRType = SBM_CHAOTIC : sbm_currentRType = SBM_OREDERED;
    (m_currentRType == SBM_ORDERED) ? m_currentRType = SBM_DISORDERED : m_currentRType = SBM_ORDERED;
}

// ���������� �������� �� ���������(������)
void OrderManager::defaultSort()
{
   //static sb_object *start = nullptr, *tmp = nullptr;
   static DWORD start = 0, finish = 0, tmp;
   static bool unsorted = false;
   start = 0;
   finish = m_totalObjectCount - 1;

   recountDistance();      //��������� ��� ��
   unsorted = false;
   while (start < finish)
   {
      tmp = start;
      while (tmp != finish)   //������ ���
      {
         if ((m_prior[tmp] < m_prior[tmp + 1])
            || ((m_prior[tmp] == m_prior[tmp + 1])
            && (m_dist[tmp] < m_dist[tmp + 1])))
         {
            unsorted = true;
            //std::swap(sbm_dist[tmp], sbm_dist[tmp + 1]);
            uts::fSwap(&m_dist[tmp], &m_dist[tmp + 1]);
            uts::fSwap(&m_prior[tmp], &m_prior[tmp + 1]);

            //std::swap(sbm_orderedLine[tmp], sbm_orderedLine[tmp + 1]);
            uts::fSwap(&m_orderedLine[tmp], &m_orderedLine[tmp + 1]);
         }
         tmp++;
      }
      if (!unsorted)
         break;
      unsorted = false;
      while (tmp != start)    //�������� ���
      {
         if ((m_prior[tmp] > m_prior[tmp - 1])
            || ((m_prior[tmp] == m_prior[tmp - 1])
            && (m_dist[tmp] > m_dist[tmp - 1])))
         {
            unsorted = true;

            uts::fSwap(&m_dist[tmp], &m_dist[tmp - 1]);
            uts::fSwap(&m_prior[tmp], &m_prior[tmp - 1]);

            uts::fSwap(&m_orderedLine[tmp], &m_orderedLine[tmp - 1]);
         }
         tmp--;
      }
      if (!unsorted)
         break;
      start++;
      finish--;
   }
}

// �������� ��������� �-� ������������������� ��������� � �������, � ����� ���������� �����������
void OrderManager::recountDistance()
{
   static D3DXVECTOR3 cam = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
   cam = m_engine->GetCamera()->GetCurrentPosition();

   for (DWORD i = 0; i < m_totalObjectCount; i++)
   {
      m_prior[i] = *m_orderedLine[i]->sbo_priority;
      m_dist[i] = D3DXVec3Length(&(cam - *m_orderedLine[i]->sbo_coordinates));
   }
}