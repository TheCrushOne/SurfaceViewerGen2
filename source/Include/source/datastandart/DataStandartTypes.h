#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "navdisp/NavDispTypes.h"

namespace SV::data_standart
{
   // hdat -> Height DATa
   // obj -> OBJects
   // pl -> Path List
   // opl -> Optimized Path List
   // phc -> Pack Hound Config

   enum DataStandartType : unsigned short
   {
      DST_UNKNOWN = 0,

      DST_PNGHM,        // [.png]
      DST_SVGM,         // [.hdat] + [.meta] // Surface Viewer Generalization Map
      DST_OBJ,          // [.obj]
      DST_PATHS,        // [.pl]
      DST_OPTPATHS,     // [.opl]
      DST_PCKHND,       // [.phc]
      DST_RESRES,       // [.phc]

      DST_MAX_VALUE,
   };

#define DECLARE_TYPE(type, name) case (type): return (name);

   inline LPCSTR convert_datastandart_type(DataStandartType type)
   {
      switch (type)
      {
         DECLARE_TYPE(DataStandartType::DST_PNGHM, "PngHeightMap")
         DECLARE_TYPE(DataStandartType::DST_SVGM, "SurfaceViewerGenMap")
         DECLARE_TYPE(DataStandartType::DST_OBJ, "ObjectData")
         DECLARE_TYPE(DataStandartType::DST_PATHS, "PathData")
         DECLARE_TYPE(DataStandartType::DST_OPTPATHS, "OptPathData")
         DECLARE_TYPE(DataStandartType::DST_PCKHND, "PackHound")
         DECLARE_TYPE(DataStandartType::DST_RESRES, "ResearchResult")
      }

      _ASSERT(!"Unknown data source type");
      return "";
   }

#undef DECLARE_TYPE

   inline DataStandartType convert_datastandart_name(LPCSTR name)
   {
      for (auto n = DataStandartType::DST_UNKNOWN + 1; n != DataStandartType::DST_MAX_VALUE; ++n)
      {
         DataStandartType type = static_cast<DataStandartType>(n);
         if (strcmp(name, convert_datastandart_type(type)) == 0)
            return type;
      }

      _ASSERT(!"Unknown data source name");
      return DataStandartType::DST_UNKNOWN;
   }

   /*struct standart_meta
   {
      data_standart::DataStandart* standart;
      data_standart::DataStandartType type;
      std::string name;
   };

   struct task_meta
   {
      navigation_dispatcher::OrderType token;
      std::vector<std::string> data_source_list;
      std::vector<std::string> data_target_list;
   };

   using meta_list = std::unordered_map<std::string, standart_meta>;

   struct command_meta
   {
      meta_list data_source_list;
      meta_list data_target_list;
      std::vector<task_meta> task_list;
   };*/

   //using TaskLauncher = std::function<bool(DataStandart * src, DataStandart * dst)>;
}