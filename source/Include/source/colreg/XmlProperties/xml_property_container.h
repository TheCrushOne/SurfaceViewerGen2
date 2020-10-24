#pragma once

#include "xml_property.h"
#include "colreg\ThirdParty\tinyxml2\tinyxml2.h"
#include <map>

namespace SV::xml_properties
{
   struct ci_less
   {
      bool operator()(const std::string& left, const std::string& right) const
      {
         return ::_stricmp(left.c_str(), right.c_str()) < 0;
      }
   };
   class PropertyItem;

   using PropertyItemList = std::multimap< std::string, PropertyItem, ci_less >;
   using Attributes = std::map< std::string, std::string >;

   class PropertyItem
   {

   public:
      PropertyItem(const char* pname) : _children() { _prop.SetName(pname); }
      PropertyItem& AddChild(const char* pname)
      {
         auto iter = _children.emplace(pname, PropertyItem(pname));
         ATLASSERT(Exists(pname));
         return iter->second;
      }

      void DelChild(const char* pname)
      {
        _children.erase(pname);
      }

      template<typename Type>
      inline bool Put(const Type& pvalue, const char* pdesc = "")
      {
         _prop.SetDesc(pdesc);
         _prop.SetValue(pvalue);
         return true;
      }
      inline bool Put(const char* value, const char* pdesc = "")
      {
         _prop.SetDesc(pdesc);
         _prop.SetCharValue(value);
         return true;
      }
      inline bool Put(const std::string& value, const char* pdesc = "")
      {
         _prop.SetDesc(pdesc);
         _prop.SetCharValue(value.c_str());
         return true;
      }

      void SetAttribute(const char* name, const char* value)
      {
         _attributes[name] = value;
      }

      const char* GetAttribute(const char* name) const
      {
         auto it = _attributes.find(name);
         return it == _attributes.end() ? nullptr : it->second.c_str();
      }

      template <typename Type>
      inline PropertyItem& operator=(const Type& value)
      {
         _prop.SetValue(value);

         return *this;
      }

      template<typename Type>
      inline Type Get() const
      {
         return _prop.GetValue<Type>();
      }
      template<typename Type>
      inline void Get(Type& value) const
      {
         value = _prop.GetValue<Type>();
      }
      inline void Get(std::string& value) const
      {
         value = std::string(_prop.GetCharValue());
      }

      inline const PropertyItem* GetChild(const char* pname) const { return findChild(pname); }
      inline PropertyItem* Child(const char* pname) { return findChild(pname); }
      const PropertyItemList& GetChildren() const { return _children; }

      inline bool Exists(const char* pname) const { return _children.find(pname) != _children.end(); }
      inline bool IsEmpty() const { return _prop.IsEmpty(); }

      const char* GetName() const { return _prop.GetName(); }
      const char* GetDesc() const { return _prop.GetDesc(); }
      const char* GetCharSubValue() const { return _prop.GetCharSubValue(); }
      const char* GetCharValue() const { return _prop.GetCharValue(); }

      void SetCharSubValue(const char* subval) { _prop.SetCharSubValue(subval); }
      void SetDesc(const char* desc) { _prop.SetDesc(desc); }

      inline PropertyItem& operator[](const char* name) { return Exists(name) ? *findChild(name) : AddChild(name); }
      inline const PropertyItem& operator[](const char* name) const  { ATLASSERT(Exists(name));  return *findChild(name); }

      template<typename T>
      typename std::enable_if<std::is_enum<T>::value, bool>::type
         GetItemValue(const char* pname, T& itemValue)const
      {
         const auto ret = Exists(pname);
         if (ret)
            itemValue = static_cast<T>(GetChild(pname)->Get<size_t>());
         return ret;
      }

      template<typename T>
      typename std::enable_if<!std::is_enum<T>::value, bool>::type
         GetItemValue(const char* pname, T& itemValue)const
      {
         const auto ret = Exists(pname);
         if (ret)
            itemValue = GetChild(pname)->Get<T>();
         return ret;
      }

      template<>
      bool GetItemValue<bool>(const char* pname, bool& itemValue)const
      {
         const auto ret = Exists(pname);
         if (ret)
            itemValue = GetChild(pname)->Get<size_t>() == 1;
         return ret;
      }

   private:
      PropertyItem* findChild(const char* pName)
      {
         auto it = _children.find(pName);
         return  it != _children.end() ? &(*it).second : nullptr;
      }
   
      const PropertyItem* findChild(const char* pName) const 
      {
         const auto it = _children.find(pName);
         return  it != _children.end() ? &(*it).second : nullptr;
      }

      ValueProperty _prop;
      PropertyItemList _children;
      Attributes _attributes;
   };

   using XmlDocumentPtr = std::unique_ptr<tinyxml2::XMLDocument, void( *)( tinyxml2::XMLDocument* )>;

   class PropertyContainer
   {
   public:
      PropertyContainer(PropertyContainer& rs) : _strType{ rs._strType }, _strVersion{ rs._strVersion }, _root(rs._root) { }
      PropertyContainer(const char* pType, const char* pVersion) : _strType{ pType }, _strVersion{ pVersion }, _root(PropertyItem("data")) { }

      inline PropertyItem& GetRoot() { return _root; }
      inline const PropertyItem& GetRoot() const { return _root; }

      bool save(const std::string& strFileName) const;
      bool load_ver(const std::string& strFileName);
      bool load(const std::string& strFileName);
      bool load(const std::string& strFileName, std::string& xmlver);

      std::string toString() const;
      bool        fromString( const std::string& xmlStr );

   private:
      XmlDocumentPtr generateXmlDocument() const;

      PropertyItem _root;

      std::string _strType;
      std::string _strVersion;
   };

