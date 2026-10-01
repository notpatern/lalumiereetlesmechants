#include "Missile.h"

Missile::Missile() : m_isActive(false)
{

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

void Missile::Launch(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity)
{
	m_lifetime.Start(LIFETIME);
	m_position = position;
	m_velocity = velocity;

	m_isActive = true;
}

void Missile::Deactivate()
{
	m_isActive = false;
}


