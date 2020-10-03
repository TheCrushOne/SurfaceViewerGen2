#include "stdafx.h"
#include "JsonUnitDataSerializer.h"
#include "common\header_collector.h"
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

#include "json/json_wrapper.h"

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
         Json::Value j;
         i >> j;
         jsonToUnitData(j, data);

         return true;
      }

      static const char* toString(const settings::unit_source_data& data)
      {
         static std::string staticBuffer;
         Json::Value j;
         unitDataToJson(j, data);
         staticBuffer = j.asString();
         return staticBuffer.c_str();
      }

      static bool fromString(const char* src, settings::unit_source_data& data)
      {
         std::stringstream i(src, std::ios_base::in | std::ios::binary);
         Json::Value j;
         i >> j;
         jsonToUnitData(j, data);

         return true;
      }

      static bool jsonToUnitData(const Json::Value& j, settings::unit_source_data& data)
      {
         auto readPSEVector = [](const Json::Value& elem, settings::unit_data_element& pse)->bool
         {
            pse.name = elem[tag::name].asString();
            pse.start.row = elem[tag::start][tag::row].asUInt();
            pse.start.col = elem[tag::start][tag::col].asUInt();
            pse.finish.row = elem[tag::finish][tag::row].asUInt();
            pse.finish.col = elem[tag::finish][tag::col].asUInt();
            if (elem.find(tag::control_points, tag::control_points+strlen(tag::control_points)) != nullptr)
            {
               for (auto& cp : elem[tag::control_points])
                  pse.control_point_list.emplace_back(SVCG::route_point(cp[tag::row].asUInt(), cp[tag::col].asUInt(), 0.f));
            }
            return true;
         };
         for (auto& elem : j[tag::land_units])
         {
            settings::unit_data_element pse;
            readPSEVector(elem, pse);
            data.land_units.emplace_back(pse);
         }

         for (auto& elem : j[tag::air_units])
         {
            settings::unit_data_element pse;
            readPSEVector(elem, pse);
            data.air_units.emplace_back(pse);
         }
         return true;
      }

      static bool unitDataToJson(Json::Value& j, const settings::unit_source_data& data)
      {
         auto writePSEVector = [](Json::Value& elem, const settings::unit_data_element& pse)->bool
         {
            elem[tag::name] = pse.name;
            elem[tag::start][tag::row] = pse.start.row;
            elem[tag::start][tag::col] = pse.start.col;
            elem[tag::finish][tag::row] = pse.finish.row;
            elem[tag::finish][tag::col] = pse.finish.col;
            elem[tag::control_points].resize(pse.control_point_list.size());
            for (size_t idx = 0; idx < pse.control_point_list.size(); idx++)
            {
               auto& cp = pse.control_point_list.at(idx);
               Json::Value o;
               o[tag::row] = cp.row;
               o[tag::col] = cp.col;
               elem[tag::control_points][idx] = o;
            }
            return true;
         };
         for (auto& elem : data.land_units)
         {
            Json::Value j_temp;
            writePSEVector(j_temp, elem);
            j[tag::land_units].emplace_back(j_temp);
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