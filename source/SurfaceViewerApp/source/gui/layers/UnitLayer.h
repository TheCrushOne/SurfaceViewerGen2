#pragma once
#include "RenderLayers.h"

class UnitLayer : public LayersContainer
{
public:
   void Render(render::iRender* renderer) override;
private:
   //void renderSuggestions(render::iRender* renderer, const colreg::statefull::suggestions_ref* suggestions);
};