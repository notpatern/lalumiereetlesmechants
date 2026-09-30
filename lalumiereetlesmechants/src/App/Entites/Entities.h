#pragma once

#include "../Render/Sprite.h"
#include "../../Utilities/Vector2.h"

class Entities
{
private:
	Utilities::Vector2<int>* m_position;
	Sprite* m_sprite;
public:
	Entities();
	~Entities();

	inline Sprite* getSprite() {
		return m_sprite;
	}

	inline void setSprite(Sprite* sprite) {
		m_sprite = sprite;
	}

	inline Utilities::Vector2<int>* getPosition() {
		return m_position;
	}

	void setPosition(Utilities::Vector2<int>* newPos) {
		m_position = newPos;
	}

	void setPosition(Utilities::Vector2<int>* newPos) {

	}

	void setPosition(Utilities::Vector2<int>* newPos) {

	}

	void setPosition(Utilities::Vector2<int>* newPos) {

	}
};

