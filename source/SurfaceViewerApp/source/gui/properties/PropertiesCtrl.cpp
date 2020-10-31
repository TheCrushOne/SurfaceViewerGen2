#include "stdafx.h"
#include "PropertiesCtrl.h"
#include "gui/selection/SelectedObjectManager.h"
#include "gui/MainFrm.h"
#include <set>
#include "common\utils.h"

using namespace SV;

namespace
{
   struct maskData
   {
      std::string editMask;
      std::string editTemplate;
   };

   const std::unordered_map<VALUE_FORMAT_TYPE, maskData> valueMask = {
      { VALUE_FORMAT_TYPE::VFT_NONE, { "" , "" } },
      /*{ VALUE_FORMAT_TYPE::VFT_ANGLE, { "ddd d°"
                                       , "___._°" } },*/
      { VALUE_FORMAT_TYPE::VFT_SPEED, { "ddd dd kts"
                                       ,   "___.__ kts" } },
      { VALUE_FORMAT_TYPE::VFT_SPEED_MS, { "ddd dd m/s"
                                       ,   "___.__ m/s" } },
      { VALUE_FORMAT_TYPE::VFT_COURSE, { "ddd d°"
                                       , "___._°" } },
      { VALUE_FORMAT_TYPE::VFT_DISTANCE, { "ddd NM"
                                       ,   "___ NM" } },
      { VALUE_FORMAT_TYPE::VFT_DISTANCE_METERS, { "ddd m"
                                       ,   "___ m" } },

      { VALUE_FORMAT_TYPE::VFT_TIME_SEC, { "00000 s"
                                       ,   "_____ s" } },

      { VALUE_FORMAT_TYPE::VFT_COORD_LAT, { "ddd dddd °"
                                       ,   "___.____ °" } },
      { VALUE_FORMAT_TYPE::VFT_COORD_LON, { "ddd dddd °"
                                       ,   "___.____ °" } },

      { VALUE_FORMAT_TYPE::VFT_PERCENT, { "ddd %"
                                      ,   "___ %" } },
   };
}

//====================================================================================================================================================

void CPropertiesCtrl::ShowProperties(iPropertyInterface* prop, bool fullReload)
{
   _selected = prop;
   if (fullReload)
   {
      _prop_struct.clear();
      RemoveAll();
   }

   if (!prop)
   {
      return;
   }

   LockWindowUpdate();
   auto* childs = prop->get_childs();
   std::set< std::string > addedFolders;
   for (auto child : *childs)
   {
      addFolder(child, nullptr, &_prop_struct);
      addedFolders.insert(child->get_name());
   }

   if (!fullReload)
   {  //почистим старые фолдера, которых нет в новой структуре
      for (auto it = _prop_struct.begin(); it != _prop_struct.end();)
      {
         if (addedFolders.find(it->first) == addedFolders.end())
         {
            DeleteProperty(it->second.grid_prop);
            it = _prop_struct.erase(it); 
         }
         else
         {
            ++it; //такой фолдер есть в новой структуре
         }
      }
   }
   AdjustLayout();
   UnlockWindowUpdate();
}

//====================================================================================================================================================

