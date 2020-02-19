#include "stdafx.h"
#include "JsonSerializer.h"
#include "common\settings.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace nlohmann;

namespace colreg
{
   class UnitDataSerializer : public iUnitDataSerializer
   {
   public:
      bool Serialize(const char* filename, const settings::unit_settings& s) const final { return serialize(filename, s); }
      bool Deserialize(const char* filename, settings::unit_settings& s) const final { return deserialize(filename, s); }

      const char* ToString(const settings::unit_settings& srcStt) const final { return toString(srcStt); }
      bool FromString(const char* src, settings::unit_settings& dstStt) const final { return fromString(src, dstStt); }
   private:
      static bool serialize(const char* filename, const settings::unit_settings& data)
      {
         //std::ofstream o("pretty.json");
         //o << std::setw(4) << j << std::endl;

         return true;
      }

      static bool deserialize(const char* filename, settings::unit_settings& data)
      {
         std::ifstream i(filename, std::ios_base::in | std::ios::binary);
         if (!i.is_open())
            return false;
         json j;
         i >> j;
         jsonToUnitData(j, data);

         return true;
      }

      static const char* toString(const settings::unit_settings& data)
      {
         return nullptr;
      }

      static bool fromString(const char* src, settings::unit_settings& data)
      {
         return true;
      }

      static bool jsonToUnitData(const json& j, settings::unit_settings& data)
      {
         auto readPSEVector = [](const json& elem, settings::point_setting_element& pse)->bool
         {
            pse.name = elem["name"].get<std::string>();
            pse.start.lat = elem["start"]["x"].get<size_t>();
            pse.start.lon = elem["start"]["y"].get<size_t>();
            pse.finish.lat = elem["finish"]["x"].get<size_t>();
            pse.finish.lon = elem["finish"]["y"].get<size_t>();
            for (auto& cp : elem["control_points"])
               pse.control_point_list.emplace_back(SVCG::route_point(cp["x"].get<size_t>(), cp["y"].get<size_t>(), 0.f));
            return true;
         };
         for (auto& elem : j["land_units"])
         {
            settings::point_setting_element pse;
            readPSEVector(elem, pse);
            data.land_units.emplace_back(pse);
         }

         for (auto& elem : j["air_units"])
         {
            settings::point_setting_element pse;
            readPSEVector(elem, pse);
            data.air_units.emplace_back(pse);
         }
         return true;
      }

      void Release() override { delete this; }

      mutable std::string _jsonString;
   };
}

colreg::iUnitDataSerializer* CreateUnitDataSerializer()
{
   return new colreg::UnitDataSerializer();
}