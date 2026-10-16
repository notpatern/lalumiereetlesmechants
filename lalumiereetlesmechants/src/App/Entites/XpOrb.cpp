#include "XpOrb.h"
#include "../Gameplay/PlayArea.h"
#include "../../Utilities/ScreenDirection.h"
#include <algorithm>

void XpOrb::Spawn(const Utility::Vector2<float>& inPosition, int inValue)
{
	m_position = inPosition;
	m_value = inValue;
	m_speed = 0.f;
	m_isAttracted = false;
	m_spriteType = SpriteUtils::SpriteType::XpOrb;
	Activate();
}

void XpOrb::Update(float inDeltaTime, const Utility::Vector2<float>& inPlayerCenter)
{
	if (!m_isAttracted && Utility::ScreenDistanceSquared(m_position, inPlayerCenter) > MAGNET_RADIUS * MAGNET_RADIUS)
	{
		m_position.y += PlayArea::SCROLL_SPEED * inDeltaTime;
		return;
	}

	m_isAttracted = true;
	FlyToward(inPlayerCenter, inDeltaTime);
}

void XpOrb::Collect()
{
	Deactivate();
}

void XpOrb::OnLeftPlayArea()
{
	Deactivate();
}

void XpOrb::FlyToward(const Utility::Vector2<float>& inTarget, float inDeltaTime)
{
	m_speed = std::min(m_speed + MAGNET_ACCELERATION * inDeltaTime, MAGNET_MAX_SPEED);
	const float step = m_speed * inDeltaTime;

	if (Utility::ScreenDistanceSquared(m_position, inTarget) <= step * step)
	{
		m_position = inTarget;
		return;
	}

	const Utility::Vector2<float> direction = Utility::ScreenDirection(m_position, inTarget);
	m_position.x += direction.x * step;
	m_position.y += direction.y * step;
}
