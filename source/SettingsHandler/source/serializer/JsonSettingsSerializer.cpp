#include "stdafx.h"
#include "JsonSettingsSerializer.h"
#include "common/header_collector.h"
#include <fstream>

#include "json/json_wrapper.h"

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
      static bool settingsToJson(Json::Value& j, const settings& data)
      {
         return true;
      }

      template<typename T>
      static Json::Value rangeWriter(T range)
      {
         Json::Value j;
         j[tag::max] = range.max;
         j[tag::min] = range.min;
         j[tag::step] = range.step;
         j[tag::values] = range.values;
         return j;
      }

      template<>
      static bool settingsToJson(Json::Value& j, const settings::pathfinding_settings& data)
      {
         Json::Value ls;
         ls[tag::dangerous_land_angle] = data.lvl_stt.dangerous_land_angle;
         ls[tag::max_air_height] = data.lvl_stt.max_air_height;
         ls[tag::max_land_angle] = data.lvl_stt.max_land_angle;
         ls[tag::max_land_height] = data.lvl_stt.max_land_height;
         ls[tag::min_land_height] = data.lvl_stt.min_land_height;
         j[tag::level_settings] = ls;
         return true;
      }

      template<>
      static bool settingsToJson(Json::Value& j, const settings::research_settings& data)
      {
         j[tag::debug_level] = data.debug_level;
         j[tag::fly_count_range] = rangeWriter(data.fly_count_range);
         j[tag::iter_count] = data.iter_count;
         j[tag::length_range] = rangeWriter(data.length_range);
         j[tag::map_size] = data.map_size;
         j[tag::multi_thread_test] = data.multi_thread_test;
         j[tag::res_type] = static_cast<unsigned short>(data.res_type);
         j[tag::single_thread_test] = data.single_thread_test;
         j[tag::task_pool_range] = rangeWriter(data.task_pool_range);
         j[tag::thread_pool_range] = rangeWriter(data.thread_pool_range);

         return true;
      }

      template<>
      static bool settingsToJson(Json::Value& j, const settings::environment_settings& data)
      {
         auto infoWriter = [](const settings::coordinate_system_info& info)->Json::Value
         {
            Json::Value ij;
            ij[tag::angle] = info.angle;
            ij[tag::scale] = info.scale;
            ij[tag::ordinate_bias] = info.ordinate_bias;
            ij[tag::abscissa_bias] = info.abscissa_bias;
            return ij;
         };
         j[tag::gcs_info] = infoWriter(data.gcs_info);
         j[tag::mtx_info] = infoWriter(data.mtx_info);
         return true;
      }

      template<>
      static bool settingsToJson(Json::Value& j, const settings::simulation_settings& data)
      {
         return true;
      }

      template<>
      static bool settingsToJson(Json::Value& j, const settings::map_settings& data)
      {
         j["col_count"] = data.col_count;
         j["row_count"] = data.row_count;
         return true;
      }

      template<class settings>
      static bool jsonToSettings(const Json::Value& j, settings& data)
      {
         return true;
      }

      template<typename T>
      static settings::range_data<T> rangeReader(const Json::Value& j)
      {
         settings::range_data<T> range;
         range.max = j[tag::max].get<T>();
         range.min = j[tag::min].get<T>();
         range.step = j[tag::step].get<T>();
         if (j.find(tag::values) != j.end())
            range.values = j[tag::values].get<std::vector<T>>();

         range.apply();
         return range;
      }

      template<>
      static bool jsonToSettings(const Json::Value& j, settings::pathfinding_settings& data)
      {
         const Json::Value& ls = j[tag::level_settings];
         data.lvl_stt.dangerous_land_angle = ls[tag::dangerous_land_angle].asDouble();
         data.lvl_stt.max_air_height = ls[tag::max_air_height].asDouble();
         data.lvl_stt.max_land_angle = ls[tag::max_land_angle].asDouble();
         data.lvl_stt.max_land_height = ls[tag::max_land_height].asDouble();
         data.lvl_stt.min_land_height = ls[tag::min_land_height].asDouble();
         return true;
      }

      template<>
      static bool jsonToSettings(const Json::Value& j, settings::research_settings& data)
      {
         data.debug_level = j["debug_level"].asInt();
         data.fly_count_range = rangeReader<size_t>(j["fly_count_range"]);
         data.iter_count = j["iter_count"].asInt();
         data.length_range = rangeReader<double>(j["length_range"]);
         data.map_size = j["map_size"].asInt();
         data.multi_thread_test = j["multi_thread_test"].asBool();
         data.res_type = static_cast<settings::ResearchType>(j["res_type"].asInt());
         data.single_thread_test = j["single_thread_test"].asBool();
         data.task_pool_range = rangeReader<size_t>(j["task_pool_range"]);
         data.thread_pool_range = rangeReader<size_t>(j["thread_pool_range"]);
         return true;
      }

      template<>
      static bool jsonToSettings(const Json::Value& j, settings::environment_settings& data)
      {
         auto infoReader = [](const Json::Value& j)->settings::coordinate_system_info
         {
            settings::coordinate_system_info info;
            info.angle = j["angle"].asDouble();
            info.scale = j["scale"].asDouble();
            info.ordinate_bias = j["ordinate_bias"].asDouble();
            info.abscissa_bias = j["abscissa_bias"].asDouble();
            return info;
         };
         data.gcs_info = infoReader(j["gcs_info"]);
         data.mtx_info = infoReader(j["mtx_info"]);
         return true;
      }

      template<>
      static bool jsonToSettings(const Json::Value& j, settings::simulation_settings& data)
      {
         return true;
      }

      template<>
      static bool jsonToSettings(const Json::Value& j, settings::map_settings& data)
      {
         data.col_count = j[tag::col_count].asInt();
         data.row_count = j[tag::row_count].asInt();
         return true;
      }
   };
}

colreg::iSettingsSerializer* CreateJsonSettingsSerializer()
{
   return new colreg::JsonSettingsSerializer();
}