#pragma once

/**
@file     gmtr.h
@brief    Support structures and defines(declarations)
@date     Created on 23/09/2014
@project  D3DBase
@author   Crush
*/


// *******************************************************************
// includes

#include "Library/cglApp.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>


// defines
#define VR_X 0
#define VR_Z 1


// Структура для функции()
/*
\class gmtr_gunc
\brief - Основной класс для работы с геометрией (т.е. создания данных в буфферах на основе исходного уравнения поверхности)
*/
class gmtr_func
{
public:
    //=========================
    gmtr_func(float as, float bs, float cs, float x0s, float y0s, float z0s, unsigned int count)
    {
        a = as; b = bs; c = cs; x0 = x0s; y0 = y0s; z0 = z0s;
        indexCount = 0; vertexCount = 0; primCount = 0;
        cellCountL = count;
        if (cellCountL > 100)
            cellCountL = 100;       //внутреннее ограничение('new')
        vertexBuf = 0;
        indexBuf = 0;
#ifdef _DEBUG
        d_file.open("debug_txt.txt", std::ios::app);
#endif
    }
    ~gmtr_func() { d_file.close(); }
private:
    //=========================
    unsigned int indexCount, vertexCount, primCount;
    float x0, y0, z0;
    float a, b, c;
    float dts;      //дельта сетки
    unsigned int cellCountL;    //количество ячеек по наибольшей стороне

    LPDIRECT3DDEVICE9 d3ddvc;
    IDirect3DIndexBuffer9 *indexBuf;
    IDirect3DVertexBuffer9 *vertexBuf;

    coordinates *vtTemp;
    int *indTemp;

    std::ofstream d_file;

    float *getCenterPosNDRad();     
    float *countHeight(float x, float z);   
    bool isInside(float x, float z);   

    //
    void mountFigure();
    void correctNDMount(coordinates cnt, float dts);
    bool ptsInFrame(coordinates cnt, float dts);
    bool ptInFrame(coordinates cnt);
    bool validRegrowth(short flag, coordinates cnt);
    void moveToBorder(coordinates &cnt);
    void mountTriangles(coordinates *pta, bool *inc);
    //

    void mountVertexBuf();     
    void mountIndexBuf();
    //void bufferizeObject();
    //
public:
    void setD3DDevice(LPDIRECT3DDEVICE9 dev) { d3ddvc = dev; }
    void bufferizeObject();
    IDirect3DVertexBuffer9 *getVertexBuffer() { return vertexBuf; }
    IDirect3DIndexBuffer9 *getIndexBuffer() { return indexBuf; }
    unsigned int getVertexCount() { return 2*vertexCount; }
    unsigned int getIndexCount() { return 2*indexCount; }
    unsigned int getPrimCount() { return 2*(indexCount / 3); }
};