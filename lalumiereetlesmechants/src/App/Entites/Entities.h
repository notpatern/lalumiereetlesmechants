#pragma once

#include "../Render/Sprite.h"
#include "../../Utilities/Vector2.h"

class Entity : public Sprite
{
protected:
	Utility::Vector2<float> m_position;

public:
	Entity();
	Entity(const Utility::Vector2<float>& position);
	~Entity();

	inline Utility::Vector2<float>& getPosition() {
		return m_position;
	}

	void setPosition(Utility::Vector2<float>& newPos) {
		m_position = newPos;
	}

	void setPosition(float x, float y) {
		m_position.x = x;
		m_position.y = y;
	}

	virtual void Update() = 0;
};
