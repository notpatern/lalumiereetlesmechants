#include "XpOrb.h"
#include <algorithm>
#include "../Gameplay/PlayArea.h"
#include "../../Utilities/ScreenDirection.h"

void XpOrb::Spawn(const Utility::Vector2<float>& position, int value)
{
	m_position = position;
	m_value = value;
	m_speed = 0.f;
	m_isAttracted = false;
	m_spriteType = SpriteUtils::SpriteType::XpOrb;
	Activate();
}

void XpOrb::Update(float dt, const Utility::Vector2<float>& playerCenter)
{
	if (!m_isAttracted && Utility::ScreenDistanceSquared(m_position, playerCenter) > MAGNET_RADIUS * MAGNET_RADIUS)
	{
		m_position.y += PlayArea::SCROLL_SPEED * dt;
		return;
	}

	m_isAttracted = true;
	FlyToward(playerCenter, dt);
}

void XpOrb::FlyToward(const Utility::Vector2<float>& target, float dt)
{
	m_speed = std::min(m_speed + MAGNET_ACCELERATION * dt, MAGNET_MAX_SPEED);
	const float step = m_speed * dt;

	if (Utility::ScreenDistanceSquared(m_position, target) <= step * step)
	{
		m_position = target;
		return;
	}

	const Utility::Vector2<float> direction = Utility::ScreenDirection(m_position, target);
	m_position.x += direction.x * step;
	m_position.y += direction.y * step;
}

void XpOrb::Collect()
{
	Deactivate();
}

void XpOrb::OnLeftPlayArea()
{
	Deactivate();
}
