#pragma once

//#include "SettingsSerializer.h"
//#include "ColregSimulation.h"
#include "common/settings.h"
#include "colreg/BaseRef.h"
#include "crossdllinterface\SettingsSerializerInterface.h"

namespace xml_properties
{
   class PropertyItem;
}

struct iPropertyInterface;

namespace colreg
{
   struct iXMLSettingsSerializer : iSettingsSerializer
   {
      virtual bool Serialize(const char* filename, const settings::pathfinding_settings& s, const settings::pathfinding_settings& ds) const = 0;
      virtual bool Deserialize(const char* filename, settings::pathfinding_settings& s, bool mult) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const settings::pathfinding_settings& s, const settings::pathfinding_settings& ds) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem& root, settings::pathfinding_settings& s) const = 0;

      virtual const char* ToString(const settings::pathfinding_settings& srcStt, const settings::pathfinding_settings& defSrcStt) const = 0;

      virtual bool Serialize(const char* filename, const settings::research_settings& s, const settings::research_settings& ds) const = 0;
      virtual bool Deserialize(const char* filename, settings::research_settings& s, bool mult) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const settings::research_settings& s, const settings::research_settings& ds) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem& root, settings::research_settings& s) const = 0;

      virtual const char* ToString(const settings::research_settings& srcStt, const settings::research_settings& defSrcStt) const = 0;

      virtual void ReleaseTree(xml_properties::PropertyItem*) const = 0;
   };
}