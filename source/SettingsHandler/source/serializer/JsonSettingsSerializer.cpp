#include "stdafx.h"
#include "JsonSettingsSerializer.h"
#include "common/settings.h"
#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace colreg
{
   class JsonSettingsSerializer : public iJsonSettingsSerializer
   {
   public:
      bool Serialize(const char*, const settings::pathfinding_settings&) const final { return true; }
      bool Deserialize(const char*, settings::pathfinding_settings&) const final { return true; }
      bool Serialize(const char*, const settings::research_settings&) const final { return true; }
      bool Deserialize(const char*, settings::research_settings&) const final { return true; }
      bool Serialize(const char*, const settings::environment_settings&) const final { return true; }
      bool Deserialize(const char*, settings::environment_settings&) const final { return true; }
      bool Serialize(const char*, const settings::simulation_settings&) const final { return true; }
      bool Deserialize(const char*, settings::simulation_settings&) const final { return true; }
      bool Serialize(const char*, const settings::map_settings&) const final { return true; }
      bool Deserialize(const char*, settings::map_settings&) const final { return true; }

      const char* ToString(const settings::pathfinding_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::pathfinding_settings& stt) const final { fromString(json, stt); }
      const char* ToString(const settings::research_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::research_settings& stt) const final { fromString(json, stt); }
      const char* ToString(const settings::environment_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::environment_settings& stt) const final { fromString(json, stt); }
      const char* ToString(const settings::simulation_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::simulation_settings& stt) const final { fromString(json, stt); }
      const char* ToString(const settings::map_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::map_settings& stt) const final { fromString(json, stt); }

      template<class settings>
      static const char* toString(const settings& data)
      {
         static std::string staticBuffer;
         json j;
         settingsToJson(j, data);
         staticBuffer = j.dump();
         return staticBuffer.c_str();
      }

      template<class settings>
      static bool fromString(const char* src, settings& data)
      {
         std::stringstream i(src, std::ios_base::in | std::ios::binary);
         json j;
         i >> j;
         jsonToSettings(j, data);

         return true;
      }

      template<class settings>
      static bool settingsToJson(json& j, const settings& data)
      {
         return true;
      }

      template<typename T>
      static json rangeWriter(T range)
      {
         json j;
         j["max"] = range.max;
         j["min"] = range.min;
         j["step"] = range.step;
         for (auto& elem : range.values)
         {
            json val;
            val["value"] = elem;
            j["values"].emplace_back(val);
         }
      }

      template<>
      static bool settingsToJson(json& j, const settings::pathfinding_settings& data)
      {
         json ls;
         ls["dangerous_land_angle"] = data.level_settings.dangerous_land_angle;
         ls["max_air_height"] = data.level_settings.max_air_height;
         ls["max_land_angle"] = data.level_settings.max_land_angle;
         ls["max_land_height"] = data.level_settings.max_land_height;
         ls["min_land_height"] = data.level_settings.min_land_height;
         j["level_settings"] = ls;
         return true;
      }

      template<>
      static bool settingsToJson(json& j, const settings::research_settings& data)
      {
         j["debug_level"] = data.debug_level;
         j["fly_count_range"] = rangeWriter(data.fly_count_range);
         j["iter_count"] = data.iter_count;
         j["length_range"] = rangeWriter(data.length_range);
         j["map_size"] = data.map_size;
         j["multi_thread_test"] = data.multi_thread_test;
         j["res_type"] = data.res_type;
         j["single_thread_test"] = data.single_thread_test;
         j["task_pool_range"] = rangeWriter(data.task_pool_range);
         j["thread_pool_range"] = rangeWriter(data.thread_pool_range);

         return true;
      }

      template<>
      static bool settingsToJson(json& j, const settings::environment_settings& data)
      {
         auto infoWriter = [](const settings::coordinate_system_info& info)->json
         {
            json ij;
            ij["angle"] = info.angle;
            ij["scale"] = info.scale;
            ij["ordinate_bias"] = info.ordinate_bias;
            ij["abscissa_bias"] = info.abscissa_bias;
            return ij;
         };
         j["gsc_info"] = infoWriter(data.gcs_info);
         j["mtx_info"] = infoWriter(data.mtx_info);
         return true;
      }

      template<>
      static bool settingsToJson(json& j, const settings::simulation_settings& data)
      {
         return true;
      }

      template<>
      static bool settingsToJson(json& j, const settings::map_settings& data)
      {
         j["col_count"] = data.col_count;
         j["row_count"] = data.row_count;
         return true;
      }

      template<class settings>
      static bool jsonToSettings(const json& j, settings& data)
      {
         return true;
      }

      template<typename T>
      static settings::range_data<T> rangeReader(const json& j)
      {
         settings::range_data<T> range;
         range.max = j["max"].get<T>();
         range.min = j["min"].get<T>();
         range.step = j["step"].get<T>();
         for (auto& elem : j["values"])
            range.values.emplace_back(elem["value"].get<T>());
         range.apply();
      }

      template<>
      static bool jsonToSettings(const json& j, settings::pathfinding_settings& data)
      {
         const json& ls = j["level_settings"];
         data.level_settings.dangerous_land_angle = ls["dangerous_land_angle"].get<double>();
         data.level_settings.max_air_height = ls["max_air_height"].get<double>();
         data.level_settings.max_land_angle = ls["max_land_angle"].get<double>();
         data.level_settings.max_land_height = ls["max_land_height"].get<double>();
         data.level_settings.min_land_height = ls["min_land_height"].get<double>();
         return true;
      }

      template<>
      static bool jsonToSettings(const json& j, settings::research_settings& data)
      {
         data.debug_level = j["debug_level"].get<size_t>();
         data.fly_count_range = rangeReader<size_t>(j["fly_count_range"]);
         data.iter_count = j["iter_count"].get<size_t>();
         data.length_range = rangeReader<double>(j["length_range"]);
         data.map_size = j["map_size"].get<size_t>();
         data.multi_thread_test = j["multi_thread_test"].get<bool>();
         data.res_type = j["res_type"].get<settings::ResearchType>();
         data.single_thread_test = j["single_thread_test"].get<bool>();
         data.task_pool_range = rangeReader<size_t>(j["task_pool_range"]);
         data.thread_pool_range = rangeReader<size_t>(j["thread_pool_range"]);
         return true;
      }

      template<>
      static bool jsonToSettings(const json& j, settings::environment_settings& data)
      {
         auto infoReader = [](const json& j)->settings::coordinate_system_info
         {
            settings::coordinate_system_info info;
            info.angle = j["angle"].get<double>();
            info.scale = j["scale"].get<double>();
            info.ordinate_bias = j["ordinate_bias"].get<double>();
            info.abscissa_bias = j["abscissa_bias"].get<double>();
            return info;
         };
         data.gcs_info = infoReader(j["gsc_info"]);
         data.mtx_info = infoReader(j["mtx_info"]);
         return true;
      }

      template<>
      static bool jsonToSettings(const json& j, settings::simulation_settings& data)
      {
         return true;
      }

      template<>
      static bool jsonToSettings(const json& j, settings::map_settings& data)
      {
         data.col_count = j["col_count"].get<size_t>();
         data.row_count = j["row_count"].get<size_t>();
         return true;
      }
   };
}