#pragma once
#include "PooledEntity.h"
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"

class Particle : public PooledEntity
{
public:
	void Spawn(const Utility::Vector2<float>& position, float angle);
	void Update(float dt);
	void OnLeftPlayArea();

	bool IsSmoke() const { return m_lifetime.getRemaining() <= SMOKE_DURATION; }

private:
	static constexpr float SPEED = 8.f;
	static constexpr float LIFETIME = 0.4f;
	static constexpr float SMOKE_DURATION = 0.2f;

	Utility::Vector2<float> m_velocity;
	Utility::Countdown m_lifetime;
};
