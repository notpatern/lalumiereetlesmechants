#pragma once

#include "../../Utilities/Vector2.h"

class World
{
private:
	Utility::Vector2<int> m_size;
	int* m_lightMap;
	char* m_worldMap;

public:
	explicit World(); 
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	~World();

	void SetArraySizes(const Utility::Vector2<int>& size);
	friend bool CanAccessMap(const World& world, const Utility::Vector2<int>& position);
	friend void SetMapValue(World& world, const Utility::Vector2<int>& position, int value, int* const map);
	friend char GetWorldMapValue(World& world, const Utility::Vector2<int>& position);
	friend char GetLightMapValue(World& world, const Utility::Vector2<int>& position);

	int* getLightMap() {
		return m_lightMap;
	}

	char* getWorldMap() {
		return m_worldMap;
	}

	Utility::Vector2<int> getSize() {
		return m_size;
	}
};
