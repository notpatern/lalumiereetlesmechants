#pragma once

#include "../World/World.h"
#include "../../Utilities/Vector2.h"
#include <vector>
#include "SpriteType.h"

class Sprite // Tout les sprite sont AddToRenderQueue à leur instanciation et remove à leur destruction
{
private:
	Utility::Vector2<float>* m_position;
	Utility::Vector2<int> m_size;

protected:
	SpriteUtils::SpriteType m_spriteType;
	// SpriteType nous permet de load une seule texture mais de la render où il faut et quand il faut

public:
	Sprite();
	Sprite(SpriteUtils::SpriteType spriteType);
	~Sprite();

	//void Render(const World& world);
	inline void SetSpritePosition(Utility::Vector2<float>* position)
	{
		m_position = position;
	}

	inline SpriteUtils::SpriteType GetType()
	{
		return m_spriteType;
	}

	inline Utility::Vector2<float> GetPosition()
	{
		return *m_position;
	}
};
