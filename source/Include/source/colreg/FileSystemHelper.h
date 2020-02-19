#pragma once
#include <vector>
#include <string>
#include "ThirdParty\dirent.h"
#include "TTCG\Common\FileSystem\Path.h"

class FileSystemHelper
{
public:
   static std::vector< std::string > EnumFiles(const std::string& folder, bool returnFullPath)
   {
      return EnumContent(folder, true, returnFullPath);
   }

   static std::vector< std::string > EnumSubfolders(const std::string& folder, bool returnFullPath)
   {
      return EnumContent(folder, false, returnFullPath);
   }

private:
   static std::vector< std::string > EnumContent(const std::string& folder, bool enumFiles, bool fullPath)
   {
      std::vector< std::string > content;
      content.reserve(32);

      DIR* dirp = opendir(folder.c_str());
      if (!dirp)
      {
         return content;
      }

      dirent * dp = nullptr;
      while ((dp = readdir(dirp)) != NULL)
      {
         if ((dp->d_type&DT_DIR) && ( strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0 ) )
         {
            continue;
         }
         if ( !( enumFiles ^ ( (dp->d_type&DT_DIR)!= 0) ) )
         {
            continue;
         }
         
         content.emplace_back(fullPath ? fpathA(folder).append(dp->d_name).get() : dp->d_name );
      }
      closedir(dirp);
      return content;
   }
};