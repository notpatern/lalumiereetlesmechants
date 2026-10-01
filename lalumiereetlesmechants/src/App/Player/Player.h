#pragma once
#include "../../Utilities/Vector2.h"
#include "../Render/Sprite.h"

class Player
{
public:
	Sprite* getSprite() {return m_sprite;}
	Utility::Vector2<int>& getPosition() { return m_currentPosition; }
	int getSpeed() const { return m_speed; }

private:
	Sprite* m_sprite = nullptr;
	Utility::Vector2<int> m_currentPosition;
	int m_speed{5};
	void move(Utility::Vector2<int> desiredDirection);
	void shoot();
};

