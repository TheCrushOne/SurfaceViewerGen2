#pragma once

//#include "SettingsSerializer.h"
//#include "ColregSimulation.h"
#include "common/header_collector.h"
#include "colreg/BaseRef.h"
#include "crossdllinterface\SettingsSerializerInterface.h"

namespace SV::xml_properties
{
   class PropertyItem;
}

struct iPropertyInterface;

namespace SV::serializer
{
   struct iXMLSettingsSerializer : iSettingsSerializer
   {
      virtual bool Serialize(const char*, const settings::pathfinding_settings&, const settings::pathfinding_settings&) const = 0;
      virtual bool Deserialize(const char*, settings::pathfinding_settings&, bool) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const settings::pathfinding_settings&, const settings::pathfinding_settings&) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem&, settings::pathfinding_settings&) const = 0;

      virtual const char* ToString(const settings::pathfinding_settings&, const settings::pathfinding_settings&) const = 0;

      virtual bool Serialize(const char*, const settings::research_settings&, const settings::research_settings&) const = 0;
      virtual bool Deserialize(const char*, settings::research_settings&, bool) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const settings::research_settings&, const settings::research_settings&) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem&, settings::research_settings&) const = 0;

      virtual const char* ToString(const settings::research_settings&, const settings::research_settings&) const = 0;

      virtual void ReleaseTree(xml_properties::PropertyItem*) const = 0;
   };
}