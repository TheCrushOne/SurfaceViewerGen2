#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <windows.h>

namespace navigation_dispatcher
{
   enum OrderType
   {
      OT_UNKNOWN = 0,

      OT_PNGHMCONVERT,
      // Convert pictured height map to binary
      // Format: [.png]->[.hdat]
      OT_GENOBJLIST,
      // Generates object list from binary heightmap
      // Format: [.hdat]->[.obj]
      OT_PATHFIND,
      // Found simplest paths from binary heightmap
      // Format: [.hdat]->[.pl]
      OT_OPTPATH,
      // Found optimized paths from binary heightmap
      // Format: [.hdat]->[.opl]
      OT_PACKHOUND,
      // Creates thread optimization preconfig
      // Format: [null]->[.phc]

      OT_MAX_VALUE
   };

#define DECLARE_TYPE(type, name) case (type): return (name);

   inline LPCWSTR convert_order_type(OrderType type)
   {
      switch (type)
      {
         DECLARE_TYPE(OT_PNGHMCONVERT, L"PngHMConvert")
         DECLARE_TYPE(OT_GENOBJLIST, L"GenObjList")
         DECLARE_TYPE(OT_PATHFIND, L"PathFind")
         DECLARE_TYPE(OT_OPTPATH, L"OptPathFind")
         DECLARE_TYPE(OT_PACKHOUND, L"PackHound")
      }

      _ASSERT(!"Unknown command type");
      return L"";
   }

#undef DECLARE_TYPE

   inline OrderType convert_command_name(LPCWSTR name)
   {
      for (int n = OT_UNKNOWN + 1; n != OT_MAX_VALUE; ++n)
      {
         OrderType type = static_cast<OrderType>(n);
         if (wcscmp(name, convert_order_type(type)) == 0)
            return type;
      }

      _ASSERT(!"Unknown command name");
      return OT_UNKNOWN;
   }
}
