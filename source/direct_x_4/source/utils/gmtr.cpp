/**
@file     gmtr.�pp
@brief    Support structures and defines(definitions)
@date     Created on 23/09/2014
@project  D3DBase
@author   Crush
*/


#include "gmtr.h"
#include "engine/engine.h"

/*
\fn void mountVertexBuf()
\brief - ����������� ������ � ������ ������
*/
void gmtr_func::mountVertexBuf()
{

    vertex *pVtx = 0;
    float *ht;

    HRESULT hRes = vertexBuf->Lock(0, 0, (void**)(&pVtx), 0);
    if (hRes == D3D_OK)
    {
        //������ ������, � ���������� ��� ����������� ������������ � 1 ������ �� ���������
        for (unsigned int i = 0; i < vertexCount; i++)
        {
            ht = countHeight(vtTemp[i].x, vtTemp[i].z);
            pVtx[i].x = vtTemp[i].x;
            pVtx[i].z = vtTemp[i].z;
            pVtx[i].y = ht[0];
            //���� ������ ���� � ������ - ������������
            //pVtx[i].color = (DWORD)D3DCOLOR_XRGB((int)(pVtx[i].x * 10000) % 256, (int)(pVtx[i].z * 10000) % 256, (int)(ht[0] * 10000) % 256);
#ifdef _DEBUG        
            d_file << "vt : " << pVtx[i].x << " " << pVtx[i].z << std::endl;
#endif
            delete ht;
        }
        for (unsigned int i = vertexCount, j = 0 ; i < 2*vertexCount; i++, j++)
        {
            ht = countHeight(vtTemp[j].x, vtTemp[j].z);
            pVtx[i].x = vtTemp[j].x;
            pVtx[i].z = vtTemp[j].z;
            pVtx[i].y = ht[1];   
            //pVtx[i].color = (DWORD)D3DCOLOR_XRGB((int)(pVtx[i].x * 10000) % 256, (int)(pVtx[i].z * 10000) % 256, (int)(ht[1] * 10000) % 256);
#ifdef _DEBUG  
            d_file << "vt : " << pVtx[i].x << " " << pVtx[i].z << std::endl;
#endif
            delete ht;
        }
        vertexBuf->Unlock();
    }
    else
        MessageBoxW(0, (LPCWSTR)"d1", (LPCWSTR)"Render critical", MB_OK);

    //return vertexBuf;
}

/*
\fn void mountIndexBuf()
\brief - ����������� ������ � ������ ��������
*/
void gmtr_func::mountIndexBuf()
{
       

    WORD *k = 0;//new WORD[indexCount * 2];

    indexBuf->Lock(0, 0, (void**)(&k), 0);

    //
    for (unsigned int i = 0; i < indexCount/3; i++)
    {
        k[3 * i] = (WORD)indTemp[3 * i] ;
        k[3 * i + 1] = (WORD)indTemp[3 * i + 1];
        k[3 * i + 2] = (WORD)indTemp[3 * i + 2];
#ifdef _DEBUG        
        d_file << "ind : " << k[3 * i] << " " << k[3 * i + 1] << " " << k[3 * i + 2] << std::endl;
#endif
    }
    for (unsigned int i = indexCount/3, j = 0; i < 2*indexCount / 3; i++, j++)
    {
        k[3 * i] = (WORD)indTemp[3 * j] + vertexCount;
        k[3 * i + 1] = (WORD)indTemp[3 * j + 2] + vertexCount;
        k[3 * i + 2] = (WORD)indTemp[3 * j + 1] + vertexCount;
#ifdef _DEBUG  
        d_file << "ind : " << k[3 * i] << " " << k[3 * i + 1] << " " << k[3 * i + 2] << std::endl;
#endif
    }
    indexBuf->Unlock();

    //return pIB;
}

