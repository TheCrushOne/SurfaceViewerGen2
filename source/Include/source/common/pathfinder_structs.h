#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "pathfinder_types.h"
#include "SVCG/route_point.h"
#include "common/header_collector.h"

namespace pathfinder
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
                  elem += Get(resRowIdx, subIdx)*right.Get(subIdx, resColIdx);
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

   struct check_fly_zone_result
   {
      FlyZoneAffilation fza;
   };

   struct check_go_zone_result
   {
      GoZoneAffilation gza;
      double asn; // angle south-north
      double awe; // angle west-east
      double aswne;  // angle southwest-northeast
      double asenw;  // angle southeast-northwest
   };

   struct strategy_settings
   {
      StrategyType type;
      double radius;
   };

   struct route_data
   {
      std::vector<settings::route> land_routes;
      std::vector<settings::route> air_routes;
      route_data() {}
      route_data(std::vector<settings::route>& landRoutes, std::vector<settings::route>& airRoutes)
         : land_routes(landRoutes)
         , air_routes(airRoutes)
      {}
   };

   using aff_checker = std::function<bool(const std::shared_ptr<Matrix<SVCG::route_point>>&, std::shared_ptr<Matrix<size_t>>&, size_t, size_t)>;

   struct path_finder_logic
   {
      aff_checker checker;
      height_corrector corrector;
      path_finder_logic(aff_checker checker, height_corrector corrector)
         : checker(checker)
         , corrector(corrector)
      {}
   };

   // А это рантайм? или че? а то дублируются...
   // Настройки патфайндера...и в принципе всё..
   struct path_finder_settings
   {
      bool multithread;

      // NOTE: Это для статистики
      std::vector<size_t> stat_field_index;    // Статистическое поле - н-мерная матрица статистики

      bool use_strategies; // флаг использования стратегий(для исследований)
      bool research;       // флаг исследовательского пробега
      bool land_path;      // необходимость расчета наземного пути
      size_t packet_size;  // размер пакета путей для многопоточного вызова(0 - все задачи в 1 пул)
      size_t thread_count; // количество запрашиваемых потоков

      path_finder_settings(bool multithread = true, std::vector<size_t> statFieldIndex = {}, bool research = false, bool landPath = true, size_t packetSize = 2, size_t threadCount = 8, bool useStrategies = true)
         : multithread(multithread)
         , stat_field_index(statFieldIndex)
         , research(research)
         , land_path(landPath)
         , packet_size(packetSize)
         , thread_count(threadCount)
         , use_strategies(useStrategies)
      {}
   };

   // Статистика для вывода в файл
   struct path_finder_statistic
   {
      // Пока что пусто
   };

   struct path_finder_indata
   {
      settings::unit_source_data unit_data;
      path_finder_settings settings;
      path_finder_statistic statistic;
      strategy_settings strategy_settings;
   };
}