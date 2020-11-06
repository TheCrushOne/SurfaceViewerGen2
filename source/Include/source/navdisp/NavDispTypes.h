#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <windows.h>

namespace SV::navigation_dispatcher
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

   inline LPCSTR convert_order_type(OrderType type)
   {
      switch (type)
      {
         DECLARE_TYPE(OT_PNGHMCONVERT, "PngHMConvert")
         DECLARE_TYPE(OT_GENOBJLIST, "GenObjList")
         DECLARE_TYPE(OT_PATHFIND, "PathFind")
         DECLARE_TYPE(OT_OPTPATH, "OptPathFind")
         DECLARE_TYPE(OT_PACKHOUND, "PackHound")
      }

      _ASSERT(!"Unknown command type");
      return "";
   }

#undef DECLARE_TYPE

   inline OrderType convert_command_name(LPCSTR name)
   {
      for (int n = OT_UNKNOWN + 1; n != OT_MAX_VALUE; ++n)
      {
         OrderType type = static_cast<OrderType>(n);
         if (strcmp(name, convert_order_type(type)) == 0)
            return type;
      }

      _ASSERT(!"Unknown command name");
      return OT_UNKNOWN;
   }
}
