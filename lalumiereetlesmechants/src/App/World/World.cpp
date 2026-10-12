#include "World.h"
#include <iostream>

World::World() : m_size(Utility::Vector2<int>::zero())
{
}

World::~World()
{
	delete[] m_lightMap;
	delete[] m_worldMap;
}

void World::SetArraySizes(const Utility::Vector2<int>& size)
{
	if (m_size == Utility::Vector2<int>::zero()) {
		m_size = size;
	}
	m_lightMap = new uint16_t[size.x * size.y]();
	m_worldMap = new uint16_t[size.x * size.y]();
	for (int i = 0; i++, i < size.x * size.y;) 
	{
		m_worldMap[i] = (uint16_t)'a';
		m_lightMap[i] = (uint16_t)2;
	}
}

bool CanAccessMap(const World& world, const Utility::Vector2<int>& position)
{

	return (position.x >= 0 && position.x < world.m_size.x && position.y >= 0 && position.y < world.m_size.y);
}

void SetMapValue(const World& world, const Utility::Vector2<int>& position, int value, uint16_t* const map)
{
	int oneDX = ((position.x % world.m_size.x) + world.m_size.x) % world.m_size.x;
	int oneDY = ((position.y % world.m_size.y) + world.m_size.y) % world.m_size.y;
	int index = oneDY * world.m_size.x + oneDX;

	map[index] = value;
}

char GetWorldMapValue(const World& world, const Utility::Vector2<int>& position)
{
	int oneDX = ((position.x % world.m_size.x) + world.m_size.x) % world.m_size.x;
	int oneDY = ((position.y % world.m_size.y) + world.m_size.y) % world.m_size.y;
	int index = oneDY * world.m_size.x + oneDX;

	return world.m_worldMap[index];
}

char GetLightMapValue(const World& world, const Utility::Vector2<int>& position)
{
	int oneDX = ((position.x % world.m_size.x) + world.m_size.x) % world.m_size.x;
	int oneDY = ((position.y % world.m_size.y) + world.m_size.y) % world.m_size.y;
	int index = oneDY * world.m_size.x + oneDX;

	return world.m_lightMap[index];
}
