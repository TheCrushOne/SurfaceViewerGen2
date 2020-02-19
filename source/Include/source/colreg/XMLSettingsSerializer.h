#pragma once

#include "SettingsSerializer.h"
#include "ColregSimulation.h"
namespace xml_properties
{
   class PropertyItem;
}

struct iPropertyInterface;

namespace colreg
{
   struct iXMLSettingsSerializer : iSettingsSerializer
   {
      virtual bool Serialize(const char* filename, const settings& s, const settings& ds) const = 0;
      virtual bool Deserialize(const char* filename, settings& s, bool mult) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const settings& s, const settings& ds) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem& root, settings& s) const = 0;

      virtual const char* ToString(const settings& srcStt, const settings& defSrcStt) const = 0;

      virtual bool Serialize(const char* filename, const environment& s, const environment& ds) const = 0;
      virtual bool Deserialize(const char* filename, environment& s, bool mult) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const environment& s, const environment& ds) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem& root, environment& s) const = 0;

      virtual const char* ToString(const environment& srcStt, const environment& defSrcStt) const = 0;

      virtual bool Serialize(const char* filename, const ColregSimulation::simulation_settings& s) const = 0;
      virtual bool Deserialize(const char* filename, ColregSimulation::simulation_settings& s) const = 0;
      virtual bool Serialize(const char* filename, const ColregSimulation::simulation_settings& s, const ColregSimulation::simulation_settings& ds) const = 0;
      virtual bool Deserialize(const char* filename, ColregSimulation::simulation_settings& s, bool mult) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const ColregSimulation::simulation_settings& s, const ColregSimulation::simulation_settings& ds) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem& root, ColregSimulation::simulation_settings& s) const = 0;

      virtual const char* ToString(const ColregSimulation::simulation_settings& srcStt) const = 0;
      virtual const char* ToString(const ColregSimulation::simulation_settings& srcStt, const ColregSimulation::simulation_settings& defSrcStt) const = 0;
      virtual bool FromString(const char* src, ColregSimulation::simulation_settings& dstStt) const = 0;

      virtual bool Serialize(const char* filename, const colreg::weather_influence_info& s) const = 0;
      virtual bool Deserialize(const char* filename, colreg::weather_influence_info& s) const = 0;
      virtual bool Serialize(const char* filename, const colreg::weather_influence_info& s, const colreg::weather_influence_info& ds) const = 0;
      virtual bool Deserialize(const char* filename, colreg::weather_influence_info& s, bool mult) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const colreg::weather_influence_info& s, const colreg::weather_influence_info& ds) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem& root, colreg::weather_influence_info& s) const = 0;

      virtual const char* ToString(const colreg::weather_influence_info& srcStt) const = 0;
      virtual const char* ToString(const colreg::weather_influence_info& srcStt, const colreg::weather_influence_info& defSrcStt) const = 0;
      virtual bool FromString(const char* src, colreg::weather_influence_info& dstStt) const = 0;

      virtual bool Serialize(const char* filename, const iPropertyInterface* s) const = 0;
      virtual bool Deserialize(const char* filename, iPropertyInterface* s) const = 0;

      virtual xml_properties::PropertyItem* DirectSerialize(const iPropertyInterface* s) const = 0;
      virtual void DirectDeserialize(const xml_properties::PropertyItem& root, iPropertyInterface* s) const = 0;

      virtual const char* ToString(const iPropertyInterface* srcStt) const = 0;
      virtual bool FromString(const char* src, iPropertyInterface* dstStt) const = 0;

      virtual void ReleaseTree(xml_properties::PropertyItem*) const = 0;
   };
}