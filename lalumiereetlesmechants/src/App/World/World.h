#pragma once

#include "../../Utilities/Vector2.h"
#include <cstdint>

class World
{
private:
	Utility::Vector2<int> m_size;
	uint16_t* m_lightMap;
	uint16_t* m_worldMap;

public:
	explicit World(); 
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	~World();

	void SetArraySizes(const Utility::Vector2<int>& size);
	friend bool CanAccessMap(const World& world, const Utility::Vector2<int>& position);
	friend void SetMapValue(const World& world, const Utility::Vector2<int>& position, int value, uint16_t* const map);
	friend char GetWorldMapValue(const World& world, const Utility::Vector2<int>& position);
	friend char GetLightMapValue(const World& world, const Utility::Vector2<int>& position);

	uint16_t* getLightMap() const {
		return m_lightMap;
	}

	uint16_t* getWorldMap() const {
		return m_worldMap;
	}

	Utility::Vector2<int> getSize() {
		return m_size;
	}
};
