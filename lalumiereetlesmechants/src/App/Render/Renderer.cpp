#include "Renderer.h"
#include <iostream>

Renderer::Renderer(World& world) : m_world(world)
{
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
	std::cout << CanAccessMap(m_world, Utility::Vector2<int>(0, 0)) << std::endl;
}
