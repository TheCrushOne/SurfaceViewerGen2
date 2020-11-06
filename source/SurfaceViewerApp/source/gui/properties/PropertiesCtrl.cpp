#include "stdafx.h"
#include "PropertiesCtrl.h"
#include "gui\Selection\SelectedObjectManager.h"
#include "../MainFrm.h"
#include <set>

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
      { VALUE_FORMAT_TYPE::VFT_SPEED, { "dd d kts"
                                      , "__._ kts" } },
      { VALUE_FORMAT_TYPE::VFT_SPEED_MS, { "dd d kts"
                                        , "__._ m/s" } },
      { VALUE_FORMAT_TYPE::VFT_COURSE, { "ddd d°"
                                       , "___._°" } },
      { VALUE_FORMAT_TYPE::VFT_DISTANCE, { "dd dd NM"
                                       ,   "__.__ NM" } },
      { VALUE_FORMAT_TYPE::VFT_DISTANCE_METERS, { "ddd m"
                                              ,   "___ m" } },

      { VALUE_FORMAT_TYPE::VFT_TIME_SEC, { "00000 s"
                                       ,   "_____ s" } },

      { VALUE_FORMAT_TYPE::VFT_PERCENT, { "ddd %"
                                      ,   "___ %" } },
      { VALUE_FORMAT_TYPE::VFT_COORD_LAT, { "+dd dddd°"
                                          , "___.____°" } },
      { VALUE_FORMAT_TYPE::VFT_COORD_LON, { "+ddd dddd°"
                                          , "____.____°" } },
   };
}


//====================================================================================================================================================

void CPropertiesCtrl::ShowProperties(iProperty* prop, bool fullReload)
{
   _selected = prop;
   // WARNING: не проходит AssertValid
   /*if (fullReload)
   {
      _prop_struct.clear();
      RemoveAll();
   }*/

   if (!prop)
      return;

   LockWindowUpdate();
   auto* childs = prop->get_child_list();
   std::set< std::string > addedFolders;
   for (auto child : *childs)
   {
      addFolder(child, nullptr, &_prop_struct);
      addedFolders.insert(child->get_name());
   }

   if (!fullReload)
   {  //ïî÷èñòèì ñòàðûå ôîëäåðà, êîòîðûõ íåò â íîâîé ñòðóêòóðå
      for (auto it = _prop_struct.begin(); it != _prop_struct.end();)
      {
         if (addedFolders.find(it->first) == addedFolders.end())
         {
            DeleteProperty(it->second.grid_prop);
            it = _prop_struct.erase(it);
         }
         else
         {
            ++it; //òàêîé ôîëäåð åñòü â íîâîé ñòðóêòóðå
         }
      }
   }
   AdjustLayout();
   UnlockWindowUpdate();
}

//====================================================================================================================================================