   inline void saveWalker(XmlDocumentPtr *cDoc, tinyxml2::XMLElement *parentNode, const PropertyItemList& properties)
   {
      for (auto prop : properties)
      {
         tinyxml2::XMLElement *memberNode = (*cDoc)->NewElement(prop.second.GetName());
         parentNode->InsertEndChild(memberNode);
         LPCSTR desc = prop.second.GetDesc();
         if (desc != nullptr && strlen(desc) > 0)
            memberNode->SetAttribute("description", desc);
         LPCSTR subval = prop.second.GetCharSubValue();
         if (subval != nullptr && strlen(subval) > 0)
            memberNode->SetAttribute("subvalue", subval);
         auto chmap = prop.second.GetChildren();
         if (!chmap.empty())
            saveWalker(cDoc, memberNode, chmap);
         else
            memberNode->InsertEndChild((*cDoc)->NewText(prop.second.GetCharValue()));
      }
   }

   inline void loadWalker(XmlDocumentPtr *cDoc, tinyxml2::XMLElement *parentNode, PropertyItem& prop)
   {
      tinyxml2::XMLElement* pElem;
      for (pElem = parentNode->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
      {
         const char *pKey = pElem->Value();
         if (pKey /*&& !prop.Exists(pKey)*/)
         {
            const char *pDesc = pElem->Attribute("description");
            const char *pSubVal = pElem->Attribute("subvalue");
            const char *pText = pElem->GetText();
            auto& child = prop.AddChild(pKey ? pKey : "default");
            child.SetDesc(pDesc ? pDesc : "default");
            child.SetCharSubValue(pSubVal ? pSubVal : "");
            {
               auto attr = pElem->FirstAttribute();
               while (attr)
               {
                  const auto attrName = attr->Name();
                  const auto attrVal = attr->Value();
                  child.SetAttribute(attrName, attrVal);
                  attr = attr->Next();
               }
            }
            
            if (pElem->FirstChildElement())
               loadWalker(cDoc, pElem, child);
            else
            {
               if (pText)
                  child.Put<const char*>(pText);
            }
         }
      }
   }

   inline bool PropertyContainer::save(const std::string& strFileName) const
   {
      auto classDoc = generateXmlDocument();

      classDoc->SaveFile(strFileName.c_str());
      classDoc->Clear();

      return true;
   }

   inline bool PropertyContainer::load_ver(const std::string& strFileName)
   {
     XmlDocumentPtr classDoc(new tinyxml2::XMLDocument(), [](tinyxml2::XMLDocument* p) { delete p; });
     if (classDoc->LoadFile(strFileName.c_str()) == tinyxml2::XMLError::XML_NO_ERROR)
     {
       tinyxml2::XMLElement* pElem;
       pElem = classDoc->FirstChildElement();
       if (!pElem) return false;

       loadWalker(&classDoc, pElem, _root);

       if (!_strVersion.size())  //version not specified - assume it does not matter
       {
         return true;
       }

       return classDoc->RootElement()->Attribute("Version") ? _strVersion == classDoc->RootElement()->Attribute("Version") : false;
     }

     return false;
   }

   inline bool PropertyContainer::load(const std::string& strFileName)
   {
      std::string xmlver;
      return load(strFileName, xmlver);
   }

   inline bool PropertyContainer::load(const std::string& strFileName, std::string& xmlver)
   {
      XmlDocumentPtr classDoc(new tinyxml2::XMLDocument(), [](tinyxml2::XMLDocument* p) { delete p; });     
      if (classDoc->LoadFile(strFileName.c_str()) == tinyxml2::XMLError::XML_NO_ERROR)
      {
         tinyxml2::XMLElement* pElem;
         pElem = classDoc->FirstChildElement();
         _strVersion = pElem->Attribute("Version");
         _strType = pElem->Attribute("Type");
         if (!pElem) return false;

         loadWalker(&classDoc, pElem, _root);

         if (!_strVersion.size())  //version not specified - assume it does not matter
         {
            return true;
         }
         xmlver = _strType;
         std::string s1 = classDoc->RootElement()->Attribute("Version");
         return classDoc->RootElement()->Attribute("Version") ? _strVersion == classDoc->RootElement()->Attribute("Version") : false;
      }
      
      return false;
   }

   inline std::string PropertyContainer::toString() const
   {
      auto classDoc = generateXmlDocument();

      tinyxml2::XMLPrinter printer;
      classDoc->Accept( &printer );

      return printer.CStr();
   }

   inline bool PropertyContainer::fromString( const std::string& xmlStr )
   {
      XmlDocumentPtr classDoc( new tinyxml2::XMLDocument(), []( tinyxml2::XMLDocument* p ) { delete p; } );
      if ( classDoc->Parse( xmlStr.c_str(), xmlStr.length() ) != tinyxml2::XML_NO_ERROR )
         return false;

      tinyxml2::XMLElement* pElem;
      pElem = classDoc->FirstChildElement();
      if ( !pElem )
         return false;

      loadWalker( &classDoc, pElem, _root );

      return true;
   }

   inline xml_properties::XmlDocumentPtr PropertyContainer::generateXmlDocument() const
   {
      XmlDocumentPtr classDoc( new tinyxml2::XMLDocument(), []( tinyxml2::XMLDocument* p ) { delete p; } );
      tinyxml2::XMLElement *rootNode;
      rootNode = classDoc->NewElement( "SerializableClass" );
      rootNode->SetAttribute( "Type", _strType.c_str() );
      rootNode->SetAttribute( "Version", _strVersion.c_str() );

      classDoc->InsertEndChild( rootNode );

      saveWalker( &classDoc, rootNode, _root.GetChildren() );

      return classDoc;
   }
}