void CPropertiesCtrl::addFolder(iPropertyInterface* folder, CMFCPropertyGridProperty* pFolderProp, props_structure* parentStruct)
{
   // TODO: починить
   if (!folder)
      return;
   if (folder->get_type() == PROPERTY_TYPE::PT_FOLRDER)
   {
      auto* childs = folder->get_childs();
      
      bool newFolderCreated = false;
      props_structure* currentPropStruct = nullptr;
      CMFCPropertyGridProperty* pSubFolder = nullptr;
      auto subFolderItemsIt = parentStruct->find(folder->get_name());
      if (subFolderItemsIt == parentStruct->end() || subFolderItemsIt->second.prop_type != PROPERTY_TYPE::PT_FOLRDER)
      {
         if (subFolderItemsIt != parentStruct->end()) //свойство с таким именем найдено , но тип отличается
         {
            DeleteProperty(subFolderItemsIt->second.grid_prop);
            parentStruct->erase(subFolderItemsIt);
         }
         newFolderCreated = true;
         std::string name(folder->get_name());
         std::wstring wname(name.begin(), name.end());
         pSubFolder = new CMFCPropertyGridProperty(wname.c_str());
         currentPropStruct = &parentStruct->insert({ folder->get_name(), { pSubFolder, folder->get_type() } }).first->second.prop_childs;
         pFolderProp ? pFolderProp->AddSubItem(pSubFolder) : AddProperty(pSubFolder, FALSE);
      }
      else
      { 
         //используем существующий Folder
         newFolderCreated = false;
         pSubFolder = subFolderItemsIt->second.grid_prop;
         currentPropStruct = &subFolderItemsIt->second.prop_childs;
      }
      
      std::set< std::string > processedItems;
      for (auto& child : *childs)
      {
         if (!child)
         {
            continue;
         }
         processedItems.insert(child->get_name());
         CMFCPropertyGridProperty* childItem = nullptr;
         
         auto childPropIt = currentPropStruct->find(child->get_name());
         if (childPropIt == currentPropStruct->end() || childPropIt->second.prop_type != child->get_type() )
         {
            if (childPropIt != currentPropStruct->end()) //свойство найдено, но тип отличается, нужно пересоздать
            {
               DeleteProperty(childPropIt->second.grid_prop);
               currentPropStruct->erase(childPropIt);
            }

            switch (child->get_type())
            {
            case PROPERTY_TYPE::PT_FOLRDER:
            {
               addFolder(child, pSubFolder, currentPropStruct);
               continue;
            }
            case PROPERTY_TYPE::PT_BOOL:
            {
               auto wname = SVGUtils::stringToWstring(child->get_name());
               auto wdesc = SVGUtils::stringToWstring(child->get_description());
               childItem = new CMFCPropertyGridProperty(wname.c_str(), !strcmp(child->get_value(), "0") ? (_variant_t)false : (_variant_t)true, wdesc.c_str());
               break;
            }
            case PROPERTY_TYPE::PT_VALUE:
            {
               childItem = createMaskedProperty(child);
               break;
            }
            case PROPERTY_TYPE::PT_TEXT_LIST:
            {
               const auto list = child->get_list();
               const auto indx = atoi(child->get_value());

               auto wname = SVGUtils::stringToWstring(child->get_name());
               auto wdesc = SVGUtils::stringToWstring(child->get_description());
               auto wlistelem = SVGUtils::stringToWstring(list[indx].c_str());
               childItem = new CMFCPropertyGridProperty(wname.c_str(), wlistelem.c_str(), wdesc.c_str());
               for (const auto& t : list)
               {
                  auto welem = SVGUtils::stringToWstring(t.c_str());
                  childItem->AddOption(welem.c_str());
               }
               break;
            }
            default:
            {
               auto wname = SVGUtils::stringToWstring(child->get_name());
               auto wdesc = SVGUtils::stringToWstring(child->get_description());
               childItem = new CMFCPropertyGridProperty(wname.c_str(), (_variant_t)child->get_value(), wdesc.c_str());
            }
            }

            currentPropStruct->insert({ child->get_name(), { childItem, child->get_type() } });
            pSubFolder->AddSubItem(childItem);
         }
         else //property alread added
         {
            childItem = childPropIt->second.grid_prop;
            
            /*if (child->get_type() == PROPERTY_TYPE::PT_FOLRDER)
            {
               addFolder(child, );
               // NOTE: ничего не делаем
            }
            else*/ if (child->get_type() == PROPERTY_TYPE::PT_TEXT_LIST)
            {
               const auto list = child->get_list();
               const auto indx = atoi(child->get_value());

               auto wlistelem = SVGUtils::stringToWstring(list[indx].c_str());
               childItem->SetValue(wlistelem.c_str());
            }
            else
            {
               auto oldValType = childItem->GetValue().vt;
               auto newVal = (_variant_t)child->get_value();
               newVal.ChangeType(oldValType); //новое значение должно совпадать по типу со старым в плане VariantType , внутреннего представления

               childItem->SetValue(newVal);
            }
            
         }
         
         childItem->SetData((DWORD_PTR)child);

         if (childItem->GetValue().vt != VT_BOOL) //to skip internal MFC ASSERT
         {
            childItem->AllowEdit(child->is_read_only() ? FALSE : TRUE);
         }
         
         childItem->Enable(child->is_read_only() ? FALSE : TRUE);

      }
      if (!newFolderCreated)
      {
         //удалим лишние айтемы, который могли остаться от старого выделенного объекта
         for (auto it = currentPropStruct->begin(); it != currentPropStruct->end(); )
         {
            if (processedItems.find(it->first ) == processedItems.end())
            {
               DeleteProperty(it->second.grid_prop);
               it = currentPropStruct->erase(it);
            }
            else
            {
               ++it;
            }
         }
      }
      else
      {
//         pSubFolder->Expand(FALSE);
         pSubFolder->Expand();
      }
   }
}

