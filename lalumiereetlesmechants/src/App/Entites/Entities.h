#pragma once

#include "../Render/Sprite.h"
#include "../../Utilities/Vector2.h"

class Entity
{
protected:
	Utility::Vector2<int> m_position;
	Sprite* m_sprite;

public:
	Entity();
	Entity(const Utility::Vector2<int>& position, Sprite* sprite);
	~Entity();

	inline Sprite* getSprite() {
		return m_sprite;
	}

	inline void setSprite(Sprite* sprite) {
		m_sprite = sprite;
	}

	inline Utility::Vector2<int>& getPosition() {
		return m_position;
	}

	void setPosition(Utility::Vector2<int>& newPos) {
		m_position = newPos;
	}

	void setPosition(Utility::Vector2<float>& newPos) {
		m_position.x = (int)newPos.x;
		m_position.y = (int)newPos.y;
	}

	void setPosition(int x, int y) {
		m_position.x = x;
		m_position.y = y;
	}

	virtual void Update() = 0;
};
