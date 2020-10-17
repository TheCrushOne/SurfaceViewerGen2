#pragma once

#include <vector>
#include <string>

namespace SV::properties
{
   struct simple_prop
   {
      simple_prop() = default;

      simple_prop(std::string k, std::string v)
         : key{ k }
         , val{ v }
      {}

      std::string key;
      std::string val;
   };

   typedef std::vector<simple_prop> simple_prop_vct;
}