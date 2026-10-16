#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"
#include "../Gameplay/DamageInfos.h"
#include "../Gameplay/IDamageable.h"
#include "../Gameplay/MissileArchetype.h"
#include "PooledEntity.h"

class Missile : public PooledEntity
{
public:
	Missile();

	void Launch(MissileType inType, const Utility::Vector2<float>& inPosition, const Utility::Vector2<float>& inVelocity);
	void Update(float inDeltaTime);
	DamageResult OnHit(IDamageable& outDamageable);
	void OnLeftPlayArea();

	MissileType getType() const { return m_type; }
	Team getTeam() const { return m_team; }
	Utility::Vector2<float> getLastPosition() const { return m_lastPosition; }
	Utility::Vector2<int> getHitboxSize() const { return m_hitboxSize; }
	DamageInfos getDamageInfos() const;

private:
	static constexpr float LIFETIME = 10.f;

	Utility::Vector2<float> m_lastPosition;
	Utility::Vector2<float> m_velocity;
	Utility::Countdown m_lifetime;

	MissileType m_type;
	Team m_team;
	Utility::Vector2<int> m_hitboxSize{ 1, 1 };
};
