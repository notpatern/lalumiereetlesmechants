#pragma once

#include "../../Utilities/Vector2.h"

class World
{
private:
	Utility::Vector2<int> m_size;
	int* m_lightMap;
	int* m_worldMap;

public:
	explicit World(); 
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	~World();

	void SetArraySizes(const Utility::Vector2<int>& size);
	bool CanAccessMap(const Utility::Vector2<int>& position);
	void SetMapValue(const Utility::Vector2<int>& position, int value, int* const map);

	int* getLightMap() {
		return m_lightMap;
	}

	int* getWorldMap() {
		return m_worldMap;
	}

	Utility::Vector2<int> getSize() {
		return m_size;
	}
};