/*
\fn void bufferizeObject()
\brief - ��������� ������ ��� ��� ������� � ��� ������� �������� ������ � ��������
*/
void gmtr_func::bufferizeObject()
{
    //vtTemp = new coordinates[cellCountL*(cellCountL + 3)];
    vtTemp = new coordinates[cellCountL*cellCountL*8];      //4 ����� �� ������� + �������� �������
    //indTemp = new int[cellCountL*(cellCountL + 3) + cellCountL*(cellCountL + 3)/2];
    indTemp = new int[cellCountL*cellCountL * 12];      //8*1.5 �� ����� ����
    mountFigure();  

    d3ddvc->CreateVertexBuffer((2*vertexCount)*sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &vertexBuf, NULL);
    d3ddvc->CreateIndexBuffer((2*indexCount)*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuf, NULL);
    mountVertexBuf();
    mountIndexBuf();
}

/*
\fn float getCenterPosNDRad()
\brief - ������� ������ �������� ��������� ������ ���������� � ����������� ��������� �� a, c(�� ������������)
\return - ������ �� 4-�� ����� : ���������� ������ ���������� �� 3-�� ���� � ���������� �������� �� a, c
*/
float *gmtr_func::getCenterPosNDRad()      
{
    float *res = new float[4];

    res[0] = x0;
    res[1] = y0;
    res[2] = z0;
    res[3] = max(a, c);

    return res;
}

/*
\fn float countHeight()
\brief - ������� �������� 2-�� ��������� y �� ��������� x � z
\param x - ���������� x
\param z - ���������� z
\return - ������ �� 2-�� ����� : �� ������� � ������ ������ ����������(������ �� ��� OY)
*/
float *gmtr_func::countHeight(float x, float z)
{
    float *res = new float[2];

    res[0] = y0 + sqrt(fabs(b*b*(1 - (x - x0)*(x - x0) / (a*a) - (z - z0)*(z - z0) / (c*c))));
    res[1] = y0 - sqrt(fabs(b*b*(1 - (x - x0)*(x - x0) / (a*a) - (z - z0)*(z - z0) / (c*c))));

    return res;
}

/*
\fn bool isInside(float x, float z)
\brief - ������� �������� ��������� ����� �� ��������� XZ � �������� ���������� �� ��� ���������(�� ������������(��������))
\param x - ���������� x
\param z - ���������� z
\return - true, ���� ������, � ��������� ������ - false
*/
bool gmtr_func::isInside(float x, float z)
{
    return (((x - x0)*(x - x0) / (a*a) + (z - z0)*(z - z0) / (c*c)) <= 1);
}


//====================================//������! �������!//========================================//
//============//�������� ���������� ����� �� ������ � ������� � ������������ �������//============//
//==============================//��������, �� �� ����� ����������//==============================//
/*
\fn void mountFigure()
\brief - ������� �������� ����� �� ��������� XZ � ����������� �������������� � ���������� �����
*/
void gmtr_func::mountFigure()
{
    float dtx, dtz;
    //����� ���������� ����������(���� ���); ��� ��������� ������ ���������� ������(� ����������� ������� �� 2 ����������� ���������)
    a > c ? dts = 2 * a / cellCountL, dtx = dts*(cellCountL / 2 + 1), dtz = dtx : dts = 2 * c / cellCountL, dtz = dts*(cellCountL / 2 + 1), dtx = dtz;

    //������� ������
    coordinates cnt(x0 - dtx, z0 + dtz);

    while (cnt.z >= z0 - dtz)
    {
        cnt.x = x0 - dtx;
        while ((!ptsInFrame(cnt, dts)) && (cnt.x <= x0 + dtx))  //���� ������ �� ����������� �������� ���������� �� XZ - ���������� �
            cnt.x += dts;
		if (cnt.x <= x0 + dtx)
		{
			while (ptsInFrame(cnt, dts))
			{
				correctNDMount(cnt, dts);   //�������������� ������ ������ ���� 1-3 ������� �� ������ � ��������
				cnt.x += dts;
			}
		}
        //�������, ��� ����� ���� ���� ������ ������ �������� ��� ���....
        cnt.z -= dts;
    }
}

