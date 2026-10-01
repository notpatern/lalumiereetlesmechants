#include "World.h"

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
	m_lightMap = new int[size.x * size.y]();
	m_worldMap = new int[size.x * size.y]();
}

bool World::CanAccessMap(const Utility::Vector2<int>& position)
{
	if (position.x >= 0 && position.x < m_size.x && position.y >= 0 && position.y < m_size.y) {
		return true;
	}
	return false;
}

void World::SetMapValue(const Utility::Vector2<int>& position, int value, int* const map)
{
	int oneDX = ((position.x % m_size.x) + m_size.x) % m_size.x;
	int oneDY = ((position.y % m_size.y) + m_size.y) % m_size.y;
	int index = oneDY * m_size.x + oneDX;

	map[index] = value;
}
