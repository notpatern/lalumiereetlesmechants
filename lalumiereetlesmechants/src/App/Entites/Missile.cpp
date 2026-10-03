#include "Missile.h"

Missile::Missile() : m_team(Team::Player), m_damage(0), m_isActive(false)
{
}

void Missile::Launch(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity, Team team, int damage)
{
	m_lifetime.Start(LIFETIME);
	m_position = position;
	m_velocity = velocity;
	m_team = team;
	m_damage = damage;

	m_isActive = true;
}

DamageInfos Missile::getDamageInfos() const
{
	return { .amount = m_damage, .source = m_team, .position = m_position };
}

void Missile::Deactivate()
{
	m_isActive = false;
}

void Missile::Explode()
{
	Deactivate();
}

void Missile::OnHit(IDamageable& damageable)
{
	damageable.TakeDamage(getDamageInfos());
	Explode();
}

void Missile::Update(float dt)
{
	if (!m_isActive)
	{
		return;
	}

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_lifetime.Update(dt);
	if (m_lifetime.IsFinished())
	{
		Deactivate();
	}
}

