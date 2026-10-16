#pragma once
#include "PooledEntity.h"
#include "../../Utilities/Vector2.h"

class XpOrb : public PooledEntity
{
public:
	void Spawn(const Utility::Vector2<float>& position, int value);
	void Update(float dt);
	void OnLeftPlayArea();

	int getValue() const { return m_value; }

private:
	static constexpr float FALL_SPEED = 4.f;

	int m_value = 0;
};