//============================
/*
\fn void correctNDMount(coordinates cnt, float dts)
\brief - ������� �������������(������ ���������, �� ��������� �������� � ��������) � ��������� ������ � ������� ������
*/
void gmtr_func::correctNDMount(coordinates cnt, float dts)
{
    coordinates pts[4];
    bool inc[4];        //������ ������ ��������� � ��������
    unsigned int sum = 0;
    bool spikeFlag = false;

    pts[0].x = cnt.x - dts / 2.0f;
    pts[0].z = cnt.z + dts / 2.0f;

    pts[1].x = cnt.x + dts / 2.0f;
    pts[1].z = cnt.z + dts / 2.0f;

    pts[2].x = cnt.x + dts / 2.0f;
    pts[2].z = cnt.z - dts / 2.0f;

    pts[3].x = cnt.x - dts / 2.0f;
    pts[3].z = cnt.z - dts / 2.0f;

    for (int i = 0; i < 4; sum += inc[i], i++)
    {
        inc[i] = 0;
        if (ptInFrame(pts[i]))
            inc[i] = 1;
    }

    
    if ((sum == 3)
        || (sum == 2))
    {
        for (int j = 0; j < 4; j++)
        {

            if (!(inc[j]))
            {
                moveToBorder(pts[j]);
                inc[j] = 1;
            }
        }
    }
    else if (sum == 1)
    {
        for (int j = 0; ((j < 4)&&(!spikeFlag)); j++)
        {
            if (inc[j])
            {                
                moveToBorder(pts[(4 - j + 1) % 4]);     //���������
                moveToBorder(pts[(4 - j - 1) % 4]);
                inc[(4 - j + 1) % 4] = 1;
                inc[(4 - j - 1) % 4] = 1;
                spikeFlag = true;
            }
        }
    }
    mountTriangles(pts, inc);
}

/*
\fn bool ptsInFrame(coordinates cnt, float dts)
\brief - ������� �������� ��������� 4 ����� � �������� ����������, �������������� �� cnt �� sqrt(2*dts^2)
\param cnt - ���������� xz ������ ������������ ��������
\param dts - �������� ������� ����� �� ������ �� ����
\return - true, ���� ������ ���� �� ���� �� �������, � ��������� ������ - false
*/
bool gmtr_func::ptsInFrame(coordinates cnt, float dts)
{
    coordinates pts[4];

    pts[0].x = cnt.x - dts / 2.0f;
    pts[0].z = cnt.z + dts / 2.0f;

    pts[1].x = cnt.x + dts / 2.0f;
    pts[1].z = cnt.z + dts / 2.0f;

    pts[2].x = cnt.x + dts / 2.0f;
    pts[2].z = cnt.z - dts / 2.0f;

    pts[3].x = cnt.x - dts / 2.0f;
    pts[3].z = cnt.z - dts / 2.0f;


    for (int i = 0; i < 4; i++)		//
    {

		if (((pts[i].x*pts[i].x - 2 * pts[i].x*x0 + x0*x0) / (a*a) + (pts[i].z*pts[i].z - 2 * pts[i].z*z0 + z0*z0) / (c*c)) <= 1) 
			return true;
    }
    return false;
}

/*
\fn bool ptsInFrame(coordinates cnt, float dts)
\brief - ������� �������� ��������� ����� � ������������ cnt � �������� ����������
\param cnt - ���������� xz ����������� �����
\return - true, ���� ������, � ��������� ������ - false
*/
bool gmtr_func::ptInFrame(coordinates cnt)
{
	if (((cnt.x*cnt.x - 2 * cnt.x*x0 + x0*x0) / (a*a) + (cnt.z*cnt.z - 2 * cnt.z*z0 + z0*z0) / (c*c)) <= 1)
        return true;
    return false;
}

