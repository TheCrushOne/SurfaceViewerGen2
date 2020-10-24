#pragma once

#include "pathfinder_types.h"
#include "SVCG/route_point.h"
#include "SVCG/geo_point.h"
#include "common/header_collector.h"

namespace SV::pathfinder
{
   template<class type>
   class Matrix
   {
   public:
      Matrix(size_t rowCount, size_t colCount, type inVal = type())
         : m_inVal(inVal)
      {
         SetRowCount(rowCount);
         SetColCount(colCount);
      }
      Matrix(type inVal = type())
         : m_rowCount(0)
         , m_colCount(0)
         , m_inVal(inVal)
      {}
      Matrix(const Matrix& mtx)
         : m_inVal(mtx.GetInVal())
      {
         SetRowCount(mtx.GetRowCount());
         SetColCount(mtx.GetColCount());
         for (size_t rIdx = 0; rIdx < m_rowCount; rIdx++)
         {
            for (size_t cIdx = 0; cIdx < m_colCount; cIdx++)
               Set(rIdx, cIdx, mtx.Get(rIdx, cIdx));
         }
      }

      type Get(size_t rIdx, size_t cIdx) const
      {
         if (m_rowCount <= rIdx || m_colCount <= cIdx)
         {
            _ASSERT(false);
            return m_inVal;
         }
         return m_data.at(rIdx).at(cIdx);
      }
      void Set(size_t rIdx, size_t cIdx, type val)
      {
         if (m_rowCount > rIdx && m_colCount > cIdx)
            m_data[rIdx][cIdx] = val;
      }

      void SetRowCount(size_t rowCount)
      {
         m_rowCount = rowCount;
         m_data.resize(m_rowCount);
         SetColCount(m_colCount);
      }
      void SetColCount(size_t colCount)
      {
         m_colCount = colCount;
         for (auto& vct : m_data)
            vct.resize(m_colCount);
      }

      size_t GetRowCount() const { return m_rowCount; }
      size_t GetColCount() const { return m_colCount; }
      type GetInVal() const { return m_inVal; }
   protected:
      size_t m_rowCount = 0, m_colCount = 0;
      type m_inVal;
      std::vector<std::vector<type>> m_data;
   };

   class GeoMatrix : public Matrix<double>
   {
   public:
      GeoMatrix(double inVal = double())
         : Matrix(inVal)
      {}
      GeoMatrix(std::vector<std::vector<double>> source)
      {
         m_rowCount = source.size();
         m_colCount = m_rowCount > 0 ? source.at(0).size() : 0;
#ifdef _DEBUG
         for (size_t idx = 0; idx < m_rowCount; idx++)
            _ASSERT(source.at(idx).size() == m_colCount);
#endif
         m_data = source;
      }
      GeoMatrix(size_t rowCount, size_t colCount, double inVal = double())
         : Matrix(rowCount, colCount, inVal)
      {}
      GeoMatrix(const GeoMatrix& mtx)
         : Matrix(mtx)
      {}

      GeoMatrix operator*(const GeoMatrix& right)
      {
         _ASSERT(GetColCount() == right.GetRowCount());
         GeoMatrix res(GetRowCount(), right.GetColCount());
         for (size_t resRowIdx = 0; resRowIdx < res.GetRowCount(); resRowIdx++)
         {
            for (size_t resColIdx = 0; resColIdx < res.GetColCount(); resColIdx++)
            {
               double elem = 0.;
               for (size_t subIdx = 0; subIdx < right.GetRowCount(); subIdx++)
                  elem += Get(resRowIdx, subIdx) * right.Get(subIdx, resColIdx);
               res.Set(resRowIdx, resColIdx, elem);
            }
         }
         return res;
      }

      double Min() const
      {
         auto rowCount = GetRowCount(), colCount = GetColCount();
         if (rowCount < 1 || colCount < 1)
            return GetInVal();
         double min = Get(0, 0);
         for (size_t ridx = 0; ridx < rowCount; ridx++)
         {
            for (size_t cidx = 0; cidx < colCount; cidx++)
            {
               if (Get(ridx, cidx) < min)
                  min = Get(ridx, cidx);
            }
         }
         return min;
      }

      double Max() const
      {
         auto rowCount = GetRowCount(), colCount = GetColCount();
         if (rowCount < 1 || colCount < 1)
            return GetInVal();
         double max = Get(0, 0);
         for (size_t ridx = 0; ridx < rowCount; ridx++)
         {
            for (size_t cidx = 0; cidx < colCount; cidx++)
            {
               if (Get(ridx, cidx) > max)
                  max = Get(ridx, cidx);
            }
         }
         return max;
      }
   };

   class RoutePointMatrix : public Matrix<CG::route_point>
   {
   public:
      RoutePointMatrix(CG::route_point inVal = CG::route_point{})
         : Matrix(inVal)
      {}
      RoutePointMatrix(CG::route_line_vct& source)
      {
         m_rowCount = source.size();
         m_colCount = m_rowCount > 0 ? source.at(0).size() : 0;
#ifdef _DEBUG
         for (size_t idx = 0; idx < m_rowCount; idx++)
            _ASSERT(source.at(idx).size() == m_colCount);
#endif
         m_data = source;
      }
      RoutePointMatrix(size_t rowCount, size_t colCount, double inVal = double())
         : Matrix(rowCount, colCount, inVal)
      {}
      RoutePointMatrix(const RoutePointMatrix& mtx)
         : Matrix(mtx)
      {}
   };
   typedef std::shared_ptr<RoutePointMatrix> SharedRoutePointMatrix;

   typedef Matrix<CG::geo_point> GeoPointMatrix;
   typedef std::shared_ptr<GeoPointMatrix> SharedGeoPointMatrix;

   typedef Matrix<size_t> UnsignedMatrix;
   typedef std::shared_ptr<UnsignedMatrix> SharedUnsignedMatrix;

   typedef Matrix<bool> BoolMatrix;
   typedef std::shared_ptr<BoolMatrix> SharedBoolMatrix;
}