#include "Particle.h"
#include "../Gameplay/PlayArea.h"
#include "../../Utilities/ScreenDirection.h"

void Particle::Spawn(const Utility::Vector2<float>& inPosition, float inAngle, SpriteUtils::SpriteType inSpark, SpriteUtils::SpriteType inSmoke)
{
	m_position = inPosition;
	m_velocity = Utility::ScreenVector(inAngle, SPEED);
	m_lifetime.Start(LIFETIME);
	m_spriteType = inSpark;
	m_smokeSprite = inSmoke;
	Activate();
}

void Particle::Update(float inDeltaTime)
{
	m_position.x += m_velocity.x * inDeltaTime;
	m_position.y += m_velocity.y * inDeltaTime;

	m_lifetime.Update(inDeltaTime);
	if (m_lifetime.IsFinished())
	{
		Deactivate();
		return;
	}

	if (IsSmoke())
	{
		m_spriteType = m_smokeSprite;
		m_position.y += PlayArea::SCROLL_SPEED * inDeltaTime;
	}
}

void Particle::OnLeftPlayArea()
{
	Deactivate();
}
