#pragma once
#include "RenderLayers.h"

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

   bool _renderVector = true;
   iPropertyPtr   _prop_renderVector;
};