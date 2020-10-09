#pragma once

#include "Thirdparty/json/json.h"

#include <fstream>

namespace json_wrapper
{
   inline bool to_file(Json::Value& object, const wchar_t* filename)
   {
      Json::StreamWriterBuilder builder;
      builder.settings_["indentation"] = "  ";
      builder.settings_["dropNullPlaceholders"] = true;
      builder.settings_["precision"] = 10;
      const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

      std::wstring fullFileName(filename);
      try
      {
         std::ofstream fStr(fullFileName.c_str());
         writer->write(object, &fStr);
      }
      catch (...)
      {
         return false;
      }
      return true;
   }
}