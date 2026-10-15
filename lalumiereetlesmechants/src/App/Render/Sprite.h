#pragma once

#include "../World/World.h"
#include "../../Utilities/Vector2.h"
#include <vector>
#include "SpriteType.h"

class Sprite
{
private:
	Utility::Vector2<int> m_position;
	Utility::Vector2<int> m_size;

protected:
	SpriteType m_spriteType;

public:
	Sprite();
	Sprite(SpriteType spriteType);
	~Sprite();

	//void Render(const World& world);
	inline void SetSpritePosition(Utility::Vector2<int> position)
	{
		m_position = position;
	}
};