void CPropertiesCtrl::addFolder(iProperty* folder, CMFCPropertyGridProperty* pFolderProp, props_structure* parentStruct)
{
   if (!folder)
      return;
   if (folder->get_type() == PROPERTY_TYPE::PT_FOLRDER)
   {
      auto* childs = folder->get_child_list();

      bool newFolderCreated = false;
      props_structure* currentPropStruct = nullptr;
      CMFCPropertyGridProperty* pSubFolder = nullptr;
      auto subFolderItemsIt = parentStruct->find(folder->get_name());
      if (subFolderItemsIt == parentStruct->end() || subFolderItemsIt->second.prop_type != PROPERTY_TYPE::PT_FOLRDER)
      {
         if (subFolderItemsIt != parentStruct->end()) //ñâîéñòâî ñ òàêèì èìåíåì íàéäåíî , íî òèï îòëè÷àåòñÿ
         {
            DeleteProperty(subFolderItemsIt->second.grid_prop);
            parentStruct->erase(subFolderItemsIt);
         }
         newFolderCreated = true;
         pSubFolder = new CMFCPropertyGridProperty(folder->get_name());
         currentPropStruct = &parentStruct->insert({ folder->get_name(), { pSubFolder, folder->get_type() } }).first->second.prop_childs;
         pFolderProp ? pFolderProp->AddSubItem(pSubFolder) : AddProperty(pSubFolder, FALSE);
      }
      else
      {
         //èñïîëüçóåì ñóùåñòâóþùèé Folder
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
         if (childPropIt == currentPropStruct->end() || childPropIt->second.prop_type != child->get_type())
         {
            if (childPropIt != currentPropStruct->end()) //ñâîéñòâî íàéäåíî, íî òèï îòëè÷àåòñÿ, íóæíî ïåðåñîçäàòü
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
               childItem = new CMFCPropertyGridProperty(child->get_name(), !strcmp(child->get_value(), "0") ? (_variant_t)false : (_variant_t)true, child->get_description());
               break;
            }
            case PROPERTY_TYPE::PT_VALUE:
            {
               childItem = createMaskedProperty(child);
               break;
            }
            case PROPERTY_TYPE::PT_COLOR:
            {
               childItem = createColorProperty(child);
               break;
            }
            case PROPERTY_TYPE::PT_TEXT_LIST:
            {
               const auto list = child->get_list();
               const auto indx = atoi(child->get_value());

               childItem = new CMFCPropertyGridProperty(child->get_name(), list[indx].c_str(), child->get_description());
               for (const auto& t : list)
                  childItem->AddOption(t.c_str());
               break;
            }
            default:
            {
               childItem = new CMFCPropertyGridProperty(child->get_name(), (_variant_t)child->get_value(), child->get_description());
            }
            }

            currentPropStruct->insert({ child->get_name(), { childItem, child->get_type() } });
            pSubFolder->AddSubItem(childItem);
         }
         else //property alread added
         {
            childItem = childPropIt->second.grid_prop;

            if (child->get_type() == PROPERTY_TYPE::PT_TEXT_LIST)
            {
               const auto list = child->get_list();
               const auto indx = atoi(child->get_value());
               childItem->SetValue(list[indx].c_str());
            }
            else if (child->get_type() == PROPERTY_TYPE::PT_COLOR)
            {
            }
            else
            {
               auto oldValType = childItem->GetValue().vt;
               auto newVal = (_variant_t)child->get_value();
               newVal.ChangeType(oldValType); //íîâîå çíà÷åíèå äîëæíî ñîâïàäàòü ïî òèïó ñî ñòàðûì â ïëàíå VariantType , âíóòðåííåãî ïðåäñòàâëåíèÿ

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
         //óäàëèì ëèøíèå àéòåìû, êîòîðûé ìîãëè îñòàòüñÿ îò ñòàðîãî âûäåëåííîãî îáúåêòà
         for (auto it = currentPropStruct->begin(); it != currentPropStruct->end(); )
         {
            if (processedItems.find(it->first) == processedItems.end())
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

CMFCPropertyGridProperty* CPropertiesCtrl::createMaskedProperty(iProperty* const& child)
{
   CMFCPropertyGridProperty* nw = nullptr;
   auto valueMaskIt = valueMask.find(child->get_value_format_type());
   if (valueMaskIt != valueMask.end()) //standard ?
   {
      auto& valMask = valueMaskIt->second;
      if (valMask.editMask.compare("") == 0)
         nw = new CMFCPropertyGridProperty(child->get_name(), (_variant_t)child->get_value(), child->get_description());
      else
         nw = new CMFCPropertyGridProperty(child->get_name(), (_variant_t)child->get_value(), child->get_description(), 0, _T(valMask.editMask.c_str()), _T(valMask.editTemplate.c_str()));
   }
   else
   {
      switch (child->get_value_format_type())
      {
      case VALUE_FORMAT_TYPE::VFT_LOGFILE:
         nw = new CMFCPropertyGridFileProperty(child->get_name(), TRUE, child->get_value(), "log", 6, nullptr, child->get_description());
         //   nw->AllowEdit(FALSE);
         break;

      case VALUE_FORMAT_TYPE::VFT_FOLDERPATH:
         nw = new CMFCPropertyGridFileProperty(child->get_name(), child->get_value(), 0, child->get_description());
         //  nw->AllowEdit(FALSE);
         break;

      default:
         break;
      }
   }
   return nw;
}

CMFCPropertyGridProperty* CPropertiesCtrl::createColorProperty(iProperty* const& child)
{
   CustomColorProperty* nw = nullptr;
   nw = new CustomColorProperty(child->get_name(), (COLORREF)child->get_value());
   return nw;
}

//====================================================================================================================================================


void CPropertiesCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
   if (!_selected)
      return;

   if (pProp)
   {
      const auto& name = pProp->GetName();
      iProperty* prop = reinterpret_cast<iProperty*>(pProp->GetData());
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
         CString svalue = W2A(pProp->GetValue().bstrVal);
         for (size_t i = 0; i < list.size(); ++i)
         {
            if (list[i] == std::string(svalue))
            {
               std::stringstream s;
               s << i;
               prop->set_value(s.str().c_str());
               break;
            }
         }


         break;
      }
      case PROPERTY_TYPE::PT_COLOR:
      {
         CMFCPropertyGridColorProperty* nwProp = static_cast<CMFCPropertyGridColorProperty*>(pProp);
         std::stringstream buffer;
         buffer.setf(std::ios_base::boolalpha | std::ios_base::fixed);
         buffer << nwProp->GetColor();
         prop->set_value(buffer.str().c_str());
         break;
      }
      default:
      {
         USES_CONVERSION;

         CString svalue = W2A(pProp->GetValue().bstrVal);
         prop->set_value(svalue);
      }
      }
   }
}

//====================================================================================================================================================