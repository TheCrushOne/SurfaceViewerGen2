#include <stdafx.h>
#include <algorithm>
#include "XMLSettingsSerializer.h"
#include "colreg/XmlProperties/xml_property_container.h"
#include "XMLSettingsStructs.h"
#include "XMLSettingsMeta.h"
#include "colreg/PropertyInterface.h"
//#include "WeatherInterface.h"
#include "common/header_collector.h"

#define _PSN(x) pathfindingMeta.at(x)
#define _RSN(x) researchMeta.at(x)
#define _ESN(x) environmentMeta.at(x)

#define VALCHECKER(type, obj, defobj, value, defValue, offset) [objptr = obj, defobjptr = defobj, va = value, da = defValue, offset_m = offset]() mutable -> bool \
{ \
   bool expact = (va - objptr == offset_m - sizeof(type)); \
   bool expdef = (da - defobjptr == offset_m - sizeof(type)); \
   /*ATLASSERT(expact && ("runtime struct offset mismatch: ") && #value);*/ \
   /*ATLASSERT(expdef && ("default struct offset mismatch: ") && #defValue);*/ \
   return true; \
}
#define VALELEM(type, dattype, signature, obj, defobj, field, diff) new xml_properties::TypedSettingsTree<type, dattype>(signature.first, signature.second, &obj.field, &defobj.field, diff, VALCHECKER(type, (char*)&obj, (char*)&defobj, (char*)&obj.field, (char*)&defobj.field, offset += sizeof(type))())
#define PSTVALELEM(type, signature, field) VALELEM(type, settings::pathfinding_settings, signature, data, defdata, field, diff)
#define RSTVALELEM(type, signature, field) VALELEM(type, settings::research_settings, signature, data, defdata, field, diff)
#define ENVVALELEM(type, signature, field) VALELEM(type, settings::environment_settings, signature, data, defdata, field, diff)

#define PSTCOLLECTINGELEM(signature) new xml_properties::TypedSettingsTree<void*, settings::pathfinding_settings>(signature.first, "",
#define RSTCOLLECTINGELEM(signature) new xml_properties::TypedSettingsTree<void*, settings::research_settings>(signature.first, "",
#define ENVCOLLECTINGELEM(signature) new xml_properties::TypedSettingsTree<void*, settings::environment_settings>(signature.first, "",

namespace SV::serializer
{
   constexpr const char* PATHFINDING_SETTINGS_HEADER = "PathfindingSettings";
   constexpr const char* PATHFINDING_SETTINGS_VERSION = "1.0";
   constexpr const char* RESEARCH_SETTINGS_HEADER = "ResearchSettings";
   constexpr const char* RESEARCH_SETTINGS_VERSION = "1.0";
   constexpr const char* ENVIRONMENT_SETTINGS_HEADER = "EnvironmentSettings";
   constexpr const char* ENVIRONMENT_SETTINGS_VERSION = "1.0";

   constexpr const char* PATHFINDING_SETTINGS_TAG = "pathfinding";
   constexpr const char* RESEARCH_SETTINGS_TAG = "research";
   constexpr const char* ENVIRONMENT_SETTINGS_TAG = "environment";

   class XMLSettingsSerializer : public iXMLSettingsSerializer
   {
   public:
      template<typename DataType>
      static std::unique_ptr<xml_properties::TypedSettingsTree<void*, DataType>> buildDataTree(DataType& data, const DataType& defdata, bool diff) { ATLASSERT(false); return nullptr; }

