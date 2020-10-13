#pragma once

#include "navdisp/ComService.h"

class Servicable
{
public:
   Servicable(navigation_dispatcher::iComServicePtr service)
      : m_pService(service)
   {}
   navigation_dispatcher::iComServicePtr GetService() const { return m_pService; }
protected:
   void checkService() const { _ASSERT(m_pService); }
private:
   navigation_dispatcher::iComServicePtr m_pService;
};