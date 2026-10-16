#pragma once

#include "../Render/Sprite.h"
#include "../../Utilities/Vector2.h"

class Entity : public Sprite // un entity est forcement renderable et donc herite de Sprite
{
public:
	Entity();
	Entity(const Utility::Vector2<float>& position);
	~Entity();

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	const Utility::Vector2<float>& getPosition() const
	{
		return m_position;
	}

	void setPosition(const Utility::Vector2<float>& newPos)
	{
		m_position = newPos;
	}

	void setPosition(float x, float y)
	{
		m_position.x = x;
		m_position.y = y;
	}

protected:
	Utility::Vector2<float> m_position;
	void SetInRenderQueue(bool shouldBeInQueue);

private:
	bool m_isInRenderQueue = false;
};