//====================================================================================================================================================

CMFCPropertyGridProperty* CPropertiesCtrl::createMaskedProperty(iPropertyInterface* const &child)
{
   CMFCPropertyGridProperty* nw = nullptr;
   auto valueMaskIt = valueMask.find(child->get_value_format_type());
   auto wname = SVGUtils::stringToWstring(child->get_name());
   auto wdesc = SVGUtils::stringToWstring(child->get_description());
   auto wvalue = SVGUtils::stringToWstring(child->get_value());

   if (valueMaskIt != valueMask.end()) //standard ?
   {
      auto& valMask = valueMaskIt->second;

      auto weditmask = SVGUtils::stringToWstring(valMask.editMask.c_str());
      auto wedittemplate = SVGUtils::stringToWstring(valMask.editTemplate.c_str());

      if (valMask.editMask.compare("") == 0)
         nw = new CMFCPropertyGridProperty(wname.c_str(), (_variant_t)child->get_value(), wdesc.c_str());
      else
         nw = new CMFCPropertyGridProperty(wname.c_str(), (_variant_t)child->get_value(), wdesc.c_str(), 0, weditmask.c_str(), wedittemplate.c_str());
   }
   else
   {
      switch (child->get_value_format_type())
      {
      case VALUE_FORMAT_TYPE::VFT_LOGFILE:
         nw = new CMFCPropertyGridFileProperty(wname.c_str(), TRUE, wvalue.c_str(), L"log", 6, nullptr, wdesc.c_str());
         //   nw->AllowEdit(FALSE);
         break;

      case VALUE_FORMAT_TYPE::VFT_FOLDERPATH:
         nw = new CMFCPropertyGridFileProperty(wname.c_str(), wvalue.c_str(), 0, wdesc.c_str());
         //  nw->AllowEdit(FALSE);
         break;

      default:
         break;
      }
   }
   return nw;
}


//====================================================================================================================================================


void CPropertiesCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
   if (!_selected)
      return;

   if (pProp)
   {
      const auto & name = pProp->GetName();
      iPropertyInterface* prop = reinterpret_cast<iPropertyInterface*>(pProp->GetData());
      switch (prop->get_type())
      {
      case PROPERTY_TYPE::PT_BOOL:
      {
         auto value = pProp->GetValue();
         prop->set_value(value.boolVal == VARIANT_TRUE ? "1" : "0");
         break;
      }
      case PROPERTY_TYPE::PT_TEXT_LIST:
      {
         const auto& list = prop->get_list();
         USES_CONVERSION;
         //CString svalue = W2A(pProp->GetValue().bstrVal);
         std::wstring ws(pProp->GetValue().bstrVal, SysStringLen(pProp->GetValue().bstrVal));
         std::string value(ws.begin(), ws.end());
         for (size_t i = 0; i < list.size(); ++i)
         {
            if (list[i].compare(value) == 0)
            {
               std::stringstream s;
               s << i;
               prop->set_value(s.str().c_str());
               break;
            }
         }

         break;
      }
      default:
      {
         USES_CONVERSION;

         //CString svalue = W2A(pProp->GetValue().bstrVal);
         std::wstring ws(pProp->GetValue().bstrVal, SysStringLen(pProp->GetValue().bstrVal));
         std::string s(ws.begin(), ws.end());
         prop->set_value(s.c_str());
      }
      }
   }
}

//====================================================================================================================================================