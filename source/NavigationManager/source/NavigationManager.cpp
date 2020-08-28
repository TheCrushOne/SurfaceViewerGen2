// NavigationManager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <atlbase.h>
#include <iostream>
#include <string>
#include <comutil.h>
#include "colreg/ModuleGuard.h"
#include "crossdllinterface/NavigationDispatcherInterface.h"
#include "ConsoleCommunicator\ConsoleCommunicator.h"
#include "common/utils.h"

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard, ...) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName, __VA_ARGS__); \
   if (!guard.IsValid()) \
   { \
      comm->RaiseError(); \
      std::string errMsg = std::string("Can't load '") + dllName + "' from '" + SVGUtils::CurrentDllPath(dllName).c_str() + "'!"; \
      comm->Message(ICommunicator::MessageType::MT_ERROR, errMsg.c_str()); \
      return false; \
   }

#define MAX_LEN 1000

int print_help()
{
   _tprintf(_T("Usage: NavigationManager.exe [base folder] [config].xml [begin order name]\n"));
   std::getchar();
   return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
   if (argc < 2 /*&& argc != 3 || !fpath(argv[1]).is_exists()*/)
      return print_help();
   std::vector<std::string> argVct;
   for (size_t idx = 0; idx < argc; idx++)
   {
      char converted[MAX_LEN];
      wcstombs(converted, argv[idx], wcslen(argv[idx]) + 1);
      argVct.emplace_back(std::string(converted));
   }
   std::shared_ptr<ICommunicator> comm = std::make_shared<ConsoleCommunicator>();
   std::shared_ptr<central_pack> pack = std::make_shared<central_pack>(comm);
   colreg::ModuleGuard<navigation_dispatcher::iNavigationDispatcher, central_pack*> m_navigationDispatcher;
   VALID_CHECK_DLL_LOAD("NavigationDispatcher", "CreateNavigationDispatcher", m_navigationDispatcher, pack.get());

   if (m_navigationDispatcher->ProcessCommand(argVct.at(2).c_str(), argc == 4 ? argVct.at(3).c_str() : NULL, argVct.at(1).c_str()) == 1)
   {
      _tprintf(_T("Process command success. Press any key...\n"));
      return 0;
   }
   else
   {
      _tprintf(_T("Process command error. Press enter...\n"));
      return 1;
   }
}