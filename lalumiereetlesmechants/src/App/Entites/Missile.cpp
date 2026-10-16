#include "Missile.h"

Missile::Missile() : m_type(MissileType::PlayerShot), m_team(Team::Player)
{
}

void Missile::Launch(MissileType type, const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity)
{
	const MissileArchetype& archetype = MissileArchetypes::Get(type);

	m_lifetime.Start(LIFETIME);
	m_lastPosition = position;
	m_position = position;
	m_velocity = velocity;
	m_type = type;
	m_team = archetype.team;
	m_hitboxSize = archetype.hitboxSize;
	m_spriteType = archetype.sprite;

	Activate();
}

DamageInfos Missile::getDamageInfos() const
{
	return { .amount = MissileArchetypes::Get(m_type).damage, .source = m_team, .position = m_position };
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

