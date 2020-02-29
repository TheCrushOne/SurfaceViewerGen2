#pragma once
#include "render/Render.h"
#include "common/base_class.h"
//#include "renderhelper.h"
#include "gui/selection/property.h"
//#include "..\UI\user_interface.h"
#include "gui/user_interface.h"

struct iRenderLayer abstract
{
   virtual void Render(render::iRender* renderer) = 0;
   virtual iProperty* GetProperties() { return nullptr; }
   virtual ~iRenderLayer() = default;
};


class LayersContainer
   : public iRenderLayer
   , public AutoContainer< LayersContainer>
{
public:
   virtual ~LayersContainer() = default;
   static void RenderLayers(render::iRender* renderer)
   {
      for (auto& obj : _objects)
      {
         if (obj->isLayerEnabled())
         {
            obj->Render(renderer);
         }

      }
   }

   static iProperty* GetLayerProperties()
   {
      if (!_properties)
      {
         auto folderProps = std::make_unique< FolderProperty>("Layers Properties");
         for (auto& obj : _objects)
         {
            if (auto p = obj->GetProperties())
            {
               folderProps->AddChild(p);
            }
         }

         if (folderProps->get_childs().arr && folderProps->get_childs().size)
         {
            _properties = std::move(folderProps);
         }
      }
      return _properties.get();
   }
protected:
   void Render(render::iRender* renderer) override {}
   virtual bool isLayerEnabled() const { return _layerVisible; }
   virtual void onLayerEnabledChanged() {};


private:
   static iPropertyPtr   _properties;

   bool _layerVisible = true;
   iPropertyPtr _prop_layerEnabled;

   void OnRenderEnabled()
   {
      onLayerEnabledChanged();
      user_interface::InvalidateView();
   }
protected:
   iProperty* GetLayerEnabledProperty()
   {
      if (_prop_layerEnabled)
      {
         return _prop_layerEnabled.get();
      }

      _prop_layerEnabled = std::make_unique< ValuePropertyHolder< LayersContainer, decltype(_layerVisible)>>
         ("Visible", "Enable or disable drawing this layer", false, VALUE_FORMAT_TYPE::VFT_NONE, this, &LayersContainer::_layerVisible, &LayersContainer::OnRenderEnabled, this);

      return _prop_layerEnabled.get();
   }
};

// to remove __declspec(selectany) - need create cpp file with instance
__declspec(selectany) iPropertyPtr LayersContainer::_properties;

//extern "C" __declspec(selectany) iPropGetter GetLayerProperties = &LayersContainer::GetLayerProperties;
