#pragma once

#include "../World/World.h"
#include "Sprite.h"
#include <vector>

class Renderer
{
	static constexpr short SCREEN_WIDTH = 80;
	static constexpr short SCREEN_HEIGHT = 40;
private:
	World* m_world;
	std::vector<Sprite*> m_renderQueue{};

public:
	Renderer();
	Renderer(World* world);
	~Renderer();

	static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }

	void Render();
	void AddToRenderQueue(Sprite* sprite);
	void RemoveFromRenderQueue(Sprite* sprite);
};

