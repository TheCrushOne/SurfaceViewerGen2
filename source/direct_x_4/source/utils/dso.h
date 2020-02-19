#pragma once

/**
@file     dso.h
@brief    Engine support draw sort order manager
@date     Created on 01/12/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"

class mb_engine;

/*
\class mb_engine
\brief - Специализированный класс, сортирующий объекты по дистанции до камеры
\
*/
class sb_manager    //pref : sbm_
{
    // 
    struct sb_object
    {
        void (*sbo_function)();
        D3DXVECTOR3 *sbo_coordinates;

        sb_object *sbo_prev;
        sb_object *sbo_next;

        DWORD sbo_serialNumber;

        sb_object(void(*frender)(), D3DXVECTOR3 *coord, sb_object *prev, DWORD serial) { sbo_function = frender; sbo_coordinates = coord; sbo_prev = prev; prev ? (prev->sbo_next = this) : 0; sbo_next = nullptr; sbo_serialNumber = serial; }
    };
    
	sb_object **sbm_orederedLine;
	float *sbm_dist;
    sb_object *sbm_first;
    sb_object *sbm_last;       
    
    DWORD sbm_totalObjectCount;

    mb_engine *sbm_engine;

    RTYPE sbm_currentRType;

    static const RTYPE SBM_DEFAULT_RENDER_TYPE;

public:
    sb_manager(mb_engine *eng);
    ~sb_manager() { while (sbm_last != sbm_first){ sbm_last = sbm_last->sbo_prev; delete sbm_last->sbo_next; } }
    
    void registerObject(void(*frender)(), D3DXVECTOR3 *coord);

    void update();

    void orederedRender();
    void disorederedRender();

    void toggleRenderType();
private:
    void defaultSort();
	void recountDistance();
	
};