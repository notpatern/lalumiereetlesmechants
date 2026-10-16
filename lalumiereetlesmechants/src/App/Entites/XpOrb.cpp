#include "XpOrb.h"

void XpOrb::Spawn(const Utility::Vector2<float>& position, int value)
{
	m_position = position;
	m_value = value;
	m_spriteType = SpriteUtils::SpriteType::XpOrb;
	Activate();
}

void XpOrb::Update(float dt)
{
	m_position.y += FALL_SPEED * dt;
}

void XpOrb::OnLeftPlayArea()
{
	Deactivate();
}
