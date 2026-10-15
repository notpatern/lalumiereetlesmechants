#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"
#include "../Gameplay/DamageInfos.h"
#include "../Gameplay/IDamageable.h"
#include "../Render/Sprite.h"
#include "../Gameplay/Collision.h"
#include "Entities.h"

class Missile : public Entity
{
public:
	Missile();

	Utility::Vector2<float> getCurrentPosition() const { return m_position; }
	Utility::Vector2<float> getLastPosition() const { return m_lastPosition; }

	void Update(float dt);

	bool getIsActive() const { return m_isActive; }

	void Launch(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity, Team team, int damage, const Utility::Vector2<int>& hitboxSize = { 1, 1 });
	void Explode();

	DamageResult OnHit(IDamageable& damageable);

	Team getTeam() const { return m_team; }
	Utility::Vector2<int> getHitboxSize() const { return m_hitboxSize; }
	DamageInfos getDamageInfos() const;
	void OnLeftPlayArea();

private:
	Utility::Vector2<float> m_lastPosition;
	Utility::Vector2<float> m_velocity;

	static constexpr float LIFETIME = 10.f;
	Utility::Countdown m_lifetime;

	Team m_team;
	int m_damage;
	Utility::Vector2<int> m_hitboxSize{ 1, 1 };

	bool m_isActive;

	void Deactivate();

};

