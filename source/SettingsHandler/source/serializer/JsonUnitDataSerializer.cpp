#include "stdafx.h"
#include "JsonUnitDataSerializer.h"
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
      bool Serialize(const char* filename, const settings::unit_source_data& s) const final { return serialize(filename, s); }
      bool Deserialize(const char* filename, settings::unit_source_data& s) const final { return deserialize(filename, s); }

      const char* ToString(const settings::unit_source_data& srcStt) const final { return toString(srcStt); }
      bool FromString(const char* src, settings::unit_source_data& dstStt) const final { return fromString(src, dstStt); }
   private:
      static bool serialize(const char* filename, const settings::unit_source_data& data)
      {
         std::ofstream o(filename, std::ios_base::in | std::ios::binary);
         //o << std::setw(4) << j << std::endl;

         return true;
      }

      static bool deserialize(const char* filename, settings::unit_source_data& data)
      {
         std::ifstream i(filename, std::ios_base::in | std::ios::binary);
         if (!i.is_open())
            return false;
         json j;
         i >> j;
         jsonToUnitData(j, data);

         return true;
      }

      static const char* toString(const settings::unit_source_data& data)
      {
         static std::string staticBuffer;
         json j;
         unitDataToJson(j, data);
         staticBuffer = j.dump();
         return staticBuffer.c_str();
      }

      static bool fromString(const char* src, settings::unit_source_data& data)
      {
         std::stringstream i(src, std::ios_base::in | std::ios::binary);
         json j;
         i >> j;
         jsonToUnitData(j, data);

         return true;
      }

      static bool jsonToUnitData(const json& j, settings::unit_source_data& data)
      {
         auto readPSEVector = [](const json& elem, settings::unit_data_element& pse)->bool
         {
            pse.name = elem["name"].get<std::string>();
            pse.start.row = elem["start"]["row"].get<size_t>();
            pse.start.col = elem["start"]["col"].get<size_t>();
            pse.finish.row = elem["finish"]["row"].get<size_t>();
            pse.finish.col = elem["finish"]["col"].get<size_t>();
            if (elem.find("control_points") != elem.end())
            {
               for (auto& cp : elem["control_points"])
                  pse.control_point_list.emplace_back(SVCG::route_point(cp["row"].get<size_t>(), cp["col"].get<size_t>(), 0.f));
            }
            return true;
         };
         for (auto& elem : j["land_units"])
         {
            settings::unit_data_element pse;
            readPSEVector(elem, pse);
            data.land_units.emplace_back(pse);
         }

         for (auto& elem : j["air_units"])
         {
            settings::unit_data_element pse;
            readPSEVector(elem, pse);
            data.air_units.emplace_back(pse);
         }
         return true;
      }

      static bool unitDataToJson(json& j, const settings::unit_source_data& data)
      {
         auto writePSEVector = [](json& elem, const settings::unit_data_element& pse)->bool
         {
            elem["name"] = pse.name;
            elem["start"]["row"] = pse.start.row;
            elem["start"]["col"] = pse.start.col;
            elem["finish"]["row"] = pse.finish.row;
            elem["finish"]["col"] = pse.finish.col;
            for (auto& cp : pse.control_point_list)
            {
               json o;
               o["row"] = cp.row;
               o["col"] = cp.col;
               elem["control_points"].emplace_back(o);
            }
            return true;
         };
         for (auto& elem : data.land_units)
         {
            json j_temp;
            writePSEVector(j_temp, elem);
            j["land_units"].emplace_back(j_temp);
         }

         for (auto& elem : data.air_units)
         {
            json j_temp;
            writePSEVector(j_temp, elem);
            j["air_units"].emplace_back(j_temp);
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