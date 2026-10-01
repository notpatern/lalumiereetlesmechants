#pragma once

#include "../World/World.h"

class Renderer
{
private:
	World& m_world;

public:
	explicit Renderer(World& world);
	~Renderer();

	void Render();
};

