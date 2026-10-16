#pragma once
#include "PooledEntity.h"
#include "../Render/SpriteType.h"
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"

class Particle : public PooledEntity
{
public:
	void Spawn(const Utility::Vector2<float>& inPosition, float inAngle, SpriteUtils::SpriteType inSpark = SpriteUtils::SpriteType::Spark, SpriteUtils::SpriteType inSmoke = SpriteUtils::SpriteType::Smoke);
	void Update(float inDeltaTime);
	void OnLeftPlayArea();

	bool IsSmoke() const { return m_lifetime.getRemaining() <= SMOKE_DURATION; }
	SpriteUtils::SpriteType getSpriteType() const { return m_spriteType; }

private:
	static constexpr float SPEED = 8.f;
	static constexpr float LIFETIME = 0.4f;
	static constexpr float SMOKE_DURATION = 0.2f;

	Utility::Vector2<float> m_velocity;
	Utility::Countdown m_lifetime;
	SpriteUtils::SpriteType m_smokeSprite = SpriteUtils::SpriteType::Smoke;
};
