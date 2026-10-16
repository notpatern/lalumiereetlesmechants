#pragma once

#include "../../Utilities/Vector2.h"
#include <cstdint>

class World
{
private:
	Utility::Vector2<int> m_size;
	uint16_t* m_lightMap; // remanant d'une epoque où la scope fait la taille de mon ego
	uint16_t* m_worldMap; // array 1D pour profiter d'une rapidité d'acces

public:
	explicit World(); 
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	~World();

	void SetArraySizes(const Utility::Vector2<int>& size);
	friend bool CanAccessMap(const World& world, const Utility::Vector2<int>& position);
	friend void SetMapValue(const World& world, const Utility::Vector2<int>& position, int value, uint16_t* const map);
	friend char GetWorldMapValue(const World& world, const Utility::Vector2<int>& position);
	friend char GetLightMapValue(const World& world, const Utility::Vector2<int>& position); // pareil on ne se sert plus de light map

	// ici on a des methods friend pour que tout le monde puisse les call en ayant une instance de world
	// sans avoir à faire de singleton ou de methodes statiques
	// c'est aussi un moyen pour nous d'apprendre à utiliser ce keyword meme si il n'est pas obligatoire dans notre archi

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
