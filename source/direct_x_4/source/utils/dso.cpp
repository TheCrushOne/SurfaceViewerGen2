/**
@file     dso.cpp
@brief    Engine support draw sort order manager
@date     Created on 01/12/2014
@project  D3DBase
@author   Crush
*/

#include "dso.h"
#include "engine/engine.h"

const RTYPE sb_manager::SBM_DEFAULT_RENDER_TYPE = SBM_DISORDERED;
//==============================//Конструктор(вынесен из .h после введения диспетчера)
sb_manager::sb_manager(mb_engine *eng)
{      
    sbm_totalObjectCount = 0;
    sbm_engine = eng;
    sbm_first = nullptr;
    sbm_last = nullptr;  
    sbm_currentRType = SBM_DEFAULT_RENDER_TYPE;
}

//==============================//блок внешнего управления//

/*
\fn void registerObject(void(*frender)(), D3DXVECTOR3 *coord)
\brief Функция регистрации действующего объекта в списке объектов, подлежащих сортировке
\param frender - Указатель на рендер функцию конкретного объекта
\param coord - Указатель на вектор координат конкретного объекта
*/
void sb_manager::registerObject(void(*frender)(), D3DXVECTOR3 *coord)
{
    sbm_last = (sbm_last ? sbm_last->sbo_next : sbm_first) = new sb_object(frender, coord, sbm_last, sbm_totalObjectCount);
    sbm_totalObjectCount++;
}

/*
\fn void update()
\brief Фукнция пересортировки объектов(перед рендером(не заблокировано))
*/
void sb_manager::update()
{
    defaultSort();      //пока что(тут можно с вариантами пошаманить)
}

/*
\fn void orederedRender()
\brief Фукнция вызова рендер функций зарегистрированных объектов в опредленном порядке(алг. сортировки)
*/
void sb_manager::orederedRender()
{
	for (DWORD i = 0; i < sbm_totalObjectCount; i++)
		sbm_orederedLine[i]->sbo_function();
}

/*
\fn void orederedRender()
\brief Фукнция вызова рендер функций зарегистрированных объектов в порядке регистрации(алг. сортировки игнорирован)
*/
void sb_manager::disorederedRender()
{
	sb_object *cr = sbm_first;
	while (cr != sbm_last)
	{
		cr->sbo_function();
		cr = cr->sbo_next;
	}
}


/*
\fn void toggleRenderType()
\brief Фукнция переключения тумблера режимов псевдорендеринга(т.к. по факту метод этого класса ничего не рендерит)
*/
void sb_manager::toggleRenderType()
{
    (sbm_currentRType == SBM_ORDERED) ? sbm_currentRType = SBM_DISORDERED : (sbm_currentRType == SBM_DISORDERED) ? sbm_currentRType = SBM_CHAOTIC : sbm_currentRType = SBM_ORDERED;
}


/*
\fn void defaultSort()
\brief Фукнция сортировки объектов по умолчанию
\algorithm - Шейкерная сортировка
*/
void sb_manager::defaultSort()
{
	
	//static sb_object *start = nullptr, *tmp = nullptr;
	static DWORD start = 0, finish = 0, tmp;
	start = 0;
	finish = sbm_totalObjectCount - 1;

	recountDistance();

	while (start < finish)
	{
		tmp = start;
		while (tmp != finish - 1)
		{
			if (sbm_dist[tmp] > sbm_dist[tmp + 1])
			{
				std::swap(sbm_dist[tmp], sbm_dist[tmp + 1]);

				std::swap(sbm_orederedLine[tmp], sbm_orederedLine[tmp + 1]);
			}		
		}
		while (tmp != start + 1)
		{
			if (sbm_dist[tmp] < sbm_dist[tmp - 1])
			{
				std::swap(sbm_dist[tmp], sbm_dist[tmp - 1]);

				std::swap(sbm_orederedLine[tmp], sbm_orederedLine[tmp - 1]);
			}
		}
		start++;
		finish--;
	}
	
}


/*
\fn void recountDistance
\brief Функция пересчета дистанции м-у зарегистрированными объектами и камерой
*/
void sb_manager::recountDistance()
{
	static D3DXVECTOR3 cam = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	cam = sbm_engine->getCameraPtr()->getCurrentPosition();

	for (DWORD i = 0; i < sbm_totalObjectCount; i++)
		sbm_dist[i] = D3DXVec3Length(&(cam - *sbm_orederedLine[i]->sbo_coordinates));
}
