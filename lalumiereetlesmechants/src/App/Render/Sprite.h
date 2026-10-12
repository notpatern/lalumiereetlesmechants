#pragma once

#include "../World/World.h"
#include "../../Utilities/Vector2.h"
#include <vector>

class Sprite
{
private:
	Utility::Vector2<int> m_position;
	Utility::Vector2<int> m_size;

	std::vector<std::vector<char>> m_sprite;

public:
	Sprite(const std::vector<std::vector<char>>& sprite);
	~Sprite();

	void Render(const World& world);
	inline void SetSpritePosition(Utility::Vector2<int> position)
	{
		m_position = position;
	}
};

