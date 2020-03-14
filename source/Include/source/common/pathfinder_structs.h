#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "pathfinder_types.h"
#include "SVCG/route_point.h"
#include "common/settings.h"

namespace pathfinder
{
   template<class type>
   class Matrix
   {
   public:
      Matrix(size_t rowCount, size_t colCount, type inVal = inVal())
         : m_rowCount(rowCount)
         , m_colCount(colCount)
         , m_inVal(inVal)
      {
         m_data.resize(m_rowCount);
         for (auto& vct : m_data)
            vct.resize(m_colCount);
      }
      Matrix(type inVal = inVal())
         : m_rowCount(0)
         , m_colCount(0)
         , m_inVal(inVal)
      {}
      Matrix(const Matrix& mtx)
         : m_rowCount(mtx.GetRowCount())
         , m_colCount(mtx.GetColCount())
         , m_inVal(mtx.GetInVal())
      {
         for (size_t rIdx = 0; rIdx < m_rowCount; rIdx++)
         {
            for (size_t cIdx = 0; cIdx < m_colCount; cIdx++)
               Set(rIdx, cIdx, mtx.Get(rIdx, cIdx));
         }
      }

      type Get(size_t rIdx, size_t cIdx) const
      {
         if(m_rowCount <= rIdx || m_colCount <= cIdx)
            return m_inVal;
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
   private:
      size_t m_rowCount, m_colCount;
      type m_inVal;
      std::vector<std::vector<type>> m_data;
   };

   struct check_fly_zone_result
   {
      FlyZoneAffilation fza;
   };

   struct check_go_zone_result
   {
      GoZoneAffilation gza;
      double asn, awe;   // angle south-north, angle west-east
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

      bool use_strategies;   // флаг использования стратегий(для исследований)
      bool research; // флаг исследовательского пробега
      bool land_path; // необходимость расчета наземного пути
      size_t packet_size;   // размер пакета путей для многопоточного вызова(0 - все задачи в 1 пул)

      path_finder_settings(bool multithread = true, std::vector<size_t> statFieldIndex = {}, bool research = false, bool landPath = true, size_t packetSize = 2)
         : multithread(multithread)
         , stat_field_index(statFieldIndex)
         , research(research)
         , land_path(landPath)
         , packet_size(packetSize)
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