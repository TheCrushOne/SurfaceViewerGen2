#pragma once

#include "navdisp\ConfigDispatcher.h"
#include "common/central_class.h"
#include "navdisp/ComService.h"
#include "colreg/XmlProperties/xml_property_container.h"

namespace navigation_dispatcher
{
   class ConfigDispatcherImpl : public iConfigDispatcher, public Central
   {
      struct tag
      {
         static constexpr char root[] = "root";

         static constexpr char common[] = "common";
         static constexpr char orders[] = "orders";
         static constexpr char standarts[] = "data_standarts";
         static constexpr char order[] = "order";
         static constexpr char standart[] = "data_standart";
         static constexpr char id[] = "id";

         static constexpr char type[] = "type";
         static constexpr char name[] = "name";

         static constexpr char params[] = "params";
      };
   public:
      ConfigDispatcherImpl(central_pack* pack, iComService* pService)
         : Central(pack)
         , m_services(pService)
      {}
      void Dispatch(const char*) override final;
   protected:
      void readDataStandart(const xml_properties::PropertyItem&);
      void readOrder(const xml_properties::PropertyItem&);
   private:
      iComService* m_services;
   };
}