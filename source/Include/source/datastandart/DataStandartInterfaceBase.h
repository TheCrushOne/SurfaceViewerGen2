#pragma once

//namespace data_standart
//{
//   struct iDataStandartIntefaceBase
//   {
//      virtual void ReadFromSource() = 0;
//      virtual void WriteToDestination() = 0;
//   };
//}
//
//#pragma once
//
//#include "DataStandart.h"
//
//#ifdef DATASTANDART_EXPORTS
//#define DS_EXPORTIMPORT __declspec(dllexport) // export DLL information
//#else
//#define DS_EXPORTIMPORT __declspec(dllimport) // import DLL information
//#endif
//
//#define PREPARE_DS(name) extern DS_EXPORTIMPORT navigation_dispatcher::iOrderPtr name(central_pack* pack, iComService* pService);
//
//namespace navigation_dispatcher
//{
//   PREPARE_ORDER(CreatePngHeighmapConverter);
//   PREPARE_ORDER(CreateObjectListGenerator);
//   PREPARE_ORDER(CreatePathfinder);
//   PREPARE_ORDER(CreateOptimizedPathfinder);
//   PREPARE_ORDER(CreatePackHound);
//}