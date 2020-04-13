#pragma once

#include "BaseRef.h"
#include "CommonStructs.h"

namespace dbg
{
   const size_t        DEFAULT_COLOR = 255;
   const size_t        DEFAULT_PIX_SIZE = 3;

   using dbg_info_node_path_ref = colreg::base_ref<const wchar_t*>;
   using dbg_point = colreg::geo_point;
   using dbg_points_ref = colreg::base_ref<dbg_point>;

   enum class PointsLinkage
   {
      PL_NONE = 0,
      PL_SOLID,
      PL_DASH,        // -------
      PL_DOT,         // .......
      PL_DASH_DOT,    // _._._._
      PL_DASH_DOT_DOT // _.._.._
   };

   const PointsLinkage DEFAULT_LINKAGE = PointsLinkage::PL_NONE;

#pragma pack (push, 1)
   struct debug_drawing_info
   {
      const char* text;    ///< “екст, отображаемый р€дом с точкой

      size_t pix_size;       ///< –азмер точки при отрисовке, пиксель (nullptr - использовать родительский)
      size_t color;          ///< ÷вет при отрисовке (nullptr - использовать родительский)

      PointsLinkage points_linkage; ///< —оединение точек
   };

   struct debug_info_node;
   using debug_info_node_ref = colreg::base_ref<debug_info_node>;
   using debug_info_node_roots_ref = colreg::base_ref<const debug_info_node*>;

   struct debug_info_node
   {
      const wchar_t* name;

      colreg::geo_points_ref points;
      debug_info_node_ref subnodes;

      debug_drawing_info drawing;
   };

   struct iDebugInfo
   {
      virtual const debug_info_node_roots_ref* GetNodes() const = 0;
      virtual void Enable(colreg::id_type id, bool enable) = 0;
      virtual bool IsEnabled(colreg::id_type id) const = 0;

      virtual void AddPoints(colreg::id_type id, const dbg_info_node_path_ref& nodePath, const dbg_points_ref& pts, const char* text = nullptr, size_t pixSz = dbg::DEFAULT_PIX_SIZE, size_t color = dbg::DEFAULT_COLOR) = 0;
      virtual void AddPoints(const dbg_info_node_path_ref& nodePath, const dbg_points_ref& pts, const char* text = nullptr, size_t pixSz = dbg::DEFAULT_PIX_SIZE, size_t color = dbg::DEFAULT_COLOR) = 0;
      virtual void AddLine(colreg::id_type id, const dbg_info_node_path_ref& nodePath, const dbg_points_ref& pts, const char* text = nullptr, size_t pixSz = dbg::DEFAULT_PIX_SIZE, size_t color = dbg::DEFAULT_COLOR, PointsLinkage ptsLinkage = dbg::PointsLinkage::PL_SOLID) = 0;
      virtual void AddLine(const dbg_info_node_path_ref& nodePath, const dbg_points_ref& pts, const char* text = nullptr, size_t pixSz = dbg::DEFAULT_PIX_SIZE, size_t color = dbg::DEFAULT_COLOR, PointsLinkage ptsLinkage = dbg::PointsLinkage::PL_SOLID) = 0;
      virtual void AddArc(colreg::id_type id, dbg_info_node_path_ref& nodePath, const dbg_point& center, double radius,
         double beg, double end, double step,
         const char* text = nullptr, size_t pixSz = dbg::DEFAULT_PIX_SIZE, size_t color = dbg::DEFAULT_COLOR, PointsLinkage ptsLinkage = dbg::PointsLinkage::PL_SOLID) = 0;
      virtual void AddArc(const dbg_info_node_path_ref& nodePath, const dbg_point& center, double radius,
         double beg, double end, double step,
         const char* text = nullptr, size_t pixSz = dbg::DEFAULT_PIX_SIZE, size_t color = dbg::DEFAULT_COLOR, PointsLinkage ptsLinkage = dbg::PointsLinkage::PL_SOLID) = 0;

      virtual  ~iDebugInfo() = default;
   };
#pragma pack (pop)
}