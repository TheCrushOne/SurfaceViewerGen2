//#pragma once
//
//#include "common/central_class.h"
//#include "datastandart/DataStandart.h"
//#include "navdisp/NavDispTypes.h"
//#include <vector>
//#include <unordered_map>
//
//namespace data_standart
//{
//   const static std::unordered_map<navigation_dispatcher::OrderType, std::pair<DataStandartType, DataStandartType>> dstAccordanceMap =
//   {
//      { navigation_dispatcher::OT_PNGHMCONVERT, { DST_PNGHM, DST_SVGM } },
//      { navigation_dispatcher::OT_GENOBJLIST, { DST_SVGM, DST_OBJ } },
//      { navigation_dispatcher::OT_PATHFIND, { DST_SVGM, DST_PATHS } },
//      { navigation_dispatcher::OT_OPTPATH, { DST_SVGM, DST_OPTPATHS } },
//      { navigation_dispatcher::OT_PACKHOUND, { DST_UNKNOWN, DST_PCKHND } }
//   };
//
//   class DataStandartManager : public Central
//   {
//   public:
//      DataStandartManager(central_pack*);
//      void DeserializeConfig(const wchar_t* filename, command_meta& meta);
//      bool CheckMeta(const command_meta& meta);
//      bool RunCommand(const command_meta& meta);
//
//      void AddTaskLauncher(navigation_dispatcher::OrderType token, const TaskLauncher launcher) { m_processorList[token] = launcher; }
//   protected:
//   private:
//      //command_meta m_meta;
//      std::unordered_map<navigation_dispatcher::OrderType, TaskLauncher> m_processorList;
//   };
//}