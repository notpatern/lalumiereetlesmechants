#pragma once
#include "PooledEntity.h"
#include "../../Utilities/Vector2.h"

class XpOrb : public PooledEntity
{
public:
	void Spawn(const Utility::Vector2<float>& inPosition, int inValue);
	void Update(float inDeltaTime, const Utility::Vector2<float>& inPlayerCenter);
	void Collect();
	void OnLeftPlayArea();

	int getValue() const { return m_value; }

private:
	static constexpr float MAGNET_RADIUS = 6.f;
	static constexpr float MAGNET_ACCELERATION = 60.f;
	static constexpr float MAGNET_MAX_SPEED = 30.f;

	int m_value = 0;

	float m_speed = 0.f;
	bool m_isAttracted = false;

	void FlyToward(const Utility::Vector2<float>& inTarget, float inDeltaTime);
};
