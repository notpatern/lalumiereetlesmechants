#include "Enemy.h"

Enemy::Enemy() : m_health(MAX_HEALTH), m_isActive(false)
{
}

void Enemy::Spawn(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity)
{
	m_position = position;
	m_velocity = velocity;
	m_health.Reset();
	m_hitFlash.Start(0.f);
	m_isActive = true;
}

void Enemy::Update(float dt)
{
	if (!m_isActive)
	{
		return;
	}

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_hitFlash.Update(dt);
}

void Enemy::Deactivate()
{
	m_isActive = false;
}

void Enemy::TakeDamage(const DamageInfos& damage)
{
	if (!m_isActive)
	{
		return;
	}

	if (m_health.TakeDamage(damage.amount))
	{
		Die();
	}
	else
	{
		m_hitFlash.Start(HIT_FLASH_DURATION);
	}
}

void Enemy::Die()
{
	Deactivate();
}
