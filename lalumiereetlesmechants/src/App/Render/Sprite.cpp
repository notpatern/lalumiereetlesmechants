#include "Sprite.h"
#include "Renderer.h"

Sprite::Sprite()
{
	Renderer::getInstance().AddToRenderQueue(this);
}

Sprite::~Sprite()
{
}

void Sprite::Render() {
	
}
