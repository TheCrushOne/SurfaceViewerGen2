#pragma once
#include "RenderLayers.h"

namespace SV
{
   class UnitLayer : public LayersContainer
   {
   public:
      void Render(render::iRender* renderer) override;
   protected:
      void renderRovers(render::iRender* renderer);
      void renderDrones(render::iRender* renderer);
      void renderShips(render::iRender* renderer);
   private:
      //void renderSuggestions(render::iRender* renderer, const colreg::statefull::suggestions_ref* suggestions);

      bool m_renderVector = true;
      iPropertyPtr m_prop_renderVector;

      double m_arrowDist = 0.5;
   };
}