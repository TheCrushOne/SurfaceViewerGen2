#include "stdafx.h"
#include "property.h"
#include "PropertyContainer.h"
#include "ThirdParty\tinyxml2\tinyxml2.h"
 
using deleted_unique_ptr = std::unique_ptr<tinyxml2::XMLDocument, void(*)(tinyxml2::XMLDocument*)>;
void saveWalker(deleted_unique_ptr *cDoc, tinyxml2::XMLElement *parentNode, const PropertyItemList& properties);
void loadWalker(deleted_unique_ptr *cDoc, tinyxml2::XMLElement *parentNode, PropertyItem& prop);

bool PropertyContainer::save( const std::string& strFileName )
{
   deleted_unique_ptr classDoc(new tinyxml2::XMLDocument(), [](tinyxml2::XMLDocument* p) { delete p; });
	tinyxml2::XMLElement *rootNode;
 	rootNode = classDoc->NewElement( "SerializableClass" );
 	rootNode->SetAttribute( "Type", _strType.c_str());
 	rootNode->SetAttribute( "Version", _strVersion.c_str());
 
	classDoc->InsertEndChild(rootNode);
   
   saveWalker(&classDoc, rootNode, _root.GetChildren());
     
	tinyxml2::XMLPrinter printer;
	classDoc->Accept( &printer );
	classDoc->Print();
	std::string xmlResult = printer.CStr();
	classDoc->SaveFile(strFileName.c_str());
	classDoc->Clear();
	 
 	return true;
}

void saveWalker(deleted_unique_ptr *cDoc, tinyxml2::XMLElement *parentNode, const PropertyItemList& properties)
{
   for (auto prop : properties)
   {
      //auto& prpt = static_cast<PropertyItem&>(prop.second);
      tinyxml2::XMLElement *memberNode = (*cDoc)->NewElement(prop.second.GetName());
      parentNode->InsertEndChild(memberNode);
      memberNode->SetAttribute("description", prop.second.GetDesc());                   
      auto chmap = prop.second.GetChildren();
      if (!chmap.empty())
         saveWalker(cDoc, memberNode, chmap);
      else
         memberNode->InsertEndChild((*cDoc)->NewText(prop.second.GetCharValue()));
   }
}

bool PropertyContainer::load(const std::string& strFileName )
{
   std::unique_ptr<tinyxml2::XMLDocument, void(*)(tinyxml2::XMLDocument*)>classDoc(new tinyxml2::XMLDocument(), [](tinyxml2::XMLDocument* p) { delete p; });
	if (classDoc->LoadFile(strFileName.c_str() ) == tinyxml2::XMLError::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* pElem;
	 	pElem = classDoc->FirstChildElement();
	 	if (!pElem) return false;
		std::string m_name = pElem->Value();

      loadWalker(&classDoc, pElem, _root);		
		
      return true;
	}
   return false;
}


void loadWalker(deleted_unique_ptr *cDoc, tinyxml2::XMLElement *parentNode, PropertyItem& prop)
{
   tinyxml2::XMLElement* pElem;
   for (pElem = parentNode->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
   {
      const char *pKey = pElem->Value();
      if (pKey && !prop.Exists(pKey))
      {
         const char *pDesc = pElem->Attribute("description");
         const char *pText = pElem->GetText();
         auto& child = prop.AddChild(pKey ? pKey : "default");
         child.SetDesc(pDesc ? pDesc : "default");
         if (pElem->LastChildElement() != pElem->FirstChildElement())
            loadWalker(cDoc, pElem, child);
         else
         {                        
            if (pText)                      
               child.Put<const char*>(pText); 
         }
      }
   }
}