#include "Movement.h"
#include <algorithm>
#include <cmath>
#include "PlayArea.h"
#include "../../Utilities/ScreenDirection.h"

void Movement::Start(const MovementSettings& settings, const Utility::Vector2<float>& start, const Utility::Vector2<float>& velocity, const Utility::Vector2<float>& target, int bodyWidth)
{
	m_settings = settings;
	m_basePosition = start;
	m_velocity = velocity;
	m_elapsed = 0.f;
	m_maxX = static_cast<float>(PlayArea::WIDTH - bodyWidth);

	if (settings.speed > 0.f)
	{
		const Utility::Vector2<float> direction = Utility::ScreenDirection(start, target);
		m_velocity = { direction.x * settings.speed, direction.y * settings.speed };
	}
}

Utility::Vector2<float> Movement::Update(float deltaTime)
{
	m_elapsed += deltaTime;

	m_basePosition.x += m_velocity.x * deltaTime;
	m_basePosition.y += m_velocity.y * deltaTime;

	if (m_settings.bounceOnEdges)
	{
		BounceOnEdges();
	}

	if (m_settings.type == MovementType::Sine)
	{
		return { m_basePosition.x, m_basePosition.y + m_settings.amplitude * std::sin(m_elapsed * m_settings.frequency) };
	}

	return m_basePosition;
}

void Movement::BounceOnEdges()
{
	const bool passedRightEdge = m_velocity.x > 0.f && m_basePosition.x > m_maxX;
	const bool passedLeftEdge = m_velocity.x < 0.f && m_basePosition.x < 0.f;

	if (passedRightEdge || passedLeftEdge)
	{
		m_velocity.x = -m_velocity.x;
		m_basePosition.x = std::clamp(m_basePosition.x, 0.f, m_maxX);
	}
}
