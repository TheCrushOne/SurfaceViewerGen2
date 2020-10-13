#pragma once

#include <string>
#include <direct.h>
//#include <atldef.h>
//#include <comutil.h>

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
      //const std::string dllPath = std::string(cCurrentPath) + "\\" + dllFileName + "_64.dll";
      const std::string dllPath = std::string(cCurrentPath) + "\\" + dllFileName + ".dll";
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

   //inline std::wstring bstrToWstring(_bstr_t string)
   //{
   //   std::wstring str(string, SysStringLen(string));
   //   return str;
   //}

   //inline _bstr_t wstringToBstr(std::wstring string)
   //{
   //   _ASSERT(!string.empty());
   //   BSTR bs = SysAllocStringLen(string.data(), string.size());
   //   return bs;
   //}

   //inline std::string bstrToString(_bstr_t string)
   //{
   //   std::wstring wstr(string, SysStringLen(string));
   //   std::string str(wstr.begin(), wstr.end());
   //   return str;
   //}

   //inline _bstr_t stringToBstr(std::string string)
   //{
   //   std::wstring wstr(string.begin(), string.end());
   //   _ASSERT(!string.empty());
   //   BSTR bs = SysAllocStringLen(wstr.data(), wstr.size());
   //   return bs;
   //}
}