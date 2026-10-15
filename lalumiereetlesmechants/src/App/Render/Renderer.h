#pragma once

#include "../World/World.h"
#include "Sprite.h"
#include "../Screens/IScreen.h"
#include <vector>

class Renderer
{
	static constexpr short SCREEN_WIDTH = 80;
	static constexpr short SCREEN_HEIGHT = 40;
private:
	World* m_world;
	std::vector<Sprite*> m_spriteRenderQueue{};
	std::vector<IScreen*> m_screenRenderQueue{};

public:
	Renderer();
	Renderer(World* world);
	~Renderer();

	static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }

	void Render();
	void RenderSprite(Sprite* sprite);
	void AddToRenderQueue(Sprite* sprite);
	void AddToRenderQueue(IScreen* sprite);
	void RemoveFromRenderQueue(Sprite* sprite);
	void RemoveFromRenderQueue(IScreen* sprite);
};

