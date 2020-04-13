#pragma once

#include "colreg/DebugStructs.h"

namespace dbg
{
   using dbg_info_node_path_vct = std::vector<std::wstring>;
   using dbg_points_vct = std::vector<dbg_point>;

   void* CreateDebugInfoHolder();

   bool AddExternalDebugInfo(const char* name, void* extDebugger);
   iDebugInfo* CreateDebugInfoManager();
   iDebugInfo* GetDebugInfoManager();
   void DestroyDebugger();

   bool IsDebuggingEnabled();

   void ClearDebugInfo(const char* moduleName = "");

   bool IsDebuggingEnabled(colreg::id_type id);

   void ClearDebugNode(const dbg_info_node_path_vct& nodePath);

   void AddPoints(colreg::id_type id, dbg_info_node_path_vct nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz = DEFAULT_PIX_SIZE, size_t color = DEFAULT_COLOR);
   void AddPoints(const dbg_info_node_path_vct& nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz = DEFAULT_PIX_SIZE, size_t color = DEFAULT_COLOR);

   void AddLine(colreg::id_type id, dbg_info_node_path_vct nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz = DEFAULT_PIX_SIZE, size_t color = DEFAULT_COLOR, PointsLinkage ptsLinkage = PointsLinkage::PL_SOLID);
   void AddLine(const dbg_info_node_path_vct& nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz = DEFAULT_PIX_SIZE, size_t color = DEFAULT_COLOR, PointsLinkage ptsLinkage = PointsLinkage::PL_SOLID);

   void AddArc(colreg::id_type id, dbg_info_node_path_vct nodePath, const dbg_point& center, double radius,
      double beg, double end, double step,
      const std::string& text, size_t pixSz = DEFAULT_PIX_SIZE, size_t color = DEFAULT_COLOR, PointsLinkage ptsLinkage = PointsLinkage::PL_SOLID);
   void AddArc(const dbg_info_node_path_vct& nodePath, const dbg_point& center, double radius,
      double beg, double end, double step,
      const std::string& text, size_t pixSz = DEFAULT_PIX_SIZE, size_t color = DEFAULT_COLOR, PointsLinkage ptsLinkage = PointsLinkage::PL_SOLID);
}