#include "Movement.h"
#include <cmath>
#include "../../Utilities/ScreenDirection.h"

void Movement::Start(const MovementSettings& settings, const Utility::Vector2<float>& start, const Utility::Vector2<float>& velocity, const Utility::Vector2<float>& target)
{
	m_settings = settings;
	m_basePosition = start;
	m_velocity = velocity;
	m_elapsed = 0.f;

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

	if (m_settings.type == MovementType::Sine)
	{
		return { m_basePosition.x, m_basePosition.y + m_settings.amplitude * std::sin(m_elapsed * m_settings.frequency) };
	}

	return m_basePosition;
}
