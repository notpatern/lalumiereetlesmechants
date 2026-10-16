#include "Particle.h"
#include "../Gameplay/PlayArea.h"
#include "../../Utilities/ScreenDirection.h"

void Particle::Spawn(const Utility::Vector2<float>& position, float angle)
{
	m_position = position;
	m_velocity = Utility::ScreenVector(angle, SPEED);
	m_lifetime.Start(LIFETIME);
	m_spriteType = SpriteUtils::SpriteType::Spark;
	Activate();
}

void Particle::Update(float dt)
{
	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_lifetime.Update(dt);
	if (m_lifetime.IsFinished())
	{
		Deactivate();
		return;
	}

	if (IsSmoke())
	{
		m_spriteType = SpriteUtils::SpriteType::Smoke;
		m_position.y += PlayArea::SCROLL_SPEED * dt;
	}
}

void Particle::OnLeftPlayArea()
{
	Deactivate();
}
