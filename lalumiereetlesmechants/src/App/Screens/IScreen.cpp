#include "IScreen.h"
#include "../Render/Renderer.h"

IScreen::IScreen()
{
	Renderer::getInstance().AddToRenderQueue(this);
}

IScreen::~IScreen()
{
	Renderer::getInstance().RemoveFromRenderQueue(this);
}
