#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"
#include "../Gameplay/DamageInfos.h"
#include "../Gameplay/IDamageable.h"
#include "../Gameplay/MissileArchetype.h"
#include "../Render/Sprite.h"
#include "../Gameplay/Collision.h"
#include "PooledEntity.h"

class Missile : public PooledEntity
{
public:
	Missile();

	Utility::Vector2<float> getLastPosition() const { return m_lastPosition; }

	void Update(float dt);

	void Launch(MissileType type, const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity);
	void Explode();

	DamageResult OnHit(IDamageable& damageable);

	MissileType getType() const { return m_type; }
	Team getTeam() const { return m_team; }
	Utility::Vector2<int> getHitboxSize() const { return m_hitboxSize; }
	DamageInfos getDamageInfos() const;
	void OnLeftPlayArea();

private:
	Utility::Vector2<float> m_lastPosition;
	Utility::Vector2<float> m_velocity;

	static constexpr float LIFETIME = 10.f;
	Utility::Countdown m_lifetime;

	MissileType m_type;
	Team m_team;
	Utility::Vector2<int> m_hitboxSize{ 1, 1 };

};

