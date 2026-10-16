#include "Missile.h"

Missile::Missile() : m_team(Team::Player), m_damage(0)
{
}

void Missile::Launch(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity, Team team, int damage, const Utility::Vector2<int>& hitboxSize)
{
	m_lifetime.Start(LIFETIME);
	m_lastPosition = position;
	m_position = position;
	m_velocity = velocity;
	m_team = team;
	m_damage = damage;
	m_hitboxSize = hitboxSize;
	if (team == Team::Enemy)
	{
		const bool isBig = hitboxSize.x > 1 || hitboxSize.y > 1;
		m_spriteType = isBig ? SpriteUtils::SpriteType::BigEnemyMissile : SpriteUtils::SpriteType::EnemyMissile;
	}
	else
	{
		m_spriteType = SpriteUtils::SpriteType::PlayerMissile;
	}

	Activate();
}

DamageInfos Missile::getDamageInfos() const
{
	return { .amount = m_damage, .source = m_team, .position = m_position };
}

void Missile::OnLeftPlayArea()
{
	Deactivate();
}

void Missile::Explode()
{
	Deactivate();
}

DamageResult Missile::OnHit(IDamageable& damageable)
{
	const DamageResult result = damageable.TakeDamage(getDamageInfos());
	Explode();
	return result;
}

void Missile::Update(float dt)
{
	if (!getIsActive())
	{
		return;
	}

	m_lastPosition = m_position;

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_lifetime.Update(dt);
	if (m_lifetime.IsFinished())
	{
		Deactivate();
	}
}

