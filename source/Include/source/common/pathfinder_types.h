#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "SVCG/route_point.h"

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
               Set(rIx, cIdx, mtx.Get(rIdx, cIdx));
         }
      }

      type Get(size_t rIdx, size_t cIdx) const { ATLASSERT(m_rowCount > rIdx&& m_colCount > cIdx); return m_data.at(rIdx).at(cIDx); }
      void Set(size_t rIdx, size_t cIdx, type val) { ATLASSERT(m_rowCount > rIdx&& m_colCount > cIdx); m_data[rIdx][cIDx] = val; }

      size_t GetRowCount() const { return m_rowCount; }
      size_t GetColCount() const { return m_colCount; }
      type GetInVal() const { return m_inVal; }
   private:
      size_t m_rowCount, m_colCount;
      type m_inVal;
      std::vector<std::vector<type>> m_data;
   };

   enum class FlyZoneAffilation : unsigned short
   {
      FZA_NORMAL,
      FZA_DANGEROUS,
      FZA_FORBIDDEN,
   };
   enum class GoZoneAffilation : unsigned short
   {
      GZA_NORMAL,
      GZA_DANGEROUS,
      GZA_FORBIDDEN,
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

   enum StrategyType : unsigned short
   {
      ST_RHOMBOID = 0,
      ST_SECTOR,
      ST_SNAKE
   };

   struct strategy_settings
   {
      StrategyType type;
      double radius;
   };

   struct route
   {
      SVCG::route_point start;
      SVCG::route_point finish;
      std::vector<SVCG::route_point> control_point_list;
      std::vector<SVCG::route_point> route_list;
      route() {}
      route(SVCG::route_point& start, SVCG::route_point& finish)
         : start(start)
         , finish(finish)
         , control_point_list()
         , route_list()
      {}
      route(SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& route, std::vector<SVCG::route_point>& controlPointList)
         : start(start)
         , finish(finish)
         , control_point_list(controlPointList)
         , route_list(route)
      {}
      route(SVCG::route_point& start, SVCG::route_point& finish, std::vector<SVCG::route_point>& controlPointList)
         : start(start)
         , finish(finish)
         , control_point_list(controlPointList)
         , route_list()
      {}
   };

   struct route_data
   {
      std::vector<route> land_routes;
      std::vector<route> air_routes;
      route_data() {}
      route_data(std::vector<route>& landRoutes, std::vector<route>& airRoutes)
         : land_routes(landRoutes)
         , air_routes(airRoutes)
      {}
   };

   using aff_checker = std::function<bool(std::shared_ptr<Matrix<SVCG::route_point>>&, std::shared_ptr<Matrix<size_t>>&, size_t, size_t)>;
   using height_corrector = std::function<float(float)>;

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

      bool research; // флаг исследовательского пробега
      bool land_path; // необходимость расчета наземного пути
      size_t packet_size;   // размер пакета путей для многопоточного вызова(0 - все задачи в 1 пул)

      path_finder_settings(bool multithread = true, std::vector<size_t> statFieldIndex = {}, bool research = false, bool landPath = true, size_t packetSize = 0)
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
}