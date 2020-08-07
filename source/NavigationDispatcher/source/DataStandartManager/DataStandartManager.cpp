//#include "stdafx.h"
//#include "DataStandartManager.h"
//#include <sstream>
//#include <iostream>
//
//using namespace data_standart;
//
//DataStandartManager::DataStandartManager(central_pack_ptr pack)
//   : Central(pack)
//{}
//
//void DataStandartManager::DeserializeConfig(const wchar_t* filename, command_meta& meta)
//{
//   std::wstring wname(filename);
//   std::string fname(wname.begin(), wname.end());
//}
//
//bool DataStandartManager::CheckMeta(const command_meta& meta)
//{
//   return true;
//}
//
//bool DataStandartManager::RunCommand(const command_meta& meta)
//{
//   size_t index = 0;
//   for (auto& task : meta.task_list)
//   {
//      for (auto& src : task.data_source_list)
//      {
//         for (auto& trg : task.data_target_list)
//         {
//            std::stringstream ss;
//            ss << "Running command shard #" << index << " with task: " << task.token << "; source: " << src << "; target: " << trg;
//            GetCommunicator()->Message(ICommunicator::MessageType::MT_INFO, ss.str().c_str());
//            m_processorList.at(task.token)(meta.data_source_list.at(src).standart, meta.data_source_list.at(trg).standart);
//            index++;
//         }
//      }
//   }
//   return true;
//}