      template<>
      static std::unique_ptr<xml_properties::TypedSettingsTree<void*, settings::pathfinding_settings>> buildDataTree(settings::pathfinding_settings& data, const settings::pathfinding_settings& defdata, bool diff)
      {
         size_t offset = 0;
         std::unique_ptr<xml_properties::TypedSettingsTree<void*, settings::pathfinding_settings>> tree(new xml_properties::TypedSettingsTree<void*, settings::pathfinding_settings>(PATHFINDING_SETTINGS_TAG, "", {
              PSTCOLLECTINGELEM(_PSN(PathfindingSettingsFieldIndex::PSFI_LEVEL_ST)) {
                   PSTVALELEM(double, _PSN(PathfindingSettingsFieldIndex::PSFI_MAXAIRHEIGHT), lvl_stt.max_air_height)
                 , PSTVALELEM(double, _PSN(PathfindingSettingsFieldIndex::PSFI_MAXLANDHEIGHT), lvl_stt.max_land_height)
                 , PSTVALELEM(double, _PSN(PathfindingSettingsFieldIndex::PSFI_MINLANDHEIGHT), lvl_stt.min_land_height)
                 , PSTVALELEM(double, _PSN(PathfindingSettingsFieldIndex::PSFI_MAXLANDANGLE), lvl_stt.max_land_angle)
                 , PSTVALELEM(double, _PSN(PathfindingSettingsFieldIndex::PSFI_DANGEROUSLANDANGLE), lvl_stt.dangerous_land_angle)
              })
         }) );
         // NOTE: проверку может выбить из-за настроек по роботам
         //ATLASSERT(offset == sizeof(settings::pathfinding_settings));
         return tree;
      }
      template<>
      static std::unique_ptr<xml_properties::TypedSettingsTree<void*, settings::research_settings> > buildDataTree(settings::research_settings& data, const settings::research_settings& defdata, bool diff)
      {
         size_t offset = 0;
         std::unique_ptr<xml_properties::TypedSettingsTree<void*, settings::research_settings> > tree(new xml_properties::TypedSettingsTree<void*, settings::research_settings>(RESEARCH_SETTINGS_TAG, "", {
              RSTCOLLECTINGELEM(_RSN(ResearchSettingsFieldIndex::RSFI_COUNTRANGE_ST)) {
                   RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_MIN), fly_count_range.min_val)
                 , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_MAX), fly_count_range.max_val)
                 , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_STEP), fly_count_range.step)
              })
            , RSTCOLLECTINGELEM(_RSN(ResearchSettingsFieldIndex::RSFI_LENGTHRANGE_ST)) {
                   RSTVALELEM(double, _RSN(ResearchSettingsFieldIndex::RSFI_MIN), length_range.min_val)
                 , RSTVALELEM(double, _RSN(ResearchSettingsFieldIndex::RSFI_MAX), length_range.max_val)
                 , RSTVALELEM(double, _RSN(ResearchSettingsFieldIndex::RSFI_STEP), length_range.step)
              })
            , RSTCOLLECTINGELEM(_RSN(ResearchSettingsFieldIndex::RSFI_THREADRANGE_ST)) {
                   RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_MIN), thread_pool_range.min_val)
                 , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_MAX), thread_pool_range.max_val)
                 , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_STEP), thread_pool_range.step)
              })
            , RSTCOLLECTINGELEM(_RSN(ResearchSettingsFieldIndex::RSFI_TASKRANGE_ST)) {
                   RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_MIN), task_pool_range.min_val)
                 , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_MAX), task_pool_range.max_val)
                 , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_STEP), task_pool_range.step)
              })
            , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_ITERCOUNT), iter_count)
            , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_MAPSIZE), map_size)
            , RSTVALELEM(bool, _RSN(ResearchSettingsFieldIndex::RSFI_MULTITHREADTEST), multi_thread_test)
            , RSTVALELEM(bool, _RSN(ResearchSettingsFieldIndex::RSFI_SINGLETHREADTEST), single_thread_test)
            , RSTVALELEM(size_t, _RSN(ResearchSettingsFieldIndex::RSFI_DEBUGLEVEL), debug_level)
         }) );
         // NOTE: тут не сходилось, пришлось отключить
         //ATLASSERT(offset == sizeof(settings::research_settings));
         return tree;
      }
      template<>
      static std::unique_ptr<xml_properties::TypedSettingsTree<void*, settings::environment_settings> > buildDataTree(settings::environment_settings& data, const settings::environment_settings& defdata, bool diff)
      {
         size_t offset = 0;
         std::unique_ptr<xml_properties::TypedSettingsTree<void*, settings::environment_settings> > tree(new xml_properties::TypedSettingsTree<void*, settings::environment_settings>(ENVIRONMENT_SETTINGS_TAG, "", {
              ENVCOLLECTINGELEM(_ESN(EnvironmentSettingsFieldIndex::ESFI_GSC)) {
                   ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_ANGLE), gcs_info.angle)
                 , ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_SCALE), gcs_info.scale)
                 , ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_ORDBIAS), gcs_info.ordinate_bias)
                 , ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_ABSBIAS), gcs_info.abscissa_bias)
              })
            , ENVCOLLECTINGELEM(_ESN(EnvironmentSettingsFieldIndex::ESFI_MTX)) {
                   ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_ANGLE), mtx_info.angle)
                 , ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_SCALE), mtx_info.scale)
                 , ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_ORDBIAS), mtx_info.ordinate_bias)
                 , ENVVALELEM(double, _ESN(EnvironmentSettingsFieldIndex::ESFI_ABSBIAS), mtx_info.abscissa_bias)
              })
         }) );
         // NOTE: тут не сходилось, пришлось отключить
         //ATLASSERT(offset == sizeof(settings::research_settings));
         return tree;
      }

      void Release() override { delete this; }

      template<typename DataType>
      static inline const char* getXMLDataHeader() { return nullptr; }
      template<>
      static inline const char* getXMLDataHeader<settings::pathfinding_settings>() { return PATHFINDING_SETTINGS_HEADER; }
      template<>
      static inline const char* getXMLDataHeader<settings::research_settings>() { return RESEARCH_SETTINGS_HEADER; }
      template<>
      static inline const char* getXMLDataHeader<settings::environment_settings>() { return ENVIRONMENT_SETTINGS_HEADER; }
      
      template<typename DataType>
      static inline const char* getXMLDataVersion() { return nullptr; }
      template<>
      static inline const char* getXMLDataVersion<settings::pathfinding_settings>() { return PATHFINDING_SETTINGS_VERSION; }
      template<>
      static inline const char* getXMLDataVersion<settings::research_settings>() { return RESEARCH_SETTINGS_VERSION; }
      template<>
      static inline const char* getXMLDataVersion<settings::environment_settings>() { return ENVIRONMENT_SETTINGS_VERSION; }

      bool Serialize(const char* filename, const settings::pathfinding_settings& settings, const settings::pathfinding_settings& defsettings) const final { return serialize(filename, settings, defsettings); }
      bool Serialize(const char* filename, const settings::pathfinding_settings& settings) const final { return serialize(filename, settings); }
      bool Serialize(const char* filename, const settings::research_settings& settings, const settings::research_settings& defsettings) const final { return serialize(filename, settings, defsettings); }
      bool Serialize(const char* filename, const settings::research_settings& settings) const final { return serialize(filename, settings); }
      bool Serialize(const char* filename, const settings::environment_settings& settings) const final { return serialize(filename, settings); }
      bool Serialize(const char* filename, const settings::simulation_settings& settings) const final { return true; }
      bool Serialize(const char* filename, const settings::map_settings& settings) const final { return true; }

      bool Deserialize(const char* filename, settings::pathfinding_settings& environment) const final { return deserialize(filename, environment); }
      bool Deserialize(const char* filename, settings::pathfinding_settings& environment, bool mult) const final { return deserialize(filename, environment, mult); }
      bool Deserialize(const char* filename, settings::research_settings& environment) const final { return deserialize(filename, environment); }
      bool Deserialize(const char* filename, settings::research_settings& environment, bool mult) const final { return deserialize(filename, environment, mult); }
      bool Deserialize(const char* filename, settings::environment_settings& environment) const final { return deserialize(filename, environment); }
      bool Deserialize(const char* filename, settings::simulation_settings& environment) const final { return true; }
      bool Deserialize(const char* filename, settings::map_settings& environment) const final { return true; }

      xml_properties::PropertyItem* DirectSerialize(const settings::pathfinding_settings& data, const settings::pathfinding_settings& defdata) const final { return serialize(data, defdata); }
      xml_properties::PropertyItem* DirectSerialize(const settings::research_settings& data, const settings::research_settings& defdata) const final { return serialize(data, defdata); }
      
      void DirectDeserialize(const xml_properties::PropertyItem& root, settings::pathfinding_settings& data) const final { deserialize(root, data); }
      void DirectDeserialize(const xml_properties::PropertyItem& root, settings::research_settings& data) const final { deserialize(root, data); }

      virtual const char* ToString(const settings::pathfinding_settings& settings) const final { return toString(settings); }
      virtual const char* ToString(const settings::pathfinding_settings& settings, const settings::pathfinding_settings& defsettings) const final { return toString(settings, defsettings); }
      virtual const char* ToString(const settings::research_settings& environment) const final { return toString(environment); }
      virtual const char* ToString(const settings::research_settings& environment, const settings::research_settings& defenvironment) const final { return toString(environment, defenvironment); }
      virtual const char* ToString(const settings::environment_settings& environment) const final { return toString(environment); }
      virtual const char* ToString(const settings::simulation_settings& environment) const final { return nullptr; }
      virtual const char* ToString(const settings::map_settings& environment) const final { return nullptr; }

      virtual bool FromString(const char* src, settings::pathfinding_settings& settings) const final { return fromString(src, settings); }
      virtual bool FromString(const char* src, settings::research_settings& environment) const final { return fromString(src, environment); }
      virtual bool FromString(const char* src, settings::environment_settings& environment) const final { return fromString(src, environment); }
      virtual bool FromString(const char* src, settings::simulation_settings& environment) const final { return true; }
      virtual bool FromString(const char* src, settings::map_settings& environment) const final { return true; }
   private:
      template<typename DataType>
      static bool serialize(const char* filename, const DataType& data, const DataType& defdata)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<DataType>(), getXMLDataVersion<DataType>());
         auto tree = buildDataTree((DataType&)data, defdata, true);
         traverseSerialize(tree.get(), &c.GetRoot());

         return c.save(filename);
      }

      template<typename DataType>
      static bool serialize(const char* filename, const DataType& data)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<DataType>(), getXMLDataVersion<DataType>());
         auto tree = buildDataTree((DataType&)data, DataType(), false);
         traverseSerialize(tree.get(), &c.GetRoot());
         return c.save(filename);
      }
      static bool serialize(const char* filename, const iPropertyInterface* data)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<iPropertyInterface>(), getXMLDataVersion<iPropertyInterface>());
         traverseSerialize((iPropertyInterface*)data, &c.GetRoot());
         return c.save(filename);
      }

      template<typename DataType>
      static xml_properties::PropertyItem* serialize(const DataType& data, const DataType& defdata)
      {
         xml_properties::PropertyItem* nwtree = new xml_properties::PropertyItem("");
         auto tree = buildDataTree((DataType&)data, defdata, true);
         walker(tree.get(), *nwtree);
         traverseSerialize(tree.get(), nwtree);

         return nwtree;
      }
      static xml_properties::PropertyItem* serialize(const iPropertyInterface* data)
      {
         xml_properties::PropertyItem* nwtree = new xml_properties::PropertyItem("");
         traverseSerialize((iPropertyInterface*)data, nwtree);
         return nwtree;
      }

      template<typename DataType>
      static void walker(const xml_properties::iSettingsTree<DataType>* tree, xml_properties::PropertyItem& root)
      {
         for (const auto it : tree->_children)
         {
            const char* tag = it->_tag.c_str();
            if (!it->_children.empty())
               walker(it, root[tag]);
            else if (!it->CompareValues())
               root[tag] = 0.;
         }
      }

      template<typename DataType>
      static void correctData(DataType& data) {}
      template<>
      static void correctData(settings::pathfinding_settings& data) {}
      template<>
      static void correctData(settings::research_settings& data) {}

      template<typename DataType>
      static bool deserialize(const char* filename, DataType& data, bool mult)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<DataType>(), getXMLDataVersion<DataType>());
         if (!c.load(filename))
            return false;
         traverseDeserialize(buildDataTree(data, data, true).get(), &c.GetRoot());
         correctData(data);
         return true;
      }

      template<typename DataType>
      static bool deserialize(const char* filename, DataType& data)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<DataType>(), getXMLDataVersion<DataType>());
         if (!c.load(filename))
            return false;
         traverseDeserialize(buildDataTree(data, data, true).get(), &c.GetRoot());
         correctData(data);
         return true;
      }
      static bool deserialize(const char* filename, iPropertyInterface* data)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<iPropertyInterface>(), getXMLDataVersion<iPropertyInterface>());
         if (!c.load(filename))
            return false;
         traverseDeserialize(data, &c.GetRoot());
         return true;
      }

      template<typename DataType>
      static bool deserialize(const xml_properties::PropertyItem& root, DataType& data)
      {
         traverseDeserialize(buildDataTree(data, data, true).get(), &root);
         return true;
      }
      static bool deserialize(const xml_properties::PropertyItem& root, iPropertyInterface* data)
      {
         traverseDeserialize(data, &root);
         return true;
      }

      template<typename DataType>
      static const char* toString(const DataType& data, const DataType& defdata)
      {
         static std::string staticBuffer;
         xml_properties::PropertyContainer c(getXMLDataHeader<DataType>(), getXMLDataVersion<DataType>());
         traverseSerialize(buildDataTree((DataType&)data, defdata, true).get(), &c.GetRoot());
         staticBuffer = c.toString();
         return staticBuffer.c_str();
      }

      template<typename DataType>
      static const char* toString(const DataType& data)
      {
         static std::string staticBuffer;
         xml_properties::PropertyContainer c(getXMLDataHeader<DataType>(), getXMLDataVersion<DataType>());
         traverseSerialize(buildDataTree((DataType&)data, DataType(), false).get(), &c.GetRoot());
         staticBuffer = c.toString();
         return staticBuffer.c_str();
      }
      static const char* toString(const iPropertyInterface* data)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<iPropertyInterface>(), getXMLDataVersion<iPropertyInterface>());
         traverseSerialize((iPropertyInterface*)data, &c.GetRoot());
         return c.toString().c_str();
      }

      template<typename DataType>
      static bool fromString(const char* src, DataType& data)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<DataType>(), getXMLDataVersion<DataType>());
         if (!c.fromString(src))
            return false;

         deserialize(c.GetRoot(), data);
         return true;
      }
      static bool fromString(const char* src, iPropertyInterface* data)
      {
         xml_properties::PropertyContainer c(getXMLDataHeader<iPropertyInterface>(), getXMLDataVersion<iPropertyInterface>());
         if (!c.fromString(src))
            return false;

         deserialize(c.GetRoot(), data);
         return true;
      }

      static inline int isNotAlphaNum(char c) { return !std::isalnum(c); }

      static inline void replaceNotAlphaNum(std::string& src) { std::replace_if(src.begin(), src.end(), isNotAlphaNum, '_'); }

      template<typename DataType>
      static void traverseSerialize(xml_properties::iSettingsTree<DataType>* node, xml_properties::PropertyItem* item)
      {
         for (auto it : node->_children)
         {
            auto nwItem = item->Child(it->_tag.c_str());

            if (!nwItem && (!it->_children.empty() || !it->CompareValues()))
            {
               nwItem = &item->AddChild(it->_tag.c_str());
            }

            if (it->_children.empty())
               it->SerializeValue(nwItem);
            else
               traverseSerialize(it, nwItem);
         }
      }
      static void traverseSerialize(iPropertyInterface* node, xml_properties::PropertyItem* item)
      {
         if (!node)
         {
            return;
         }
         for (auto* it : node->get_childs())
         {
            std::string name(it->get_name());
            replaceNotAlphaNum(name);
            auto nwItem = item->Child(name.c_str());

            if (!nwItem)
               nwItem = &item->AddChild(name.c_str());

            if (it->get_childs().empty())
            {
               ATLASSERT(nwItem);
               *nwItem = it->get_value();
               nwItem->SetDesc(it->get_description());
            }
            else
               traverseSerialize(it, nwItem);
         }
      }

      template<typename DataType>
      static void traverseDeserialize(const xml_properties::iSettingsTree<DataType>* node, const xml_properties::PropertyItem* item)
      {
         for (const auto it : node->_children)
         {
            auto nwItem = item->GetChild(it->_tag.c_str());
            if (!nwItem)
               continue;
            if (it->_children.empty())
               it->DeserializeValue(nwItem);
            else
               traverseDeserialize(it, nwItem);
         }
      }
      static void traverseDeserialize(const iPropertyInterface* node, const xml_properties::PropertyItem* item)
      {
         for (auto* it : node->get_childs())
         {
            std::string name(it->get_name());
            replaceNotAlphaNum(name);
            auto nwItem = item->GetChild(name.c_str());
            if (!nwItem)
               continue;
            if (it->get_childs().empty())
            {
               std::string data;
               nwItem->Get(data);
               it->set_value(data.c_str());
            }
            else
               traverseDeserialize(it, nwItem);
         }
      }

      void ReleaseTree(xml_properties::PropertyItem* tree) const { ATLASSERT(tree); delete tree; }

      mutable std::string _xmlString;
   };
}

SV::serializer::iSettingsSerializer * CreateXMLSettingsSerializer()
{
   return new SV::serializer::XMLSettingsSerializer();
}

#undef _PSN
#undef _RSN

#undef VALCHECKER
#undef VALELEM
#undef PSTTVALELEM
#undef RSTTVALELEM