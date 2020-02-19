#pragma once

#include <string>

namespace SVGUtils
{
   struct FileStorage
   {
      std::string pathfinderSettingsPath;
      std::string researchSettingsPath;
      std::string mapPath;
      std::string unitDataPath;

      FileStorage(std::wstring metaFileName)
         : pathfinderSettingsPath{ metaFileName.begin(), metaFileName.end() }
         , researchSettingsPath{ metaFileName.begin(), metaFileName.end() }
         , mapPath{ metaFileName.begin(), metaFileName.end() }
         , unitDataPath{ metaFileName.begin(), metaFileName.end() }
      {
         pathfinderSettingsPath.replace(pathfinderSettingsPath.find(".meta"), sizeof(".meta") - 1, ".psxml");
         researchSettingsPath.replace(researchSettingsPath.find(".meta"), sizeof(".meta") - 1, ".rsxml");
         mapPath.replace(mapPath.find(".meta"), sizeof(".meta") - 1, ".png");
         unitDataPath.replace(unitDataPath.find(".meta"), sizeof(".meta") - 1, ".udp");
      }
   };
}