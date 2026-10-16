#include "Missile.h"

Missile::Missile() : m_type(MissileType::PlayerShot), m_team(Team::Player)
{
}

void Missile::Launch(MissileType inType, const Utility::Vector2<float>& inPosition, const Utility::Vector2<float>& inVelocity)
{
	const MissileArchetype& archetype = MissileArchetypes::Get(inType);

	m_lifetime.Start(LIFETIME);
	m_lastPosition = inPosition;
	m_position = inPosition;
	m_velocity = inVelocity;
	m_type = inType;
	m_team = archetype.team;
	m_hitboxSize = archetype.hitboxSize;
	m_spriteType = archetype.sprite;

	Activate();
}

void Missile::Update(float inDeltaTime)
{
	m_lastPosition = m_position;

	m_position.x += m_velocity.x * inDeltaTime;
	m_position.y += m_velocity.y * inDeltaTime;

	m_lifetime.Update(inDeltaTime);
	if (m_lifetime.IsFinished())
	{
		Deactivate();
	}
}

DamageResult Missile::OnHit(IDamageable& outDamageable)
{
	const DamageResult result = outDamageable.TakeDamage(getDamageInfos());
	Deactivate();
	return result;
}

void Missile::OnLeftPlayArea()
{
	Deactivate();
}

DamageInfos Missile::getDamageInfos() const
{
	return { .amount = MissileArchetypes::Get(m_type).damage, .source = m_team, .position = m_position };
}