/*
\fn bool validRegrowth(short flag, coordinates cnt)
\brief - ������� �������� ����������� ������� ����� � ������� �������� ����������(��� ����� �� ��������� ��������)(����� ������, ���� ��������� �� ��������� ��� ����� �������� ��������� � ��������..)
\param cnt - ���������� xz ����������� �����
\param flag - x ��� z, � ����������� �� ������������ �����������
\return - true, ���� ������ �� ��������� ��� ��������, � ��������� ������ - false
*/
bool gmtr_func::validRegrowth(short flag, coordinates cnt)
{
    if (flag == VR_X)    
        if ((cnt.x >= (x0 - a))
            && (cnt.x <= (x0 + a)))
            return true;    
    else if (flag == VR_Z)    
        if ((cnt.z >= (z0 - c))
            && (cnt.z <= (z0 + c)))
            return true;    
    return false;
}

/*
\fn void moveToBorder(coordinates &cnt)
\brief - ������� ������� ���������� ����� � ������� ������� �������� ����������
\param cnt - ���������� xz ����������� �����
*/
void gmtr_func::moveToBorder(coordinates &cnt)
{
    float zb = 1.0f, xb = 1.0f;
    //��������� ����� ����� ����������� �� ��������

    fabs((sqrt(fabs(1 - (cnt.x*cnt.x - 2 * cnt.x*x0 + x0*x0) / (a*a)))*c + z0) - cnt.z) >= fabs((-sqrt(fabs(1 - (cnt.x*cnt.x - 2 * cnt.x*x0 + x0*x0) / (a*a)))*c + z0) - cnt.z) ? zb = ((-sqrt(fabs(1 - (cnt.x*cnt.x - 2 * cnt.x*x0 + x0*x0) / (a*a)))*c + z0)) : zb = ((sqrt(fabs(1 - (cnt.x*cnt.x - 2 * cnt.x*x0 + x0*x0) / (a*a)))*c + z0));
        
    fabs((sqrt(fabs(1 - (cnt.z*cnt.z - 2 * cnt.z*z0 + z0*z0) / (c*c)))*a + x0) - cnt.x) >= fabs((-sqrt(fabs(1 - (cnt.z*cnt.z - 2 * cnt.z*z0 + z0*z0) / (c*c)))*a + x0) - cnt.x) ? xb = ((-sqrt(fabs(1 - (cnt.z*cnt.z - 2 * cnt.z*z0 + z0*z0) / (c*c)))*a + x0)) : xb = ((sqrt(fabs(1 - (cnt.z*cnt.z - 2 * cnt.z*z0 + z0*z0) / (c*c)))*a + x0));
 
    if (!validRegrowth(VR_X, cnt))
        cnt.x = xb;
    else if (!validRegrowth(VR_Z, cnt))
        cnt.z = zb;
    else
	    fabs(xb - cnt.x) < fabs(zb - cnt.z) ? cnt.x = xb : cnt.z = zb;

}

/*
\fn void moveToBorder(coordinates &cnt)
\brief - ������� ������ ������ �� ��������� ������� ������ � �������� � ���������� �� ����������� ���������
\param cnt - ���������� xz ������������ �����
\param inc - ����� ������������ �����(�.�. ������ ��� ������� 1-�� �� 4-�� ����� ��������(� ���� ������ ��������� ���))
*/
void gmtr_func::mountTriangles(coordinates *pta, bool *inc)
{
    
    int i, j/*, k*/;

    for (i = 0, j = 0; i < 4; i++)
    {
        if (inc[i] == 1)
        {
            vtTemp[vertexCount + j].x = pta[i].x;
            vtTemp[vertexCount + j].z = pta[i].z;
            j++;
        }
    }

    if (i == j)
    {
        indTemp[indexCount] = vertexCount;
        indTemp[indexCount + 1] = vertexCount + 1;
        indTemp[indexCount + 2] = vertexCount + 3;
        indTemp[indexCount + 3] = vertexCount + 1;
        indTemp[indexCount + 4] = vertexCount + 2;
        indTemp[indexCount + 5] = vertexCount + 3;
        vertexCount += 4;
        indexCount += 6;
    }
    else
    {
        indTemp[indexCount] = vertexCount;
        indTemp[indexCount + 1] = vertexCount + 1;
        indTemp[indexCount + 2] = vertexCount + 2;
        vertexCount += 3;
        indexCount += 3;
    }



}