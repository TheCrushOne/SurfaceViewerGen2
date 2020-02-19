#include "stdafx.h"
#include "UnitLayer.h"

// HACK: шайтан каст...но без него не линкуется автоконтейнер
AutoContainer< LayersContainer>::ListObjects AutoContainer< LayersContainer>::_objects;
namespace
{
   UnitLayer layer;
   //const auto imageSuggestion = fpath::get_module_folder().append("images\\tip.png");
}

//std::string get_suggestion_text(const colreg::statefull::suggestion& suggestion);

void UnitLayer::Render(render::iRender* renderer)
{
   
}