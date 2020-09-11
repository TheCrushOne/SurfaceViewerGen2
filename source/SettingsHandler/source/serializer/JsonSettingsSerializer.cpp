#include "stdafx.h"
#include "JsonSettingsSerializer.h"
#include "common/header_collector.h"
#include <nlohmann/json.hpp>
#include <fstream>

using namespace nlohmann;

namespace colreg
{
   class JsonSettingsSerializer : public iJsonSettingsSerializer
   {
   public:
      void Release() override { delete this; }

      bool Serialize(const char* filename, const settings::pathfinding_settings& stt) const final { return serialize(filename, stt); }
      bool Deserialize(const char* filename, settings::pathfinding_settings& stt) const final { return deserialize(filename, stt); }
      bool Serialize(const char* filename, const settings::research_settings& stt) const final { return serialize(filename, stt); }
      bool Deserialize(const char* filename, settings::research_settings& stt) const final { return deserialize(filename, stt); }
      bool Serialize(const char* filename, const settings::environment_settings& stt) const final { return serialize(filename, stt); }
      bool Deserialize(const char* filename, settings::environment_settings& stt) const final { return deserialize(filename, stt); }
      bool Serialize(const char* filename, const settings::simulation_settings& stt) const final { return serialize(filename, stt); }
      bool Deserialize(const char* filename, settings::simulation_settings& stt) const final { return deserialize(filename, stt); }
      bool Serialize(const char* filename, const settings::map_settings& stt) const final { return serialize(filename, stt); }
      bool Deserialize(const char* filename, settings::map_settings& stt) const final { return deserialize(filename, stt); }

      const char* ToString(const settings::pathfinding_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::pathfinding_settings& stt) const final { return fromString(json, stt); }
      const char* ToString(const settings::research_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::research_settings& stt) const final { return fromString(json, stt); }
      const char* ToString(const settings::environment_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::environment_settings& stt) const final { return fromString(json, stt); }
      const char* ToString(const settings::simulation_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::simulation_settings& stt) const final { return fromString(json, stt); }
      const char* ToString(const settings::map_settings& stt) const final { return toString(stt); }
      bool FromString(const char* json, settings::map_settings& stt) const final { return fromString(json, stt); }

      template<class settings>
      static bool serialize(const char* filename, const settings& data)
      {
         std::ofstream o(filename);
         json j;
         settingsToJson(j, data);
         o << j;

         return true;
      }

      template<class settings>
      static bool deserialize(const char* filename, settings& data)
      {
         std::ifstream i(filename, std::ios_base::in | std::ios::binary);
         if (!i.is_open())
            return false;
         json j;
         i >> j;
         jsonToSettings(j, data);

         return true;
      }

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
         j["values"] = range.values;
         return j;
      }

      template<>
      static bool settingsToJson(json& j, const settings::pathfinding_settings& data)
      {
         json ls;
         ls["dangerous_land_angle"] = data.lvl_stt.dangerous_land_angle;
         ls["max_air_height"] = data.lvl_stt.max_air_height;
         ls["max_land_angle"] = data.lvl_stt.max_land_angle;
         ls["max_land_height"] = data.lvl_stt.max_land_height;
         ls["min_land_height"] = data.lvl_stt.min_land_height;
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
         j["gcs_info"] = infoWriter(data.gcs_info);
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
         if (j.find("values") != j.end())
            range.values = j["values"].get<std::vector<T>>();

         range.apply();
         return range;
      }

      template<>
      static bool jsonToSettings(const json& j, settings::pathfinding_settings& data)
      {
         const json& ls = j["level_settings"];
         data.lvl_stt.dangerous_land_angle = ls["dangerous_land_angle"].get<double>();
         data.lvl_stt.max_air_height = ls["max_air_height"].get<double>();
         data.lvl_stt.max_land_angle = ls["max_land_angle"].get<double>();
         data.lvl_stt.max_land_height = ls["max_land_height"].get<double>();
         data.lvl_stt.min_land_height = ls["min_land_height"].get<double>();
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
         data.gcs_info = infoReader(j["gcs_info"]);
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

colreg::iSettingsSerializer* CreateJsonSettingsSerializer()
{
   return new colreg::JsonSettingsSerializer();
}