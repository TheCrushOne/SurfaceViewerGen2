#pragma once

/**
@file     utils.h
@brief    Support structures and defines(declarations)
@date     Created on 23/09/2014
@project  D3DBase
@author   Crush
*/

#pragma warning(disable : 4316)
#pragma warning(disable : 4996)
// *******************************************************************
// includes

#include <d3d9.h>
#include <d3dx9.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>

#include "typecluster.h"

// Координаты на плоскости XZ для создание сетки(временно не работает)
struct coordinates
{
    coordinates() { x = 0.0f; z = 0.0f; };
    coordinates(float xs, float zs) { x = xs; z = zs; };

    float x;
    float z;
};


// Данные вершины (заброшено(но не факт....))
struct vertex
{
    float x, y, z;
    //float rhw, u, v;
    float nx, ny, nz;    
    //DWORD color;
};

namespace uts
{
    static const DWORD MAX_LENGTH = 80;
    static const DWORD AME_EXCEPTION_COUNT = 5;
    static const char AME_EXCEPTION_DEFINITION[5][uts::MAX_LENGTH] = {
#include "excdef.amlss"
    };

    /*
    \enum sb_code
    \brief Перечисление типов ошибок
    */
    enum sb_code
    {
        SBC_MESHLOADFAILURE = 100,  //чтоб с другими не пересекалось
        SBC_EMPTYTEXTURECREATIONFAILURE,
        SBC_SUBTEXTURELOADFAILURE,
        SBC_FONTCREATEFAILURE,
        SBC_SHADERLOADFAILURE,
    };

    /*
    \class AMLSS_E
    \brief Anti Memory Leak Security System Exception class
    */
    class AMLSS_E : std::runtime_error
    {
    public:
        //===========================================
        AMLSS_E(sb_code eType, const char *msg) : runtime_error(""), ame_eTarget(msg), ame_tCode(eType)
        {
            std::stringstream ss;
            ss << "0x" << std::hex << eType;
            ame_eCode = ss.str();
        }
        _declspec(nothrow) virtual ~AMLSS_E() {}
        //===========================================
        _declspec(nothrow) virtual const char* what()
        {
            std::string exc = std::string("app halted with AMLSS_E code : ") + ame_eCode + std::string(" : ") + std::string(AME_EXCEPTION_DEFINITION[ame_tCode - SBC_MESHLOADFAILURE]) + std::string(" on object ") + ame_eTarget;
            char * cstr = new char[200];
            std::strcpy(cstr, exc.c_str());
            return cstr;
        }


    private:
        //static const DWORD AME_EXCEPTION_COUNT;
        //static const char AME_EXCEPTION_DEFINITION[][uts::MAX_LENGTH];

        sb_code ame_tCode;
        std::string ame_eCode;
        std::string ame_eTarget;
    };

    //inline с inline asm-ом

    inline void fSwap(LPVOID pt1, LPVOID pt2)
    {
        _asm
        {
            mov esi, pt1
            mov eax, [esi]
            mov edi, pt2
            mov edx, [edi]
            mov[esi], edx
            mov[edi], eax
        }
    }

    inline float fAbs(float val)
    {
        _asm
        {
            fld val
            fabs
            fstp val
        }
        return val;
    }

};

//направления движения + переходные типы(заброшено)
#define DT_XU 1
#define DT_XD 2
#define DT_ZU 3
#define DT_ZD 4
#define DT_XUZU 5
#define DT_ZUXD 6
#define DT_XDZD 7
#define DT_ZDXU 8

#define SHAMANISTIC_INSTANT_CAST(x) x=-x

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 