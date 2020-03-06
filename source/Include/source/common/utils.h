#pragma once

#include <string>
#include <direct.h>

namespace SVGUtils
{
   inline std::string CurrentCurrentPath()
   {
      char cCurrentPath[FILENAME_MAX];
      _getcwd(cCurrentPath, sizeof(cCurrentPath));
      return std::string{cCurrentPath};
   }

   inline std::string CurrentDllPath(const char* dllFileName)
   {
      char cCurrentPath[FILENAME_MAX];
      _getcwd(cCurrentPath, sizeof(cCurrentPath));
#ifdef _WIN64
      const std::string dllPath = std::string(cCurrentPath) + "\\" + dllFileName + "_64.dll";
#else
      const std::string dllPath = std::string(cCurrentPath) + "\\" + dllFileName + ".dll";
#endif
      return dllPath;
   }

   inline std::wstring stringToWstring(std::string string)
   {
      std::wstring wstr(string.begin(), string.end());
      return wstr;
   }

   inline std::string wstringToString(std::wstring string)
   {
      std::string str(string.begin(), string.end());
      return str;
   }
}