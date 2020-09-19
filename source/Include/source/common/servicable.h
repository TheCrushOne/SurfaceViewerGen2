#pragma once

#include "navdisp/ComService.h"

class Servicable
{
public:
   Servicable(navigation_dispatcher::iComService* service)
      : m_pService(service)
   {}
   navigation_dispatcher::iComService* GetService() const { return m_pService; }
protected:
   void checkService() const { _ASSERT(m_pService); }
private:
   navigation_dispatcher::iComService* m_pService;
};